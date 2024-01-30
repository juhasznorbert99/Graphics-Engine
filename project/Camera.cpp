#include "Camera.hpp"
using namespace std;
namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        pitchAngle = 0.0f;
        yawAngle = 0.0f;

        //TODO - Update the rest of camera parameters
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::cross(this->cameraFrontDirection, this->cameraUpDirection);
    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    glm::vec3 Camera::getPositionVec() {
        return this->cameraPosition;
    }
    glm::vec3 Camera::getCameraFrontDirection() {
        return this->cameraFrontDirection;
    }
    glm::vec3 Camera::getcameraRightDirection() {
        return this->cameraRightDirection;
    }
    glm::vec3 Camera::getCameraTarget() {
        return this->cameraTarget;
    }

    void Camera::setCamera(glm::vec3 cameraP, glm::vec3 cameraT, glm::vec3 cameraU) {
        this->cameraTarget = cameraT;
        this->cameraPosition = cameraP;
        this->cameraUpDirection = cameraU;

        pitchAngle = 0.0f;
        yawAngle = 0.0f;

        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::cross(this->cameraFrontDirection, this->cameraUpDirection);
    }
    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        switch (direction)
        {
            case MOVE_FORWARD:
                this->cameraPosition += this->cameraFrontDirection * speed;
                this->cameraTarget += this->cameraFrontDirection * speed;
                break;
            case MOVE_BACKWARD:
                this->cameraPosition -= this->cameraFrontDirection * speed;
                this->cameraTarget -= this->cameraFrontDirection * speed;
                break;
            case MOVE_LEFT:
                this->cameraPosition -= this->cameraRightDirection * speed;
                this->cameraTarget -= this->cameraRightDirection * speed;
                break;
            case MOVE_RIGHT:
                this->cameraPosition += this->cameraRightDirection * speed;
                this->cameraTarget += this->cameraRightDirection * speed;
                break;
            case MOVE_UP:
                this->cameraPosition += this->cameraUpDirection * speed;
                this->cameraTarget += this->cameraUpDirection * speed;
                break;
            case MOVE_DOWN:
                this->cameraPosition -= this->cameraUpDirection * speed;
                this->cameraTarget -= this->cameraUpDirection * speed;
                break;
        }
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
    }
    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        pitchAngle += pitch;
        yawAngle += yaw;


        this->cameraTarget.z = this->cameraPosition.z - cos(yawAngle);
        this->cameraTarget.x = this->cameraPosition.x + sin(yawAngle);


        this->cameraTarget.y = this->cameraPosition.y + sin(pitchAngle);
        this->cameraFrontDirection = glm::normalize(this->cameraTarget - this->cameraPosition);
        this->cameraRightDirection = glm::cross(this->cameraFrontDirection, this->cameraUpDirection);
    }
}