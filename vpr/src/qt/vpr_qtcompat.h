#ifndef VPR_QTCOMPAT_H
#define VPR_QTCOMPAT_H

#include <ezgl/qt/ezgl_qtcompat.hpp>

#include <QCheckBox>
#include <QBoxLayout>

void gtk_button_set_label(QAbstractButton* button, const char* text);

void gtk_combo_box_text_append(QComboBox* combo,
                               const char* id,
                               const char* text);

QString g_strdup(const char* str);

void gtk_box_pack_start(QBoxLayout* box,
                        QWidget* widget,
                        bool expand,
                        bool fill,
                        int padding);

using GtkBox = QBoxLayout;

#define Q_ABSTRACT_BUTTON(w) qobject_cast<QAbstractButton*>(w);

#include <QRadioButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialog>
#include <QDialogButtonBox>

using GtkWindow = QWidget;
using GtkGrid = QWidget; //QGridLayout is laying inside the widget

void widget_set_margin_start(QWidget*, int);
void widget_set_margin_end(QWidget*, int);
void widget_set_margin_top(QWidget*, int);
void widget_set_margin_bottom(QWidget*, int);

void gtk_label_set_markup(QLabel* label, const QString& text);
const char* gtk_button_get_label(QPushButton* button);
QPushButton* gtk_button_new_with_label(const char* text);
const char* gtk_widget_get_name(QWidget* w);
void gtk_window_set_title(QWidget* w, const char* title);

#define GTK_COMBO_BOX_TEXT(w) qobject_cast<QComboBox*>(reinterpret_cast<QObject*>(w))
#define GTK_GRID(w) qobject_cast<QWidget*>(w)
#define GTK_BOX(l) qobject_cast<QBoxLayout*>(l)

QGridLayout* get_grid_layout(QWidget* grid);
QWidget* gtk_grid_get_child_at(QWidget* widget, int col, int row);
void gtk_grid_attach(QWidget* widget, QWidget* child, int col, int row, int w, int h);
void gtk_container_add(QWidget* container, QWidget* w);
QWidget* gtk_grid_new();

#define GTK_DIALOG_MODAL 0x1
#define GTK_RESPONSE_ACCEPT QDialog::Accepted
#define GTK_RESPONSE_REJECT QDialog::Rejected
struct GtkDialogButton {
    const char* text;
    int response;
};
QDialog* gtk_dialog_new_with_buttons(
    const char* title,
    QWidget* parent,
    int flags,
    const GtkDialogButton* buttons,
    int button_count
);

QWidget* gtk_dialog_get_content_area(QWidget* dialog);

#define GTK_WIN_POS_CENTER 0
void gtk_window_set_position(QWidget* w, int key);

#define g_list_free(x) ((void)0)

QList<QWidget*> gtk_container_get_children(QWidget* container);

#define GTK_ALIGN_FILL   0
#define GTK_ALIGN_START  1
#define GTK_ALIGN_END    2
#define GTK_ALIGN_CENTER 3
void gtk_widget_set_halign(QWidget* w, int flag);
void gtk_window_set_transient_for(QWidget* dialog, QWidget* parent);
QWidget* gtk_widget_get_parent_window(QWidget* w);

#endif // VPR_QTCOMPAT_H
