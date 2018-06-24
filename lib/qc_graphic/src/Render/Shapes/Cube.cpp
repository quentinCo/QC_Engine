#include <vector>

#include <qc_graphic/Render/Shapes/Cube.hpp>

using namespace qc_graphic::render::shape;

Cube::Cube()
{
    std::vector<geometry::Vertex>   vertices;
    std::vector<int>                indices;
    
    std::vector<glm::vec3>  points;
    
    int nbPoints    = 8;
    int nbFace      = 8;
    int angleSubDiv = 4;
    float angleFac  = 1.f / angleSubDiv;

    float x;
    float y;
    float z;
    
    for (int i = 0; i < angleSubDiv; ++i)
    {
        float cosA = cos(i * angleFac * 2 * 3.14);
        float sinA = sin(i * angleFac * 2 * 3.14);

        for (int j = 0; j < angleSubDiv; ++j)
        {
            float cosB = cos(j * angleFac * 2 * 3.14);
            float sinB = sin(j * angleFac * 2 * 3.14);

            x = 0.5 * cosA * cosB;
            y = 0.5 * sinA;
            z = 0.5 * cosB * sinB;

            points.emplace_back(x, y, z);
        }
    }



        //points.push_back()
    

}