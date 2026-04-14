/*
 * @file 	manual_moves.cpp
 * @author	Paula Perdomo
 * @date 	2021-07-19
 * @brief 	Contains the function definitions needed for manual moves feature.
 *
 * Includes the graphics/gtk function for manual moves. The Manual Move Generator class is defined  manual_move_generator.h/cpp.
 * The manual move feature allows the user to select a move by choosing the block to move, x position, y position, layer_position, subtile position.
 * If the placer accepts the move, the user can accept or reject the move with respect to the delta cost,
 * delta timing and delta bounding box cost displayed on the UI. The manual move feature interacts with placement through
 * the ManualMoveGenerator class in the manual_move_generator.cpp/h files and in the place.cpp file by checking
 * if the manual move toggle button in the UI is active or not, and calls the function needed.
 */

#ifndef NO_GRAPHICS

#include "manual_moves.h"
#include "draw_debug.h"
#include "globals.h"
#include "draw.h"
#include "draw_global.h"
#include "draw_searchbar.h"
#include "buttons.h"
#include "physical_types_util.h"

#include "vpr_qtcompat.h"

void draw_manual_moves_window(const std::string& block_id) {
    t_draw_state* draw_state = get_draw_state_vars();

    if (!draw_state->manual_moves_state.manual_move_window_is_open) {
        draw_state->manual_moves_state.manual_move_window_is_open = true;

        //Window settings-
        draw_state->manual_moves_state.manual_move_window = new QWidget;
        draw_state->manual_moves_state.manual_move_window->setAttribute(Qt::WA_DeleteOnClose);
        gtk_window_set_position((GtkWindow*)draw_state->manual_moves_state.manual_move_window, GTK_WIN_POS_CENTER);
        gtk_window_set_title((GtkWindow*)draw_state->manual_moves_state.manual_move_window, "Manual Moves Generator");
        draw_state->manual_moves_state.manual_move_window->setObjectName("manual_move_window");

        QWidget* grid = gtk_grid_new();
        QLineEdit* block_entry = new QLineEdit;

        if (draw_state->manual_moves_state.user_highlighted_block) {
            block_entry->setText(block_id.c_str());
            draw_state->manual_moves_state.user_highlighted_block = false;
        }

        QLineEdit* x_position_entry = new QLineEdit;
        QLineEdit* y_position_entry = new QLineEdit;
        QLineEdit* layer_position_entry = new QLineEdit;
        QLineEdit* subtile_position_entry = new QLineEdit;
        QLabel* block_label = new QLabel("Block ID/Block Name:");
        QLabel* to_label = new QLabel("To Location:");
        QLabel* x = new QLabel("x:");
        QLabel* y = new QLabel("y:");
        QLabel* layer = new QLabel("layer:");
        QLabel* subtile = new QLabel("Subtile:");

        QWidget* calculate_cost_button = gtk_button_new_with_label("Calculate Costs");

        //Add all to grid
        gtk_grid_attach((GtkGrid*)grid, block_label, 0, 0, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, block_entry, 0, 1, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, to_label, 2, 0, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, x, 1, 1, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, x_position_entry, 2, 1, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, y, 1, 2, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, y_position_entry, 2, 2, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, layer, 1, 3, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, layer_position_entry, 2, 3, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, subtile, 1, 4, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, subtile_position_entry, 2, 4, 1, 1);
        gtk_grid_attach((GtkGrid*)grid, calculate_cost_button, 0, 5, 3, 1); //spans three columns

        //Set margins
        widget_set_margin_bottom(grid, 20);
        widget_set_margin_top(grid, 20);
        widget_set_margin_start(grid, 20);
        widget_set_margin_end(grid, 20);
        widget_set_margin_bottom(block_label, 5);
        widget_set_margin_bottom(to_label, 5);
        widget_set_margin_top(calculate_cost_button, 15);
        widget_set_margin_start(x, 13);
        widget_set_margin_start(y, 13);
        gtk_widget_set_halign(calculate_cost_button, GTK_ALIGN_CENTER);

        //connect signals
        QObject::connect(Q_BUTTON(calculate_cost_button), &QAbstractButton::clicked,
                         [grid]() { calculate_cost_callback(nullptr, grid); });
        QObject::connect(draw_state->manual_moves_state.manual_move_window, &QObject::destroyed,
                         []() { close_manual_moves_window(); });

        gtk_container_add(GTK_CONTAINER(draw_state->manual_moves_state.manual_move_window), grid);
        draw_state->manual_moves_state.manual_move_window->show();
    }
}

