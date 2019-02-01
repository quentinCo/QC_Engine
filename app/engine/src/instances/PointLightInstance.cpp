#include <instances\PointLightInstance.hpp>

using namespace app;

PointLightInstance::PointLightInstance(const glm::vec3& position, float intensity, const glm::vec3& color)
    : light(position, intensity, color), transformation(position)
{}

qc::render::light::PointLight& PointLightInstance::getLight()
{
    if (this->transformation.isValid() == false)
        this->updateLightProperties();

    return this->light;
}


/*
 *
 *
 *
 *
 *
 */


 // PRIVATE ----------------------------------------------------------------------------------------------------------------------------
 // ------------------------------------------------------------------------------------------------------------------------------------
void PointLightInstance::updateLightProperties()
{
    this->light.setPosition(this->transformation.getPosition());
}