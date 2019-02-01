#include <instances\DirectionalLightInstance.hpp>

using namespace app;

DirectionalLightInstance::DirectionalLightInstance(const glm::vec3& direction, float intensity, const glm::vec3& color)
    : light(glm::vec3(0), intensity, color), transformation()
{
    glm::vec3 normalizedDir = glm::normalize(direction);

    glm::vec3 orientation;
    orientation.x = asin(normalizedDir.x);
    orientation.y = asin(normalizedDir.y);
    orientation.z = asin(normalizedDir.z);

    this->transformation.setOrientation(orientation);
    this->updateLightProperties();
}

qc::render::light::DirectionalLight& DirectionalLightInstance::getLight()
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
void DirectionalLightInstance::updateLightProperties()
{
    const auto& orientation = this->transformation.getOrientation();
    glm::vec3 dir;
    dir.x = asin(orientation.x);
    dir.y = asin(orientation.y);
    dir.z = asin(orientation.z);

    this->light.setDirection(dir);
}