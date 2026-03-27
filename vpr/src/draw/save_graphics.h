#pragma once
/**
 * @file save_graphics.h
 * 
 * Manages saving of graphics in different file formats
 */

#ifndef NO_GRAPHICS

#include "ezgl/application.hpp"

#ifdef VPR_QT
#include "vpr_qtcompat.h"
// TODO: this should be part of EZGL
#else
#include <gtk/gtk.h>
#endif

void save_graphics(std::string extension, std::string file_name);
void save_graphics_dialog_box(GtkWidget* /*widget*/, ezgl::application* /*app*/);
void save_graphics_from_button(GtkWidget* /*widget*/, gint response_id, gpointer data);

#endif /* NO_GRAPHICS */
