#include "noc_router.h"

// constructor
NocRouter::NocRouter(int id, int grid_position_x, int grid_position_y, int layer_position)
    : router_user_id(id)
    , router_grid_position_x(grid_position_x)
    , router_grid_position_y(grid_position_y)
    , router_layer_position(layer_position) {
    // initialize variables
    router_block_ref = ClusterBlockId(0);
}

// getters
int NocRouter::get_router_user_id(void) const {
    return router_user_id;
}

int NocRouter::get_router_grid_position_x(void) const {
    return router_grid_position_x;
}

int NocRouter::get_router_grid_position_y(void) const {
    return router_grid_position_y;
}

int NocRouter::get_router_layer_position(void) const {
    return router_layer_position;
}

t_physical_tile_loc NocRouter::get_router_physical_location(void) const {
    const int x = get_router_grid_position_x();
    const int y = get_router_grid_position_y();
    const int layer = get_router_layer_position();
    t_physical_tile_loc phy_loc{x, y, layer};

    return phy_loc;
}

ClusterBlockId NocRouter::get_router_block_ref(void) const {
    return router_block_ref;
}

// setters
void NocRouter::set_router_block_ref(ClusterBlockId router_block_ref_id) {
    router_block_ref = router_block_ref_id;
    return;
}