void calculate_cost_callback(QWidget* /*widget*/, QWidget* grid) {
    int block_id = -1;
    bool valid_input = true;

    t_draw_state* draw_state = get_draw_state_vars();

    //Loading the context/data structures needed.
    const ClusteringContext& cluster_ctx = g_vpr_ctx.clustering();

    //Getting entry values
    QWidget* block_entry_widget = gtk_grid_get_child_at((GtkGrid*)grid, 0, 1);
    QLineEdit* block_entry = Q_LINEEDIT(block_entry_widget);
    std::string block_id_string = block_entry->text().toStdString();

    if (string_is_a_number(block_id_string)) { //for block ID
        block_id = std::atoi(block_id_string.c_str());
    } else { //for block name
        block_id = size_t(cluster_ctx.clb_nlist.find_block(block_id_string));
    }

    QWidget* x_position_entry_widget = gtk_grid_get_child_at((GtkGrid*)grid, 2, 1);
    QWidget* y_position_entry_widget = gtk_grid_get_child_at((GtkGrid*)grid, 2, 2);
    QWidget* layer_position_entry_widget = gtk_grid_get_child_at((GtkGrid*)grid, 2, 3);
    QWidget* subtile_position_entry_widget = gtk_grid_get_child_at((GtkGrid*)grid, 2, 4);

    QLineEdit* x_position_entry = Q_LINEEDIT(x_position_entry_widget);
    QLineEdit* y_position_entry = Q_LINEEDIT(y_position_entry_widget);
    QLineEdit* layer_position_entry = Q_LINEEDIT(layer_position_entry_widget);
    QLineEdit* subtile_position_entry = Q_LINEEDIT(subtile_position_entry_widget);

    int x_location = x_position_entry->text().toInt();
    int y_location = y_position_entry->text().toInt();
    int layer_location = layer_position_entry->text().toInt();
    int subtile_location = subtile_position_entry->text().toInt();

    if (block_entry->text().isEmpty() 
    || x_position_entry->text().isEmpty() 
    || y_position_entry->text().isEmpty() 
    || layer_position_entry->text().isEmpty() 
    || subtile_position_entry->text().isEmpty()) {
        invalid_breakpoint_entry_window("Not all fields are complete");
        valid_input = false;
    }

    t_pl_loc to = t_pl_loc(x_location, y_location, subtile_location, layer_location);
    valid_input = is_manual_move_legal(ClusterBlockId(block_id), to);

    if (valid_input) {
        draw_state->manual_moves_state.manual_move_info.valid_input = true;
        draw_state->manual_moves_state.manual_move_info.blockID = block_id;
        draw_state->manual_moves_state.manual_move_info.x_pos = x_location;
        draw_state->manual_moves_state.manual_move_info.y_pos = y_location;
        draw_state->manual_moves_state.manual_move_info.layer = layer_location;
        draw_state->manual_moves_state.manual_move_info.subtile = subtile_location;
        draw_state->manual_moves_state.manual_move_info.to_location = to;

        //Highlighting the block
        deselect_all();
        ClusterBlockId clb_index = ClusterBlockId(draw_state->manual_moves_state.manual_move_info.blockID);
        draw_highlight_blocks_color(cluster_ctx.clb_nlist.block_type(clb_index), clb_index);
        application.refresh_drawing();

        //Continues to move costs window.
        QWidget* proceed = find_button("ProceedButton");
        ezgl::press_proceed(proceed, &application);

    } else {
        draw_state->manual_moves_state.manual_move_info.valid_input = false;
    }
}

