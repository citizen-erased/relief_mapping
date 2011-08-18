#include "precompiled.h"
#include "gui_input.h"

GuiInput::GuiInput()
{
    x = y = 0;
}

void GuiInput::event(const sf::Event &evt)
{
    if(evt.Type == sf::Event::KeyPressed || evt.Type == sf::Event::KeyReleased)
    {
        int key = convertKey(evt.Key.Code);

        if(key == -1)
            return;

        gcn::KeyInput k;
        k.setAltPressed(evt.Key.Alt);
        k.setShiftPressed(evt.Key.Shift);
        k.setControlPressed(evt.Key.Control);
        k.setMetaPressed(false);
        k.setType(evt.Type == sf::Event::KeyPressed ? gcn::KeyInput::PRESSED : gcn::KeyInput::RELEASED);
        k.setNumericPad(false);
        k.setKey(key);
        key_q.push(k);
    }
    else if(evt.Type == sf::Event::TextEntered)
    {
        gcn::KeyInput k;
        k.setAltPressed(evt.Key.Alt);
        k.setShiftPressed(evt.Key.Shift);
        k.setControlPressed(evt.Key.Control);
        k.setMetaPressed(false);
        k.setType(gcn::KeyInput::PRESSED);
        k.setNumericPad(false);
        k.setKey(evt.Text.Unicode);
        key_q.push(k);
    }
    else if(evt.Type == sf::Event::MouseButtonPressed || evt.Type == sf::Event::MouseButtonReleased)
    {
        unsigned int type = (evt.Type == sf::Event::MouseButtonPressed) ? gcn::MouseInput::PRESSED : gcn::MouseInput::RELEASED;
        unsigned int button = gcn::MouseInput::EMPTY;
        switch(evt.MouseButton.Button)
        {
            case sf::Mouse::Left:   button = gcn::MouseInput::LEFT;   break;
            case sf::Mouse::Right:  button = gcn::MouseInput::RIGHT;  break;
            case sf::Mouse::Middle: button = gcn::MouseInput::MIDDLE; break;
            default: break;
        }

        gcn::MouseInput m(button, type, x, y, 0);
        mouse_q.push(m);
    }
    else if(evt.Type == sf::Event::MouseMoved)
    {
        x = evt.MouseMove.X;
        y = evt.MouseMove.Y;
        gcn::MouseInput m(gcn::MouseInput::EMPTY, gcn::MouseInput::MOVED, x, y, 0);
        mouse_q.push(m);
    }
}

int GuiInput::convertKey(sf::Key::Code key)
{
    switch(key)
    {
        case sf::Key::Space:    return gcn::Key::SPACE;         break;
        case sf::Key::Tab:      return gcn::Key::TAB;           break;
        case sf::Key::Return:   return gcn::Key::ENTER;         break;
        case sf::Key::LAlt:     return gcn::Key::LEFT_ALT;      break;
        case sf::Key::RAlt:     return gcn::Key::RIGHT_ALT;     break;
        case sf::Key::LShift:   return gcn::Key::LEFT_SHIFT;    break;
        case sf::Key::RShift:   return gcn::Key::RIGHT_SHIFT;   break;
        case sf::Key::LControl: return gcn::Key::LEFT_CONTROL;  break;
        case sf::Key::RControl: return gcn::Key::RIGHT_CONTROL; break;
        case sf::Key::LSystem:  return gcn::Key::LEFT_SUPER;    break;
        case sf::Key::RSystem:  return gcn::Key::RIGHT_SUPER;   break;
        case sf::Key::Insert:   return gcn::Key::INSERT;        break;
        case sf::Key::Home:     return gcn::Key::HOME;          break;
        case sf::Key::PageUp:   return gcn::Key::PAGE_UP;       break;
        case sf::Key::Delete:   return gcn::Key::DELETE;        break;
        case sf::Key::End:      return gcn::Key::END;           break;
        case sf::Key::PageDown: return gcn::Key::PAGE_DOWN;     break;
        case sf::Key::Escape:   return gcn::Key::ESCAPE;        break;
        case sf::Key::Back:     return gcn::Key::BACKSPACE;     break;
        case sf::Key::F1:       return gcn::Key::F1;            break;
        case sf::Key::F2:       return gcn::Key::F2;            break;
        case sf::Key::F3:       return gcn::Key::F3;            break;
        case sf::Key::F4:       return gcn::Key::F4;            break;
        case sf::Key::F5:       return gcn::Key::F5;            break;
        case sf::Key::F6:       return gcn::Key::F6;            break;
        case sf::Key::F7:       return gcn::Key::F7;            break;
        case sf::Key::F8:       return gcn::Key::F8;            break;
        case sf::Key::F9:       return gcn::Key::F9;            break;
        case sf::Key::F10:      return gcn::Key::F10;           break;
        case sf::Key::F11:      return gcn::Key::F11;           break;
        case sf::Key::F12:      return gcn::Key::F12;           break;
        case sf::Key::F13:      return gcn::Key::F13;           break;
        case sf::Key::F14:      return gcn::Key::F14;           break;
        case sf::Key::F15:      return gcn::Key::F15;           break;
        case sf::Key::Pause:    return gcn::Key::PAUSE;         break;
        case sf::Key::Left:     return gcn::Key::LEFT;          break;
        case sf::Key::Right:    return gcn::Key::RIGHT;         break;
        case sf::Key::Up:       return gcn::Key::UP;            break;
        case sf::Key::Down:     return gcn::Key::DOWN;          break;
        default: return -1;
    }
    return -1;
}

bool GuiInput::isKeyQueueEmpty()
{
    return key_q.empty();
}

gcn::KeyInput GuiInput::dequeueKeyInput()
{
    if(key_q.empty())
        return gcn::KeyInput(gcn::Key::SPACE, gcn::KeyInput::RELEASED);

    gcn::KeyInput k = key_q.front();
    key_q.pop();
    return k;
}

bool GuiInput::isMouseQueueEmpty()
{
    return mouse_q.empty();
}

gcn::MouseInput GuiInput::dequeueMouseInput()
{
    if(mouse_q.empty())
        return gcn::MouseInput(gcn::MouseInput::EMPTY, gcn::MouseInput::RELEASED, 0, 0, 0);

    gcn::MouseInput m = mouse_q.front();
    mouse_q.pop();
    return m;
}

