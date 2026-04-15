#include <vpr_qtcompat.h>

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

