#include "precompiled.h"
#include "types.h"
#include "draw.h"
#include "camera.h"
#include "data_cache.h"
#include "render.h"
#include "ui.h"

RenderWindowPtr window;
sf::View window_view;
FreeCameraPtr cam;

bool running    = true;
bool ticking    = true;
bool step       = false;
bool draw_help  = false;
bool draw_ui    = false;
bool recording  = false;

size_t active_object = 0;
int ticks_per_second = 0;

UIPtr gui;
RenderPtr render;

void init();
void run();
void tick();
void draw();
void resized(int width, int height);
void drawHelp();
void processEvents();
void keyPressed(const sf::Event &evt);
void keyReleased(const sf::Event &evt);
void screenshot(const std::string &dir);
bool checkGLerror();



int main(int argc, char* argv[])
{
    init();
    run();

    return 0;
}

void init()
{
    window = RenderWindowPtr(new sf::RenderWindow());
    window->Create(sf::VideoMode(1024, 768), "lab1");
    window->PreserveOpenGLStates(true);
    window->SetFramerateLimit(60);
    window->SetView(window_view);

    GLenum err = glewInit();
    if(GLEW_OK != err)
        fatal(format("GLEW: %1%\n") % glewGetErrorString(err));
    print(format("using GLEW version %1%\n") % glewGetString(GLEW_VERSION));

    print(format("OpenGL 2.0 supported: %1%\n") % (GLEW_VERSION_2_0 ? "yes" : "no"));
    print(format("OpenGL 2.1 supported: %1%\n") % (GLEW_VERSION_2_1 ? "yes" : "no"));
    print(format("OpenGL 3.0 supported: %1%\n") % (GLEW_VERSION_3_0 ? "yes" : "no"));
    print(format("OpenGL 3.1 supported: %1%\n") % (GLEW_VERSION_3_1 ? "yes" : "no"));
    print(format("OpenGL 3.2 supported: %1%\n") % (GLEW_VERSION_3_2 ? "yes" : "no"));
    print(format("OpenGL 3.3 supported: %1%\n") % (GLEW_VERSION_3_3 ? "yes" : "no"));
    print(format("OpenGL 4.0 supported: %1%\n") % (GLEW_VERSION_4_0 ? "yes" : "no"));
    print(format("OpenGL 4.1 supported: %1%\n") % (GLEW_VERSION_4_1 ? "yes" : "no"));

    GLint max_tex_units;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_tex_units);
    print(format("Available texture units: %1%\n") % max_tex_units);

    cam = FreeCameraPtr(new FreeCamera());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT0);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    render = RenderPtr(new Render());
    render->camera = cam;

    gui = UIPtr(new UI(window));
    gui->render = render;

    resized(1024, 768);
}

void run()
{
    sf::Clock wall_timer;
    int ticks = 0;

    while(running)
    {
        processEvents();
        tick();
        draw();

        if(wall_timer.GetElapsedTime() > 1.0f)
        {
            print(format("ups=%1%\n") % ticks);
            ticks_per_second = ticks;
            ticks = 0;
            wall_timer.Reset();
        }

        ticks++;
    }
}

void tick()
{
    if(ticking || step)
    {
        render->tick();
        step = false;
    }

    if(draw_ui) gui->tick();
    cam->tick();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(cam->getInvTransform().data());
    
    float p[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, p);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    render->draw();
    glPopAttrib();

    glUseProgram(0);

    if(draw_ui) gui->draw();
    if(draw_help) drawHelp();

    std::string str = boost::str(
        format("%1%fps\nactive object = %2%\nrelief map depth = %3%") % 
        ticks_per_second % 
        active_object %
        render->objects[active_object].relief_scale);

    window->Draw(sf::String(str, *DataCache::getFont("data/proggy_clean_sz.ttf", 32).get(), 16.0f));

    if(recording) screenshot("frames");
    window->Display();
    checkGLerror();
}

bool checkGLerror()
{
    GLenum err = 0;
    bool errors = false;

    while((err = glGetError()) != GL_NO_ERROR)
    {
        warning(format("OpenGL Error: %1%\n") % gluErrorString(err));
        errors = true;
    }

    return errors;
}


void resized(int width, int height)
{
    int w = width < 1 ? 1 : width;
    int h = height < 1 ? 1 : height;

    float ratio = (float)w/(float)h;
    GLint mode;
    glGetIntegerv(GL_MATRIX_MODE, &mode);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 1024.0f);
    glViewport(0, 0, w, h);

    glMatrixMode(mode);

    window_view.SetFromRect(sf::FloatRect(0.0f, 0.0f, (float)w, (float)h));
    gui->setSize(w, h);
}

void drawHelp()
{
    std::string str = 
    "w,s,a,d     - move camera\n"
    "pageup/down - camera up/down\n"
    "q,e         - roll camera\n"
    "arrow keys  - pitch/yaw camera\n\n"

    "t           - toggle logic updating\n"
    "y           - step logic once when logic updating is off\n"
    "g           - toggle user interface\n"
    "escape      - exit\n\n";

    sf::String txt(str, *DataCache::getFont("data/proggy_clean_sz.ttf", 32).get(), 16.0f);
    window->Draw(txt);
}

void screenshot()
{
    sf::Image Screen = window->Capture();
    Screen.SaveToFile("screenshot.png");
}

void screenshot(const std::string &dir)
{
    static int screen_num = 0;

    std::stringstream ss, ss2;
    ss << screen_num++;

    int zeros = 6 - ss.str().length();
    for(int i = 0; i < zeros; i++)
        ss2 << "0";
    ss2 << ss.str();

    sf::Image img = window->Capture();
    img.SaveToFile(dir + "/" + ss2.str() + (dir == "screenshots" ? ".png" : ".jpg"));
}

void processEvents()
{
    sf::Event evt;

    while(window->GetEvent(evt))
    {
        switch(evt.Type)
        {
            case sf::Event::Closed:      running = false;                          break;
            case sf::Event::Resized:     resized(evt.Size.Width, evt.Size.Height); break;
            case sf::Event::KeyPressed:  keyPressed(evt);                          break;
            case sf::Event::KeyReleased: keyReleased(evt);                         break;
        }

        if(draw_ui) gui->event(evt);
        cam->event(evt);
    }
}

void keyPressed(const sf::Event &evt)
{
    switch(evt.Key.Code)
    {
        case sf::Key::Escape:   running = false;        break;
        case sf::Key::T:        ticking =! ticking;     break;
        case sf::Key::Y:        step = true;            break;
        case sf::Key::H:        draw_help =! draw_help; break;
        case sf::Key::G:        draw_ui =! draw_ui;     break;
        case sf::Key::F1:       screenshot("screenshots"); break;
        case sf::Key::F2:       recording = !recording; break;

        case sf::Key::Num0:     render->debug_draw = !render->debug_draw; break;
        case sf::Key::Num9:     render->draw_teapot = !render->draw_teapot; break;

        case sf::Key::Num1:     active_object = 0; break;
        case sf::Key::Num2:     active_object = 1; break;
        case sf::Key::Num3:     active_object = 2; break;
        case sf::Key::Num4:     active_object = 3; break;
        case sf::Key::Num5:     active_object = 4; break;

        case sf::Key::M: render->objects[active_object].relief_scale += 0.01f; break;
        case sf::Key::N: render->objects[active_object].relief_scale = std::max(render->objects[active_object].relief_scale - 0.01f, 0.0f); break;
        case sf::Key::V: render->objects[active_object].visible = !render->objects[active_object].visible; break;
    }
}

void keyReleased(const sf::Event &evt)
{
    switch(evt.Key.Code)
    {
    }
}

