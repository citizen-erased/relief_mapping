#ifndef CAMERA_H
#define CAMERA_H

#include <cml/cml.h>

/*----------------------------------------------------------------------*
 *                                                                      *
 *                             Base Camera                              *
 *                                                                      *
 *----------------------------------------------------------------------*/
class Camera
{
protected:
    cml::matrix44f_c transform, inv_transform;

public:
    Camera();

    const cml::matrix44f_c& getTransform() const;
    const cml::matrix44f_c& getInvTransform() const;

    virtual void tick(){}
    virtual void event(const sf::Event &evt){};
};


/*----------------------------------------------------------------------*
 *                                                                      *
 *                             Free Camera                              *
 *                                                                      *
 *----------------------------------------------------------------------*/
class FreeCamera : public Camera
{
private:
    cml::quaternionf_p quat;
    cml::vector3f      trans;

    cml::quaternionf_p vel;
    cml::vector3f      ang_vel;

public:
    FreeCamera();

    void tick();
    void event(const sf::Event &evt);
};

#endif /* CAMERA_H */

