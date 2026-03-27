#ifndef VPR_QTCOMPAT2_H
#define VPR_QTCOMPAT2_H

#ifdef VPR_QT

#include <ezgl/qt/_qtcompat.hpp>

void gtk_button_set_label(QAbstractButton* button, const char* text)
{
    if (!button) {
        return;
    }
    button->setText(text);
}

void gtk_combo_box_text_append(QComboBox* combo,
                               const char* id,
                               const char* text)
{
  if (!combo) {
    return;
  }
  combo->addItem(QString::fromUtf8(text), QString::fromUtf8(id));
}

QCheckBox* gtk_check_button_new_with_label(const QString& label)
{
    return new QCheckBox(label);
}

QString g_strdup(const char* str)
{
    QString s = str ? QString::fromUtf8(str) : QString();
    return s;
}

#define GTK_SWITCH(w) qobject_cast<QCheckBox*>(w);

#endif // VPR_QT
#endif // VPR_QTCOMPAT2_H
