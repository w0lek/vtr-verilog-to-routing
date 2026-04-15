#pragma once
/**
 * @file save_graphics.h
 * 
 * Manages saving of graphics in different file formats
 */

#ifndef NO_GRAPHICS

#include "ezgl/application.hpp"

#include <QWidget>

void save_graphics(std::string extension, std::string file_name);
void save_graphics_dialog_box(QWidget* /*widget*/, ezgl::application* /*app*/);
void save_graphics_from_button(QWidget* /*widget*/, int response_id, void* data);

#endif /* NO_GRAPHICS */
