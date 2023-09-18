#include "gui.hpp"


MargolusGUI::MargolusGUI(const std::string& guiFile)
    : builder(Gtk::Builder::create_from_file(guiFile)),
      window(nullptr),
      glArea(nullptr) {

    builder->get_widget("window", window);
    builder->get_widget("gl_area", glArea);
    if (!glArea)
        throw std::runtime_error("Could not find gl_area in GUI file");

    glArea->set_required_version(3, 3);
    glArea->set_double_buffered(true);

    safe_connect_signal(glArea, glArea->signal_render(), [this]() { this->onRenderGL(); });
}

void MargolusGUI::onRenderGL() {
    // Inside this function, you can perform OpenGL rendering.
    // Clear the background with a black color.
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Set up the model-view matrix and draw your graphics here.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw a red rectangle (example)
    glBegin(GL_QUADS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    glEnd();

    // Request a redraw
    glArea->queue_render();
}

void MargolusGUI::run() {
    Gtk::Main::run(*window);
}

void MargolusGUI::quit() {
    Gtk::Main::quit();
}
