
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>


class Camera 
{
    public:
        Camera(glm::vec3 pos);
        ~Camera();

        void SetPosition(glm::vec3 pos);
        //void SetFOV(float fov);



        glm::vec3 GetPosition() const { return position; };
        glm::vec3 GetFront() const { return front; };


    private:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 upVec;

};