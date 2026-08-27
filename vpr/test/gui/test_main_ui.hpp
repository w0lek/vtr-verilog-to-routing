#pragma once
/**
 * @brief Which main UI form the GUI tests load, and in which format.
 *
 * The suite is compiled twice: once against the Glade form and once against
 * the native Qt Designer form, so the same assertions verify that the two
 * describe the same window. Both are overridden together from CMake:
 *
 *   target_compile_definitions(<target> PRIVATE
 *       VPR_MAIN_UI_PATH="/ezgl/main.ui"
 *       VPR_MAIN_UI_FORMAT=ezgl::ui_format::qt)
 *
 * Path and format must stay in step -- a native form parsed by the Glade
 * loader (or the reverse) fails in ways that look like a broken form rather
 * than a mismatched pair, so prefer the helper below over hand-rolling the
 * MainWindow construction at each call site.
 */

#include <ezgl/main_window.hpp>

#include <QString>

#include <optional>

// Defaults mirror what VPR actually ships (draw.cpp), so the default build
// tests the production configuration. The second target overrides both to keep
// the Glade form covered until it is removed.
#ifndef VPR_MAIN_UI_PATH
#define VPR_MAIN_UI_PATH ":/ezgl/main.ui"
#endif

#ifndef VPR_MAIN_UI_FORMAT
#define VPR_MAIN_UI_FORMAT ezgl::ui_format::qt
#endif

namespace vpr_gui_test {

/// Load the main UI form this build targets.
inline ezgl::MainWindow load_main_ui() {
    return ezgl::MainWindow(QString::fromLatin1(VPR_MAIN_UI_PATH),
                            std::nullopt,
                            VPR_MAIN_UI_FORMAT);
}

/// Load an arbitrary path in this build's format. Used by the tests that
/// exercise the loader's failure paths, so those run against whichever
/// loader is under test rather than always the Glade one.
inline ezgl::MainWindow load_ui(const QString& path) {
    return ezgl::MainWindow(path, std::nullopt, VPR_MAIN_UI_FORMAT);
}

} // namespace vpr_gui_test
