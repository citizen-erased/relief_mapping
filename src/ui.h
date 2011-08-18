#ifndef UI_H
#define UI_H

#include <guichan.hpp>
#include <guichan/opengl.hpp>
#include "types.h"

class GuiInput;
class GuiFont;
class GuiImageLoader;
class Physics;
class DebugDrawOptions;

template<class T> class GenericListModel : public gcn::ListModel
{
public:
    std::vector<T> data;

public:
    std::string getElementAt(int i)
    {
        return boost::lexical_cast<std::string>(data[i]);
    }

    int getNumberOfElements()
    {
        return data.size();
    }
};

class UI : public gcn::ActionListener
{
private:
    RenderWindowPtr window;

    gcn::Gui                        *gui;
    GuiInput                        *input;
    gcn::OpenGLGraphics             *graphics;
    GuiImageLoader                  *image_loader;

    gcn::Container                  *top;

    GuiFont                         *font_proggy;

    
    gcn::ListBox                    *materials;
    GenericListModel<std::string>   materials_model;
    gcn::Slider                     *eta_r, *eta_g, *eta_b;


public:
    RenderPtr render;

public:
    UI(RenderWindowPtr window);

    void setSize(int w, int h);

    void tick();
    void draw();
    void event(const sf::Event &evt);

    void action(const gcn::ActionEvent &evt);
};

#endif /* UI_H */
