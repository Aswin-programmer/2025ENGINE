#include "EditorCamera.h"

EditorCamera::EditorCamera(float fov, float aspect, float nearClip, float farClip)
    : m_FOV(fov), m_AspectRatio(aspect), m_NearClip(nearClip), m_FarClip(farClip) {
    UpdateCamera();
}

glm::mat4 EditorCamera::GetViewMatrix() const {
    return glm::lookAt(m_Position, m_Target, m_Up);
}

glm::mat4 EditorCamera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
}

void EditorCamera::OnMouseRotate(float deltaX, float deltaY) {
    const float sensitivity = 0.3f;
    m_Yaw += deltaX * sensitivity;
    m_Pitch += deltaY * sensitivity;

    // Clamp pitch to prevent flip
    if (m_Pitch > 89.0f) m_Pitch = 89.0f;
    if (m_Pitch < -89.0f) m_Pitch = -89.0f;

    UpdateCamera();
}

void EditorCamera::OnMousePan(float deltaX, float deltaY) {
    float panSpeed = m_Distance * 0.002f;
    glm::vec3 right = glm::normalize(glm::cross(m_Target - m_Position, m_Up));
    glm::vec3 upMove = glm::normalize(glm::cross(right, m_Target - m_Position));

    m_Target += -right * deltaX * panSpeed + upMove * deltaY * panSpeed;

    UpdateCamera();
}

void EditorCamera::OnMouseScroll(float delta) {
    m_Distance -= delta;
    if (m_Distance < 1.0f) m_Distance = 1.0f;
    if (m_Distance > 100.0f) m_Distance = 100.0f;

    UpdateCamera();
}

void EditorCamera::SetViewportSize(float width, float height) {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    m_AspectRatio = width / height;
}

void EditorCamera::UpdateCamera() {
    float yawRad = glm::radians(m_Yaw);
    float pitchRad = glm::radians(m_Pitch);

    glm::vec3 direction;
    direction.x = cos(pitchRad) * cos(yawRad);
    direction.y = sin(pitchRad);
    direction.z = cos(pitchRad) * sin(yawRad);

    m_Position = m_Target - glm::normalize(direction) * m_Distance;
}
