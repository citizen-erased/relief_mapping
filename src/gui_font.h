#ifndef GUI_FONT_H
#define GUI_FONT_H

#include <string>
#include <guichan/font.hpp>
#include "types.h"

class GuiFont : public gcn::Font
{
private:
    FontPtr font;
    unsigned int char_size;
    int height;
    RenderWindowPtr window;

public:
    GuiFont(RenderWindowPtr window, const std::string &filename, unsigned int char_size);

    int getWidth(const std::string& str) const;
    int getHeight() const;
    void drawString(gcn::Graphics* graphics, const std::string& str, int x, int y);
};

#endif /* GUI_FONT_H */

