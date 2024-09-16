#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
      mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
  this->position = position;
  this->world_up = up;
  this->yaw = yaw;
  this->pitch = pitch;
  update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y,
               float up_z, float yaw, float pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movement_speed(SPEED),
      mouse_sensitivity(SENSITIVITY), zoom(ZOOM) {
  position = glm::vec3(pos_x, pos_y, pos_z);
  world_up = glm::vec3(up_x, up_y, up_z);
  this->yaw = yaw;
  this->pitch = pitch;
  update_camera_vectors();
}

// Returns the view matrix calculated using Euler Angles and LookAt matrix
glm::mat4 Camera::get_view_matrix() const {
  return glm::lookAt(position, position + front, up);
}

// Process keyboard input
void Camera::process_keyboard(CameraMovement direction, float delta_time) {
  float velocity = movement_speed * delta_time;
  if (direction == CameraMovement::Forward)
    position += front * velocity;
  if (direction == CameraMovement::Backward)
    position -= front * velocity;
  if (direction == CameraMovement::Left)
    position -= right * velocity;
  if (direction == CameraMovement::Right)
    position += right * velocity;
}

// Process mouse movement input
void Camera::process_mouse_movement(glm::vec2 offset,
                                    GLboolean constrain_pitch) {
  offset *= mouse_sensitivity;

  yaw += offset.x;
  pitch -= offset.y;

  // Ensure pitch is within bounds
  if (constrain_pitch) {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  // Update the camera vectors based on the new Euler angles
  update_camera_vectors();
}

// Process mouse scroll input
void Camera::process_mouse_scroll(float yoffset) {
  zoom -= yoffset;
  if (zoom < 1.0f)
    zoom = 1.0f;
  if (zoom > 45.0f)
    zoom = 45.0f;
}

// Update camera vectors based on the current yaw and pitch
void Camera::update_camera_vectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  this->front = glm::normalize(front);

  // Recalculate Right and Up vectors
  right = glm::normalize(glm::cross(this->front, world_up));
  up = glm::normalize(glm::cross(right, this->front));
}
