#ifndef GUI_INPUT_H
#define GUI_INPUT_H

#include <queue>
#include <guichan/input.hpp>
#include <guichan/keyinput.hpp>
#include <guichan/mouseinput.hpp>
#include <guichan/platform.hpp>
#include <SFML/Window/Event.hpp>

#include "types.h"


class GuiInput : public gcn::Input
{
private:
    std::queue<gcn::KeyInput> key_q;
    std::queue<gcn::MouseInput> mouse_q;

    unsigned int x, y;

public:
    GuiInput();
    void event(const sf::Event &evt);

    bool isKeyQueueEmpty();
    gcn::KeyInput dequeueKeyInput();
    bool isMouseQueueEmpty();
    gcn::MouseInput dequeueMouseInput();
    void _pollInput(){}

private:
    int convertKey(sf::Key::Code key);
};


#endif /* GUI_INPUT_H */

