#include <vpr_qtcompat.h>

void gtk_combo_box_text_append(QComboBox* combo,
                               const char* id,
                               const char* text)
{
  if (!combo) {
    return;
  }
  combo->addItem(QString::fromUtf8(text), QString::fromUtf8(id));
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

QGridLayout* get_grid_layout(QWidget* grid)
{
  if (!grid) {
    return nullptr;
  }

  QGridLayout* gridLayout = qobject_cast<QGridLayout*>(grid->layout());
  if (!gridLayout) {
    return nullptr;
  }
  return gridLayout;
}

QWidget* gtk_grid_get_child_at(QWidget* widget, int col, int row)
{
  QGridLayout* grid = get_grid_layout(widget);
  if (!grid) {
    return nullptr;
  }

  for (int i = 0; i < grid->count(); ++i) {
    int r, c, rs, cs;
    grid->getItemPosition(i, &r, &c, &rs, &cs);

    if (r == row && c == col) {
      if (auto item = grid->itemAt(i)) {
          return item->widget();
      }
    }
  }
  return nullptr;
}

void gtk_grid_attach(QWidget* widget, QWidget* child, int col, int row, int w, int h)
{
  QGridLayout* grid = get_grid_layout(widget);
  if (!grid) {
    return;
  }
  grid->addWidget(child, row, col, h, w);
}

void gtk_container_add(QWidget* container, QWidget* w)
{
  if (!container->layout()) {
    container->setLayout(new QVBoxLayout(container));
  }

  container->layout()->addWidget(w);
}

QWidget* gtk_grid_new()
{
  QWidget* w = new QWidget;
  w->setLayout(new QGridLayout);
  return w;
}

QDialog* gtk_dialog_new_with_buttons(
    const char* title,
    QWidget* parent,
    bool is_modal,
    const GtkDialogButton* buttons,
    int button_count
)
{
    QDialog* dialog = new QDialog(parent);
    dialog->setWindowTitle(title);

    if (is_modal) {
      dialog->setModal(true);
    }

    auto* layout = new QVBoxLayout(dialog);
    auto* buttonBox = new QDialogButtonBox(dialog);

    for (int i = 0; i < button_count; ++i) {
        const auto& b = buttons[i];

        QPushButton* btn =
            buttonBox->addButton(b.text, QDialogButtonBox::ActionRole);

        QObject::connect(btn, &QPushButton::clicked, dialog, [dialog, b]() {
            dialog->done(b.response);
        });
    }

    layout->addWidget(buttonBox);

    return dialog;
}

QWidget* gtk_dialog_get_content_area(QWidget* dialog)
{
  if (!dialog) return nullptr;

  if (!dialog->layout()) {
    auto* layout = new QVBoxLayout(dialog);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
  }

  return dialog;
}

void gtk_widget_set_halign(QWidget* w, int flag)
{
    Qt::Alignment qtAlign;
    if (flag == GTK_ALIGN_START) qtAlign = Qt::AlignLeft;
    else if (flag == GTK_ALIGN_END) qtAlign = Qt::AlignRight;
    else qtAlign = Qt::AlignHCenter;

    if (auto* label = qobject_cast<QLabel*>(w)) {
        label->setAlignment((label->alignment() & ~Qt::AlignHorizontal_Mask) | qtAlign);
    } else if (w->parentWidget() && w->parentWidget()->layout()) {
        w->parentWidget()->layout()->setAlignment(w, qtAlign);
    }
}
