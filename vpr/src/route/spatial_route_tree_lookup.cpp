#include "spatial_route_tree_lookup.h"

#include "globals.h"

SpatialRouteTreeLookup build_route_tree_spatial_lookup(const Netlist<>& net_list,
                                                       const vtr::vector<ParentNetId, t_bb>& net_bound_box,
                                                       ParentNetId net,
                                                       const RouteTreeNode& rt_root) {
    constexpr float BIN_AREA_PER_SINK_FACTOR = 4;

    auto& device_ctx = g_vpr_ctx.device();

    int fanout = net_list.net_sinks(net).size();

    t_bb bb = net_bound_box[net];
    float bb_area = (bb.xmax - bb.xmin) * (bb.ymax - bb.ymin);
    float bb_area_per_sink = bb_area / fanout;
    float bin_area = BIN_AREA_PER_SINK_FACTOR * bb_area_per_sink;

    /* Set a minimum bin dimension so that we don't get minuscule bin sizes
     * when flat routing is enabled and every LUT input becomes a sink.
     * (P.S. This took some time to debug.) */
    constexpr float MIN_BIN_DIM = 3;
    float bin_dim = std::max(MIN_BIN_DIM, std::ceil(std::sqrt(bin_area)));

    size_t bins_x = std::ceil(device_ctx.grid.width() / bin_dim);
    size_t bins_y = std::ceil(device_ctx.grid.height() / bin_dim);

    SpatialRouteTreeLookup spatial_lookup({bins_x, bins_y});

    update_route_tree_spatial_lookup_recur(rt_root, spatial_lookup);

    return spatial_lookup;
}

// Adds the sub-tree rooted at rt_node to the spatial look-up
void update_route_tree_spatial_lookup_recur(const RouteTreeNode& rt_node, SpatialRouteTreeLookup& spatial_lookup) {
    auto& device_ctx = g_vpr_ctx.device();
    const auto& rr_graph = device_ctx.rr_graph;

    RRNodeId rr_node = (RRNodeId)rt_node.inode;

    int bin_xlow = grid_to_bin_x(rr_graph.node_xlow(rr_node), spatial_lookup);
    int bin_ylow = grid_to_bin_y(rr_graph.node_ylow(rr_node), spatial_lookup);
    int bin_xhigh = grid_to_bin_x(rr_graph.node_xhigh(rr_node), spatial_lookup);
    int bin_yhigh = grid_to_bin_y(rr_graph.node_yhigh(rr_node), spatial_lookup);

    spatial_lookup[bin_xlow][bin_ylow].push_back(rt_node);

    // We currently look at the start/end locations of the RR nodes and add the node
    // to both bins if they are different
    //
    // TODO: Depending on bin size, long wires may end up being added only to bins at
    //      their start/end and may pass through bins along their length to which they
    //      are not added. If this becomes an issues, reconsider how we add nodes to
    //      bins
    if (bin_xhigh != bin_xlow || bin_yhigh != bin_ylow) {
        spatial_lookup[bin_xhigh][bin_yhigh].push_back(rt_node);
    }

    // Recurse
    for (auto& child : rt_node.child_nodes()) {
        update_route_tree_spatial_lookup_recur(child, spatial_lookup);
    }
}

size_t grid_to_bin_x(size_t grid_x, const SpatialRouteTreeLookup& spatial_lookup) {
    auto& device_ctx = g_vpr_ctx.device();

    int bin_width = std::ceil(float(device_ctx.grid.width()) / spatial_lookup.dim_size(0));

    return grid_x / bin_width;
}

size_t grid_to_bin_y(size_t grid_y, const SpatialRouteTreeLookup& spatial_lookup) {
    auto& device_ctx = g_vpr_ctx.device();

    int bin_height = std::ceil(float(device_ctx.grid.height()) / spatial_lookup.dim_size(1));

    return grid_y / bin_height;
}

bool validate_route_tree_spatial_lookup(const RouteTreeNode& rt_node, const SpatialRouteTreeLookup& spatial_lookup) {
    auto& device_ctx = g_vpr_ctx.device();
    const auto& rr_graph = device_ctx.rr_graph;
    RRNodeId rr_node = (RRNodeId)rt_node.inode;

    int bin_xlow = grid_to_bin_x(rr_graph.node_xlow(rr_node), spatial_lookup);
    int bin_ylow = grid_to_bin_y(rr_graph.node_ylow(rr_node), spatial_lookup);
    int bin_xhigh = grid_to_bin_x(rr_graph.node_xhigh(rr_node), spatial_lookup);
    int bin_yhigh = grid_to_bin_y(rr_graph.node_yhigh(rr_node), spatial_lookup);

    bool valid = true;

    auto& low_bin_rt_nodes = spatial_lookup[bin_xlow][bin_ylow];
    if (std::find(low_bin_rt_nodes.begin(), low_bin_rt_nodes.end(), rt_node) == low_bin_rt_nodes.end()) {
        valid = false;
        VPR_FATAL_ERROR(VPR_ERROR_ROUTE, "Failed to find route tree node %d at (low coord %d,%d) in spatial lookup [bin %d,%d]",
                        rt_node.inode, rr_graph.node_xlow(rr_node), rr_graph.node_ylow(rr_node), bin_xlow, bin_ylow);
    }

    auto& high_bin_rt_nodes = spatial_lookup[bin_xhigh][bin_yhigh];
    if (std::find(high_bin_rt_nodes.begin(), high_bin_rt_nodes.end(), rt_node) == high_bin_rt_nodes.end()) {
        valid = false;
        VPR_FATAL_ERROR(VPR_ERROR_ROUTE, "Failed to find route tree node %d at (high coord %d,%d) in spatial lookup [bin %d,%d]",
                        rt_node.inode, rr_graph.node_xhigh(rr_node), rr_graph.node_yhigh(rr_node), bin_xhigh, bin_yhigh);
    }

    // Recurse
    for (auto& child : rt_node.child_nodes()) {
        valid &= validate_route_tree_spatial_lookup(child, spatial_lookup);
    }

    return valid;
}
