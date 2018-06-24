#pragma once

#include <qc_graphic/Geometry/Transformation.hpp>
#include <qc_graphic/Render/Shapes/Shape.hpp>

namespace qc_graphic
{

namespace render
{

class Object3d
{
public:
    Object3d()
        : transformation()
    {

    }
    
    Object3d(const render::shape::Shape * s)
        : transformation()
    {
        assert(s != nullptr);
        shape = s;
    }

    geometry::Transformation& getTransfomation()
    {
        return transformation;
    }

    const render::shape::Shape* getShape()
    {
        return shape;
    }

    void setTransformation(const geometry::Transformation& t)
    {
        transformation = t;
    }

    void setShape(const render::shape::Shape* s)
    {
        shape = s;
    }


private:
    geometry::Transformation        transformation;
    const render::shape::Shape*     shape;

    //Materiaux
};


} // render


} // qc_graphic