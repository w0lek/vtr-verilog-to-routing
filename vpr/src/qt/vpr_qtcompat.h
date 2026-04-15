#ifndef VPR_QTCOMPAT_H
#define VPR_QTCOMPAT_H

#include <ezgl/qt/ezgl_qtcompat.hpp>

#include <QCheckBox>
#include <QBoxLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialog>
#include <QDialogButtonBox>

void gtk_combo_box_text_append(QComboBox* combo,
                               const char* id,
                               const char* text);

void gtk_box_pack_start(QBoxLayout* box,
                        QWidget* widget,
                        bool expand,
                        bool fill,
                        int padding);



using GtkGrid = QWidget; //QGridLayout is laying inside the widget

#define GTK_COMBO_BOX_TEXT(w) qobject_cast<QComboBox*>(reinterpret_cast<QObject*>(w))
#define GTK_GRID(w) qobject_cast<QWidget*>(w)
#define GTK_BOX(l) qobject_cast<QBoxLayout*>(l)

QGridLayout* get_grid_layout(QWidget* grid);
QWidget* gtk_grid_get_child_at(QWidget* widget, int col, int row);
void gtk_grid_attach(QWidget* widget, QWidget* child, int col, int row, int w, int h);
void gtk_container_add(QWidget* container, QWidget* w);
QWidget* gtk_grid_new();

struct GtkDialogButton {
    const char* text;
    int response;
};
QDialog* gtk_dialog_new_with_buttons(
    const char* title,
    QWidget* parent,
    bool is_modal,
    const GtkDialogButton* buttons,
    int button_count
);

QWidget* gtk_dialog_get_content_area(QWidget* dialog);

#define GTK_ALIGN_FILL   0
#define GTK_ALIGN_START  1
#define GTK_ALIGN_END    2
#define GTK_ALIGN_CENTER 3
void gtk_widget_set_halign(QWidget* w, int flag);

#endif // VPR_QTCOMPAT_H
