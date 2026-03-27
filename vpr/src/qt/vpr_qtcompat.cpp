#ifdef VPR_QT

#include <vpr_qtcompat.h>

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

void gtk_box_pack_start(QBoxLayout* box,
                        QWidget* widget,
                        bool expand,
                        bool fill,
                        int padding)
{
  if (!box || !widget) {
    return;
  }

  int stretch = expand ? 1 : 0;

  Qt::Alignment align = Qt::Alignment();
  if (!fill) {
    align = Qt::AlignLeft;
  }

  box->addWidget(widget, stretch, align);

  if (padding > 0) {
    box->setSpacing(padding);
  }
}

void gtk_switch_set_active(QCheckBox* button, bool flag)
{
  if (!button) {
    return;
  }
  button->setChecked(flag);
}

void gtk_widget_show_all(QWidget* w)
{
    w->show();
    for (auto child : w->findChildren<QWidget*>()) {
      child->show();
    }
}

bool gtk_toggle_button_get_active(GtkToggleButton* button) 
{
  if (!button) {
    return false;
  }
  return button->isChecked();
}

void gtk_toggle_button_set_active(GtkToggleButton* button, bool flag) 
{
  if (!button) {
    return;
  }
  button->setChecked(flag);
}

void gtk_widget_set_margin_start(QWidget* w, int m)
{
  if (!w) {
    return;
  }

  QMargins margins = w->contentsMargins();
  margins.setLeft(m);
  w->setContentsMargins(margins);
}

void gtk_widget_set_margin_end(QWidget* w, int m)
{
  if (!w) {
    return;
  }

  QMargins margins = w->contentsMargins();
  margins.setRight(m);
  w->setContentsMargins(margins);
}

void gtk_widget_set_margin_top(QWidget* w, int m)
{
  if (!w) {
    return;
  }

  QMargins margins = w->contentsMargins();
  margins.setTop(m);
  w->setContentsMargins(margins);
}

void gtk_widget_set_margin_bottom(QWidget* w, int m)
{
  if (!w) {
    return;
  }

  QMargins margins = w->contentsMargins();
  margins.setBottom(m);
  w->setContentsMargins(margins);
}

void gtk_window_close(QWidget* w)
{
  if (!w) {
    return; 
  }
  w->close();
}

#endif // VPR_QT
