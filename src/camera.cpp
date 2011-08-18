#include "precompiled.h"
#include "camera.h"

/*----------------------------------------------------------------------*
 *                                                                      *
 *                             Base Camera                              *
 *                                                                      *
 *----------------------------------------------------------------------*/
Camera::Camera()
{
    transform.identity();
}

const cml::matrix44f_c& Camera::getTransform() const
{
    return transform;
}

const cml::matrix44f_c& Camera::getInvTransform() const
{
    return inv_transform;
}




/*----------------------------------------------------------------------*
 *                                                                      *
 *                             Free Camera                              *
 *                                                                      *
 *----------------------------------------------------------------------*/
FreeCamera::FreeCamera()
{
    transform.identity();
    inv_transform.identity();
    quat.identity();
    vel[0] = 0.0f; vel[1] = 0.0f; vel[2] = 0.0f; vel[3] = 0.0f;
    trans.set(0.0f, 0.0f, 10.0f);
    ang_vel.set(0.0f, 0.0f, 0.0f);
}

void FreeCamera::tick()
{
    cml::quaternionf_p rot;
    cml::quaternion_rotation_euler(rot, ang_vel[0], ang_vel[1], ang_vel[2], cml::euler_order_xyz);
    quat *= rot;
    quat.normalize();

    trans += (quat * vel * cml::conjugate(quat)).imaginary();

    transform.identity();
    cml::matrix_rotation_quaternion(transform, quat);
    cml::matrix_set_translation(transform, trans);

    inv_transform = cml::inverse(transform);
}

void FreeCamera::event(const sf::Event &evt)
{
    switch(evt.Type)
    {
        case sf::Event::KeyPressed:
            switch(evt.Key.Code)
            {
                case sf::Key::W: vel[2] = -1.0f; break;
                case sf::Key::S: vel[2] =  1.0f; break;
                case sf::Key::A: vel[0] = -1.0f; break;
                case sf::Key::D: vel[0] =  1.0f; break;
                case sf::Key::PageUp:   vel[1] =  1.0f; break;
                case sf::Key::PageDown: vel[1] = -1.0f; break;

                case sf::Key::Up:    ang_vel[0] = cml::rad( 1.0f); break;
                case sf::Key::Down:  ang_vel[0] = cml::rad(-1.0f); break;
                case sf::Key::Left:  ang_vel[1] = cml::rad( 1.0f); break;
                case sf::Key::Right: ang_vel[1] = cml::rad(-1.0f); break;
                case sf::Key::Q:     ang_vel[2] = cml::rad( 1.0f); break;
                case sf::Key::E:     ang_vel[2] = cml::rad(-1.0f); break;

                case sf::Key::R:
                {
                    quat.identity();
                }
                break;
            }
        break;

        case sf::Event::KeyReleased:
            switch(evt.Key.Code)
            {
                case sf::Key::W: vel[2] = 0.0f; break;
                case sf::Key::S: vel[2] = 0.0f; break;
                case sf::Key::A: vel[0] = 0.0f; break;
                case sf::Key::D: vel[0] = 0.0f; break;
                case sf::Key::PageUp:   vel[1] = 0.0f; break;
                case sf::Key::PageDown: vel[1] = 0.0f; break;

                case sf::Key::Up:    ang_vel[0] = 0.0f; break;
                case sf::Key::Down:  ang_vel[0] = 0.0f; break;
                case sf::Key::Left:  ang_vel[1] = 0.0f; break;
                case sf::Key::Right: ang_vel[1] = 0.0f; break;
                case sf::Key::Q:     ang_vel[2] = 0.0f; break;
                case sf::Key::E:     ang_vel[2] = 0.0f; break;
            }
        break;
    }
}

