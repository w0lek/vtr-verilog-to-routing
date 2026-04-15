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

void gtk_box_pack_start(QBoxLayout* box,
                        QWidget* widget,
                        bool expand,
                        bool fill,
                        int padding);



// dialog
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
// dialog

#endif // VPR_QTCOMPAT_H
