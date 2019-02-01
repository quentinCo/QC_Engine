#pragma once

#include <glm\vec3.hpp>

namespace qc
{

namespace render
{

namespace light
{

class Light
{
public:
    Light() = default;
    virtual ~Light() = default;

    virtual float getIntensity() const = 0;
    virtual void setIntensity(float intensity) = 0;

    virtual const glm::vec4& getColor() const = 0;
    virtual void setColor(const glm::vec3& color) = 0;
};

}   //! namespace light

}   //! namespace render

}   //! namespace qc