#pragma once

#include <qc_graphic\Transform.hpp>
#include <qc_graphic\render\Mesh.hpp>

namespace app
{

class Object3d
{
public:
    Object3d(const qc::render::Mesh* mesh = nullptr)
        : mesh(mesh)
    {}

    qc::Transform& getTransformation()
    {
        return this->transformation;
    }

    const qc::Transform& getTransformation() const
    {
        return this->transformation;
    }

    void setTransformation(const qc::Transform& transform)
    {
        this->transformation = transform;
    }

    const qc::render::Mesh* getMesh() const
    {
        return this->mesh;
    }

    void setMesh(const qc::render::Mesh* mesh)
    {
        this->mesh = mesh;
    }

private:
    const qc::render::Mesh* mesh;
    qc::Transform           transformation;
};

}   //! namespace app