bool is_manual_move_legal(ClusterBlockId block_id, t_pl_loc to) {
    const ClusteringContext& cluster_ctx = g_vpr_ctx.clustering();
    const DeviceContext& device_ctx = g_vpr_ctx.device();
    t_draw_state* draw_state = get_draw_state_vars();
    const auto& grid_blocks = draw_state->get_graphics_blk_loc_registry_ref().grid_blocks();
    const auto& block_locs = draw_state->get_graphics_blk_loc_registry_ref().block_locs();

    //if the block is not found
    if ((!cluster_ctx.clb_nlist.valid_block_id(ClusterBlockId(block_id)))) {
        invalid_breakpoint_entry_window("Invalid block ID/Name");
        return false;
    }

    //If the dimensions are out of bounds
    if (to.x < 0 || to.x >= int(device_ctx.grid.width())
        || to.y < 0 || to.y >= int(device_ctx.grid.height())) {
        invalid_breakpoint_entry_window("Dimensions are out of bounds");
        return false;
    }

    //If the block s not compatible
    auto physical_tile = device_ctx.grid.get_physical_type({to.x, to.y, to.layer});
    auto logical_block = cluster_ctx.clb_nlist.block_type(block_id);
    if (to.sub_tile < 0 || to.sub_tile >= physical_tile->capacity || !is_sub_tile_compatible(physical_tile, logical_block, to.sub_tile)) {
        invalid_breakpoint_entry_window("Blocks are not compatible");
        return false;
    }

    //If the destination block is user constrained, abort this swap
    ClusterBlockId b_to = grid_blocks.block_at_location(to);
    if (b_to) {
        if (block_locs[b_to].is_fixed) {
            invalid_breakpoint_entry_window("Block is fixed");
            return false;
        }
    }

    //If the block requested is already in that location.
    t_pl_loc current_block_loc = block_locs[block_id].loc;
    if (to.x == current_block_loc.x && to.y == current_block_loc.y && to.sub_tile == current_block_loc.sub_tile) {
        invalid_breakpoint_entry_window("The block is currently in this location");
        return false;
    }

    return true;
}

bool manual_move_is_selected() {
    t_draw_state* draw_state = get_draw_state_vars();
    QCheckBox* manual_moves = application.find_check_box("manualMove");
    draw_state->manual_moves_state.manual_move_enabled = manual_moves->isChecked();
    return draw_state->manual_moves_state.manual_move_enabled;
}

void manual_move_cost_summary_dialog() {
    t_draw_state* draw_state = get_draw_state_vars();
    QDialog* dialog;
    QWidget* content_area;

    const GtkDialogButton btns[] = {
        {"Accept", GTK_RESPONSE_ACCEPT},
        {"Reject", GTK_RESPONSE_REJECT}
    };
    dialog = gtk_dialog_new_with_buttons("Move Costs",
                                         Q_WIDGET(draw_state->manual_moves_state.manual_move_window),
                                         GTK_DIALOG_MODAL,
                                         btns, 2);
    gtk_window_set_transient_for((GtkWindow*)dialog, (GtkWindow*)draw_state->manual_moves_state.manual_move_window);

    //Create elements for the dialog and printing costs to the user.
    QLabel* title_label = new QLabel;
    gtk_label_set_markup(title_label, "<b>Move Costs and Outcomes</b>");
    std::string delta_cost = "Delta Cost: " + std::to_string(draw_state->manual_moves_state.manual_move_info.delta_cost) + "   ";
    QLabel* delta_cost_label = new QLabel(delta_cost.c_str());
    std::string delta_timing = "   Delta Timing: " + std::to_string(draw_state->manual_moves_state.manual_move_info.delta_timing) + "   ";
    QLabel* delta_timing_label = new QLabel(delta_timing.c_str());
    std::string delta_bounding_box = "  Delta Bounding Box Cost: " + std::to_string(draw_state->manual_moves_state.manual_move_info.delta_bounding_box) + "   ";
    QLabel* delta_bounding_box_label = new QLabel(delta_bounding_box.c_str());
    std::string outcome = e_move_result_to_string(draw_state->manual_moves_state.manual_move_info.placer_move_outcome);
    std::string move_outcome = "  Annealing Decision: " + outcome + "   ";
    QLabel* move_outcome_label = new QLabel(move_outcome.c_str());
    QLabel* space_label1 = new QLabel("    ");
    QLabel* space_label2 = new QLabel("    ");

    //Attach elements to the content area of the dialog.
    content_area = gtk_dialog_get_content_area(dialog);
    gtk_container_add(GTK_CONTAINER(content_area), title_label);
    gtk_container_add(GTK_CONTAINER(content_area), space_label1);
    gtk_container_add(GTK_CONTAINER(content_area), delta_cost_label);
    gtk_container_add(GTK_CONTAINER(content_area), delta_timing_label);
    gtk_container_add(GTK_CONTAINER(content_area), delta_bounding_box_label);
    gtk_container_add(GTK_CONTAINER(content_area), move_outcome_label);
    gtk_container_add(GTK_CONTAINER(content_area), space_label2);

    //Show the dialog with all the labels.
    dialog->show();

    //Update message if user accepts the move.
    std::string msg = "Manual move accepted. Block #" + std::to_string(draw_state->manual_moves_state.manual_move_info.blockID);
    msg += " to location (" + std::to_string(draw_state->manual_moves_state.manual_move_info.x_pos) + ", " + std::to_string(draw_state->manual_moves_state.manual_move_info.y_pos) + ")";

    //Waiting for the user to respond to return to try_swa function.
    int result = dialog->exec();
    switch (result) {
        //If the user accepts the manual move
        case GTK_RESPONSE_ACCEPT:
            draw_state->manual_moves_state.manual_move_info.user_move_outcome = e_move_result::ACCEPTED;
            application.update_message(msg);
            break;
        //If the user rejects the manual move
        case GTK_RESPONSE_REJECT:
            draw_state->manual_moves_state.manual_move_info.user_move_outcome = e_move_result::REJECTED;
            application.update_message("Manual move was rejected");
            break;
        default:
            draw_state->manual_moves_state.manual_move_info.user_move_outcome = e_move_result::ABORTED;
            break;
    }

    //Destroys the move outcome dialog.
    dialog->deleteLater();
}

