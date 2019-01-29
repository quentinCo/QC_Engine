#pragma once

#include <glm\vec3.hpp>
#include <glm\matrix.hpp>

namespace qc
{

namespace render
{

class Camera
{
public:
    // Constructors
    Camera(const glm::vec3& pos = glm::vec3(0,0,0), float fov = 70, float nearPlan = 0.1, float far = 10000, const glm::vec3& vFront = glm::vec3(0,0,-1));
    Camera(const glm::vec3& pos, const glm::vec3& target, float fov = 70, float nearPlan = 0.1, float far = 10000);

    ~Camera() {}

    // Getters / Setters
    const glm::vec3& getPosition();
    void setPosition(const glm::vec3& pos);
    
    const glm::vec3& getFrontVector();
    
    const glm::vec3& getRightVector();
    
    const glm::vec3& getUpVector();
    
    const glm::vec3& getAngles();
    void setAngles(const glm::vec3& angles);
    
    const glm::mat4& getViewMatrix();
    
    float getNear();
    void setNear(float nearPlan);
    float getFar();
    void setFar(float far);
    float getFov();
    void setFov(float tov);

    const glm::mat4& getProjMatrix(const glm::ivec2& viewportSize);
    
private:
    // Camera properties
    glm::vec3 position;
    
    glm::vec3 frontVector;
    glm::vec3 rightVector;
    glm::vec3 upVector;

    glm::vec3 angles;           // Radian

    glm::mat4 viewMatrix;
 
    float nearPlan;
    float farPlan;
    float fov;

    // Camera properties depend on screen properties
    glm::mat4   projMatrix;
    glm::ivec2  viewportSize;

    // Event
    bool isValidVector;
    bool isValidViewMatrix;
    bool isValidProjMatrix;


    // Functions
    void computeAngles();
    void computeVectors();
    void computeViewMatrix();
    void computeProjMatrix(const glm::ivec2& viewportSize);

};

}; //! namespace render

}; //! namespace qc