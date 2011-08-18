#include "precompiled.h"
#include <guichan/opengl.hpp>
#include "data_cache.h"
#include "gui_font.h"

GuiFont::GuiFont(RenderWindowPtr window, const std::string &filename, unsigned int char_size)
{
    this->char_size = char_size;
    this->window = window;
    font = DataCache::getFont(filename, char_size);

    sf::String s("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?!", *font.get(), char_size);
    height = s.GetRect().GetHeight();
}

int GuiFont::getWidth(const std::string& str) const
{
    sf::String s(str, *font.get(), char_size);
    return s.GetRect().GetWidth();
}

int GuiFont::getHeight() const
{
    //return font->GetCharacterSize();
    return height;
}

void GuiFont::drawString(gcn::Graphics* graphics, const std::string& str, int x, int y)
{
    gcn::OpenGLGraphics* gl_graphics = dynamic_cast<gcn::OpenGLGraphics*>(graphics);

    if(gl_graphics == NULL)
        return;

    const gcn::ClipRectangle& top = gl_graphics->getCurrentClipArea();
    int rx = x + top.xOffset;
    int ry = y + top.yOffset;

    sf::String text(str, *font.get(), char_size);
    text.SetPosition(rx, ry);

    gcn::Color c = gl_graphics->getColor();
    text.SetColor(sf::Color(c.r, c.g, c.b, c.a));

    //guichan opengl graphics sets the texture environment to replace
    //so set it back to modulate to draw the string.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    window->Draw(text);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

