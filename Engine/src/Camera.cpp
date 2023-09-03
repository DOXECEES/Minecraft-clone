
#include "Camera.hpp"


Camera::Camera(glm::vec3 pos)
    :position(pos), front(0.0f,0.0f,-1.0f), upVec(0.0f,1.0f,0.0f)  
{
    glm::mat4 view = glm::lookAt(position,position + front,upVec); 

}


void Camera::SetPosition(glm::vec3 pos)
{
    position = pos;
}

