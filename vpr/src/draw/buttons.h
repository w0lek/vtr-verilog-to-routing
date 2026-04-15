#pragma once

#ifndef NO_GRAPHICS

#include <QWidget>

[[deprecated("todo: move to ezgl")]]
void delete_button(const std::string& button_name);
[[deprecated("todo: move to ezgl")]]
QWidget* find_button(const std::string& button_name);

#endif /* NO_GRAPHICS */
