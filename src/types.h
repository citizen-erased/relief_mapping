#ifndef TYPES_H
#define TYPES_H

#include <boost/shared_ptr.hpp>

class Camera;
typedef boost::shared_ptr<Camera> CameraPtr;

class FreeCamera;
typedef boost::shared_ptr<FreeCamera> FreeCameraPtr;

class Model;
typedef boost::shared_ptr<Model> ModelPtr;

class Render;
typedef boost::shared_ptr<Render> RenderPtr;

class RenderObject;

class Shader;
typedef boost::shared_ptr<Shader> ShaderPtr;

class Material;
typedef boost::shared_ptr<Material> MaterialPtr;

class TransmittanceMaterial;
typedef boost::shared_ptr<TransmittanceMaterial> TransmittanceMaterialPtr;

class ReliefMaterial;
typedef boost::shared_ptr<ReliefMaterial> ReliefMaterialPtr;

class BumpMaterial;
typedef boost::shared_ptr<BumpMaterial> BumpMaterialPtr;

class UI;
typedef boost::shared_ptr<UI> UIPtr;

namespace sf
{
    class Event;
    class Font;
    class RenderWindow;
}

typedef boost::shared_ptr<sf::Font> FontPtr;
typedef boost::shared_ptr<sf::RenderWindow> RenderWindowPtr;


#endif /* TYPES_H */

