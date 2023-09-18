#ifndef MARGOLUS_GUI_
#define MARGOLUS_GUI_

#include <string>
#include <stdexcept>

#include <gtkmm.h>
#include <GL/gl.h>
#include <sigc++-2.0/sigc++/sigc++.h>

#include "gui.tpp"


class MargolusGUI {
protected:
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Window* window;
    Gtk::GLArea* glArea;

    void onRenderGL();

public:
    MargolusGUI(const std::string& guiFile);
    void run();
    void quit();
};

#endif