#pragma once

#include <qc_graphic/Geometry/Transformation.hpp>
#include <qc_graphic/Geometry/Shapes/Shape.hpp>

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
    
    Object3d(const geometry::shape::Shape * s)
        : transformation()
    {
        assert(s != nullptr);
        shape = s;
    }

    geometry::Transformation& getTransfomation()
    {
        return transformation;
    }

    const geometry::shape::Shape* getShape()
    {
        return shape;
    }

    void setTransformation(const geometry::Transformation& t)
    {
        transformation = t;
    }

    void setShape(const geometry::shape::Shape* s)
    {
        shape = s;
    }


private:
    geometry::Transformation        transformation;
    const geometry::shape::Shape*   shape;

    //Materiaux
};


} // render


} // qc_graphic