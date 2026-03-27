#ifndef VPR_QTCOMPAT2_H
#define VPR_QTCOMPAT2_H

#ifdef VPR_QT

#include <ezgl/qt/_qtcompat.hpp>

void gtk_button_set_label(QAbstractButton* button, const char* text);

void gtk_combo_box_text_append(QComboBox* combo,
                               const char* id,
                               const char* text);

QCheckBox* gtk_check_button_new_with_label(const QString& label);

QString g_strdup(const char* str);

void gtk_box_pack_start(QBoxLayout* box,
                        QWidget* widget,
                        bool expand,
                        bool fill,
                        int padding);

void gtk_switch_set_active(QCheckBox* button, bool flag);

using GtkBox = QBoxLayout;

#define GTK_SWITCH(w) qobject_cast<QCheckBox*>(w);

#endif // VPR_QT
#endif // VPR_QTCOMPAT2_H
