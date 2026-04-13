#ifndef NO_GRAPHICS

#include "gtkcomboboxhelper.h"
#include "vpr_qtcompat.h"

/**
 * @brief Get the number of items in the combo box.
 * 
 * This function returns the number of items currently present in the combo box.
 * 
 * @param combo_box A pointer to the combo box widget.
 * @return The number of items in the combo box.
 */
static int get_items_count(void* combo_box) {
    QComboBox* combo = GTK_COMBO_BOX_TEXT(combo_box);
    int count = combo->count();
    return count;
}

/**
 * @brief Get the index of an item in a combo box by its text.
 * 
 * This function searches for an item with the specified text in the combo box
 * and returns its index if found.
 * 
 * @param combo_box A pointer to the combo box widget.
 * @param target_item The text of the item to search for.
 * @return The index of the item if found, or -1 if not found.
 */
int get_item_index_by_text(void* combo_box, const char* target_item) {
    int result_index = -1;
    QComboBox* combo = GTK_COMBO_BOX_TEXT(combo_box);
    result_index = combo->findText(target_item);
    return result_index;
}

#endif // NO_GRAPHICS
