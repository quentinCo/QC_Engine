#pragma once

#include <qc_graphic\render\lights\DirectionalLight.hpp>
#include <qc_graphic\Transform.hpp>

namespace app
{

class DirectionalLightInstance
{
public:
    DirectionalLightInstance(const glm::vec3& direction = glm::vec3(-1,0,0), float intensity = 1, const glm::vec3& color = glm::vec3(1));

    qc::render::light::DirectionalLight& getLight();

    const qc::Transform& getTransform() const
    {
        return this->transformation;
    }

    qc::Transform& getTransform()
    {
        return this->transformation;
    }

private:
    qc::render::light::DirectionalLight   light;
    qc::Transform                   transformation;

    void updateLightProperties();
};


}   //! namespace app