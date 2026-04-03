[ ] - Proper render implementation (QPainter refactor + API redesign)
[ ] - stress test of new render impl, final performance test GTK vs Qt on complex graphic scene
[ ] - Remove VPR_QT macro and GTK code sections under the #else ... #endif, from now Qt is only single framework used in backend.
[ ] - Rename and clean function arguments or use QWidget:: API directly for gtk calls -> remove qt_compat.h layer completely
[ ] - Convert glade main.ui to Qt designed proper UI format and remove QGladeLoader as temporary solution.
[ ] - Full code comments coverage
