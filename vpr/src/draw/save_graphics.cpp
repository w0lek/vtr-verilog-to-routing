#ifndef NO_GRAPHICS

#include <cstdio>

#include "draw.h"
#include "save_graphics.h"
#include "search_bar.h"

extern ezgl::rectangle initial_world;

void save_graphics_from_button(QWidget* /*widget*/, [[maybe_unused]] int response_id, void* data) {
    QDialog* dialog = Q_DIALOG(static_cast<QObject*>(data));
    QLineEdit* text_entry = dialog->findChild<QLineEdit*>("file_name_text_entry");
    QComboBox* combo_box = dialog->findChild<QComboBox*>("file_name_combo_box");
    if (text_entry && combo_box) {
        std::string file_name = text_entry->text().toStdString();
        std::string extension = combo_box->currentText().toStdString();
        save_graphics(extension, file_name);
    }
}

void save_graphics(std::string extension, std::string file_name) {
    //Trim any leading '.' from the extension
    if (vtr::starts_with(extension, ".")) {
        extension = std::string(extension.begin() + 1, extension.end());
    }

    auto canvas = application.get_canvas(application.get_main_canvas_id());

    bool result = true;

    file_name = file_name + "." + extension;
    if (extension == "pdf") {
        result = canvas->print_pdf(file_name.c_str(), initial_world.width(), initial_world.height());
    } else if (extension == "png") {
        constexpr int IMAGE_WIDTH_PIXELS = 2048;
        int image_height_pixels = IMAGE_WIDTH_PIXELS * float(initial_world.height()) / initial_world.width();
        result = canvas->print_png(file_name.c_str(), IMAGE_WIDTH_PIXELS, image_height_pixels);
    } else if (extension == "svg") {
        result = canvas->print_svg(file_name.c_str(), initial_world.width(), initial_world.height());
    } else {
        warning_dialog_box("Invalid file type");
    }

    VTR_ASSERT_MSG(result == true, "Failed to save graphics");
}

void save_graphics_dialog_box(QWidget* /*widget*/, ezgl::application* /*app*/) {
    QWidget* main_window;
    QWidget* content_area;
    QLineEdit* text_entry;
    QLabel* name_label;
    QLabel* type_label;
    QDialog* dialog;
    QComboBox* combo_box;

    // get a pointer to the main window
    main_window = application.find_widget(application.get_main_window_id().c_str());
    dialog = new QDialog(main_window);
    dialog->setWindowTitle("Save Graphics Contents");
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    // create elements
    name_label = new QLabel("File name:");
    text_entry = new QLineEdit;
    type_label = new QLabel("File format:");
    combo_box = new QComboBox;

    // set name for text entry and combo box for later data extraction
    text_entry->setObjectName("file_name_text_entry");
    combo_box->setObjectName("file_name_combo_box");

    combo_box->addItem("pdf"); // index 0
    combo_box->addItem("png"); // index 1
    combo_box->addItem("svg"); // index 2

    // set default values
    combo_box->setCurrentIndex(0);        // default set to pdf which has an index 0
    text_entry->setText("vpr_graphics");  // default text set to vpr_graphics

    // attach elements to the content area of the dialog
    content_area = gtk_dialog_get_content_area(dialog);
    gtk_container_add(GTK_CONTAINER(content_area), name_label);
    gtk_container_add(GTK_CONTAINER(content_area), text_entry);
    gtk_container_add(GTK_CONTAINER(content_area), type_label);
    gtk_container_add(GTK_CONTAINER(content_area), combo_box);

    // show the label & child widget of the dialog
    dialog->show();

    auto* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel, dialog);
    gtk_container_add(dialog, buttonBox);
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, dialog, [dialog]() {
        save_graphics_from_button(dialog, 0, dialog);
        dialog->accept();
    });
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
    return;
}

#endif /* NO_GRAPHICS */
