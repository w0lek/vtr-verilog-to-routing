#pragma once

#ifndef NO_GRAPHICS

#ifdef VPR_QT
#include <ezgl/qt/_qtcompat.hpp>
#else // VPR_QT
#include <gtk/gtk.h>
#endif // VPR_QT

[[deprecated("todo: move to ezgl")]]
void delete_button(const char* button_name);
[[deprecated("todo: move to ezgl")]]
GtkWidget* find_button(const char* button_name);

#endif /* NO_GRAPHICS */
