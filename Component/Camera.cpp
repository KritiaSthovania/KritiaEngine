#include "Camera.h"
#include "../CoreModule/Input.h"
#include "../CoreModule/Time.h"
#include "../CoreModule/Debug.h"
#include "../CoreModule/Mathf.h"
using namespace KritiaEngine;
using namespace glm;

std::shared_ptr<Camera> Camera::current = nullptr;
std::shared_ptr<Camera> Camera::editorCamera = nullptr;

// Editor-Only
void Camera::EditorCameraUpdate() {
    //控制视角和移动
    GLFWwindow* window = Input::GetWindow();
    float deltaTime = Time::deltaTime;
    ProcessMouseScroll(Input::GetMouseScrollOffset());
    Move();
    if (Input::GetMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
        ProcessMouseMovement(Input::GetMouseXOffset(), Input::GetMouseYOffset(), true);
    }
}

KritiaEngine::Camera::Camera()
{
    this->Position = Vector3(0, 1, 5);
    Vector3 vec1 = Vector3(0, 0, 0);
    Vector3 vec2 = Vector3(0, 1, 0);
    this->Forward = Vector3::Normalize(vec1 - vec2);
    this->Target = Vector3(0, 0, 0);
    WorldUp = Vector3(0, 1, 0);
    Right = Vector3::Normalize(Vector3::Cross(Forward, Vector3(0, 1, 0)));
    this->Up = Vector3::Normalize(Vector3::Cross(Right, Forward));
    Speed = 1.f;
    Yaw = -90;
    Pitch = 0;
    MouseSensitivity = 0.1f;
    Zoom = 45.f;
    UpdateCameraVectors();
}

KritiaEngine::Camera::Camera(GameObject* gameObject)
{
    this->gameObject = gameObject;
    this->Position = Vector3(0, 0, 3);
    Vector3 vec1 = Vector3(0, 0, 0);
    Vector3 vec2 = Vector3(0, 1, 0);
    this->Forward = Vector3::Normalize(vec1 - vec2);
    this->Target = Vector3(0, 0, 0);
    WorldUp = Vector3(0, 1, 0);
    Right = Vector3::Normalize(Vector3::Cross(Forward, Vector3(0, 1, 0)));
    this->Up = Vector3::Normalize(Vector3::Cross(Right, Forward));
    Speed = 2.f;
    Yaw = -90;
    Pitch = 0;
    MouseSensitivity = 0.1f;
    Zoom = 45.f;
    UpdateCameraVectors();
}

Matrix4x4 Camera::GetViewMatrix() {
	return Matrix4x4::LookAt(Position, Position + Forward, Up);
}

Vector3 KritiaEngine::Camera::GetPosition()
{
    if (editorCamera != nullptr) {
        return Position;
    }
}

//only in editor
void Camera::Move() {
    float velocity = Speed * Time::deltaTime;
    if (Input::GetKeyDown(GLFW_KEY_W)) {
        Position += Forward * velocity;
    }
    if (Input::GetKeyDown(GLFW_KEY_A)) {
        Position -= Right * velocity;
    }
    if (Input::GetKeyDown(GLFW_KEY_S)) {
        Position -= Forward * velocity;
    }
    if (Input::GetKeyDown(GLFW_KEY_D)) {
        Position += Right * velocity;
    }   
}

//only in editor
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (Pitch > 89.0f) {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f) {
            Pitch = -89.0f;
        }
    }

    // update Forward, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis, only in editor
void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f) {
        Zoom = 1.0f;
    }
    if (Zoom > 45.0f) {
        Zoom = 45.0f;
    }
}

void Camera::UpdateCameraVectors() {
    // calculate the new Forward vector
    Vector3 forward;
    forward.x = Mathf::Cos(Yaw) * Mathf::Cos(Pitch);
    forward.y = Mathf::Sin(Pitch);
    forward.z = Mathf::Sin(Yaw) * Mathf::Cos(Pitch);
    Forward = Vector3::Normalize(forward);
    // also re-calculate the Right and Up vector
    Right = Vector3::Normalize(Vector3::Cross(Forward, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = Vector3::Normalize(Vector3::Cross(Right, Forward));
}
