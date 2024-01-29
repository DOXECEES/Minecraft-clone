
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "EventsHandler/Mouse.hpp"

class Camera
{
public:
    Camera(glm::vec3 pos);
    ~Camera() = default;

    void Update(const glm::vec3 pos, const float yaw, const float pitch);
    void SetPosition(glm::vec3 pos);
    // void SetFOV(float fov);

    inline glm::vec3 GetPosition() const { return position; };
    inline glm::vec3 GetFront() const { return front; };
    inline glm::vec3 GetDirection() const { return direction; };
    inline glm::mat4 GetViewMatrix() const { return view; }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 upVec;

    glm::mat4 view;

    glm::vec3 direction;
};