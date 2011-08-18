#include "precompiled.h"
#include "gui_font.h"
#include "gui_input.h"
#include "material.h"
#include "render.h"
#include "ui.h"

UI::UI(RenderWindowPtr window)
{
    this->window = window;

    gui         = new gcn::Gui();
    graphics    = new gcn::OpenGLGraphics();
    input       = new GuiInput();

    font_proggy = new GuiFont(window, "data/proggy_clean_sz.ttf", 16);

    gui->setGraphics(graphics);
    gui->setInput(input);
    gcn::Widget::setGlobalFont(font_proggy);

    top = new gcn::Container();
    top->setOpaque(false);
    gui->setTop(top);


    eta_r = new gcn::Slider(0.0f, 1.0f);
    eta_g = new gcn::Slider(0.0f, 1.0f);
    eta_b = new gcn::Slider(0.0f, 1.0f);
    eta_r->addActionListener(this);
    eta_g->addActionListener(this);
    eta_b->addActionListener(this);
    top->add(eta_r);
    top->add(eta_g);
    top->add(eta_b);

    materials = new gcn::ListBox();
    materials = new gcn::ListBox(&materials_model);
    materials->addActionListener(this);
    top->add(materials);

    setSize(window->GetWidth(), window->GetHeight());
}

void UI::setSize(int w, int h)
{
    graphics->setTargetPlane(w, h);
    top->setDimension(gcn::Rectangle(0, 0, w, h));

    materials->setPosition(0.0f, 0.0f);
    materials->setSize(150.0f, 200.0f);

    float s_width = 100.0f;
    float s_height = 20.0f;
    eta_r->setSize(s_width, s_height);
    eta_g->setSize(s_width, s_height);
    eta_b->setSize(s_width, s_height);

    eta_r->setPosition(200.0f, 0.0f);
    eta_g->setPosition(200.0f, eta_r->getY() + eta_r->getHeight());
    eta_b->setPosition(200.0f, eta_g->getY() + eta_g->getHeight());
}

void UI::action(const gcn::ActionEvent &evt)
{
    if(evt.getSource() == eta_r || 
       evt.getSource() == eta_g || 
       evt.getSource() == eta_b)
    {
        int selected = materials->getSelected();
        if(selected >= 0)
        {
            TransmittanceMaterialPtr mat = render->transmittance_materials[materials_model.data[selected]];
            mat->eta_r = eta_r->getValue();
            mat->eta_g = eta_g->getValue();
            mat->eta_b = eta_b->getValue();
        }
    }
}

void UI::tick()
{
    materials_model.data.clear();

    for(std::map<std::string, TransmittanceMaterialPtr>::iterator it = render->transmittance_materials.begin(); it != render->transmittance_materials.end(); it++)
        materials_model.data.push_back(it->first);

    int selected = materials->getSelected();
    if(selected >= 0)
    {
        TransmittanceMaterialPtr mat = render->transmittance_materials[materials_model.data[selected]];
        eta_r->setValue(mat->eta_r);
        eta_g->setValue(mat->eta_g);
        eta_b->setValue(mat->eta_b);
    }


    gui->logic();
}

void UI::draw()
{
    gui->draw();
}

void UI::event(const sf::Event &evt)
{
    input->event(evt);
}

