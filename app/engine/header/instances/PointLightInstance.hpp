#pragma once

#include <qc_graphic\render\lights\PointLight.hpp>
#include <qc_graphic\Transform.hpp>

namespace app
{

class PointLightInstance
{
public:
    PointLightInstance(const glm::vec3& position = glm::vec3(0), float intensity = 1, const glm::vec3& color = glm::vec3(1));

    qc::render::light::PointLight& getLight();

    const qc::Transform& getTransform() const
    {
        return this->transformation;
    }

    qc::Transform& getTransform()
    {
        return this->transformation;
    }

private:
    qc::render::light::PointLight   light;
    qc::Transform                   transformation;

    void updateLightProperties();
};


}   //! namespace app