void manual_move_highlight_new_block_location() {
    t_draw_state* draw_state = get_draw_state_vars();
    const ClusteringContext& cluster_ctx = g_vpr_ctx.clustering();
    //Unselects all blocks first
    deselect_all();
    //Highlighting the block
    ClusterBlockId clb_index = ClusterBlockId(draw_state->manual_moves_state.manual_move_info.blockID);
    draw_highlight_blocks_color(cluster_ctx.clb_nlist.block_type(clb_index), clb_index);
    application.refresh_drawing();
}

//Manual move window turns false, the window is destroyed.
void close_manual_moves_window() {
    t_draw_state* draw_state = get_draw_state_vars();
    draw_state->manual_moves_state.manual_move_window_is_open = false;
}

bool string_is_a_number(const std::string& block_id) {
    return std::all_of(block_id.begin(), block_id.end(), isdigit);
}

//Updates ManualMovesInfo cost and placer move outcome variables. User_move_outcome is also updated.
e_move_result pl_do_manual_move(double d_cost, double d_timing, double d_bounding_box, e_move_result& move_outcome) {
    t_draw_state* draw_state = get_draw_state_vars();
    draw_state->manual_moves_state.manual_move_info.delta_cost = d_cost;
    draw_state->manual_moves_state.manual_move_info.delta_timing = d_timing;
    draw_state->manual_moves_state.manual_move_info.delta_bounding_box = d_bounding_box;
    draw_state->manual_moves_state.manual_move_info.placer_move_outcome = move_outcome;

    //Displays the delta cost to the user.
    manual_move_cost_summary_dialog();
    //User accepts or rejects the move.
    move_outcome = draw_state->manual_moves_state.manual_move_info.user_move_outcome;
    return move_outcome;
}

e_create_move manual_move_display_and_propose(ManualMoveGenerator& manual_move_generator,
                                              t_pl_blocks_to_be_moved& blocks_affected,
                                              e_move_type& move_type,
                                              float rlim,
                                              const t_placer_opts& placer_opts,
                                              const PlacerCriticalities* criticalities) {
    draw_manual_moves_window("");
    update_screen(ScreenUpdatePriority::MAJOR, " ", e_pic_type::PLACEMENT, nullptr);
    move_type = e_move_type::MANUAL_MOVE;
    t_propose_action proposed_action{move_type, -1}; //no need to specify block type in manual move "propose_move" function
    return manual_move_generator.propose_move(blocks_affected, proposed_action, rlim, placer_opts, criticalities);
}

#endif /*NO_GRAPHICS*/
