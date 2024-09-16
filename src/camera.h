#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum class CameraMovement { Forward, Backward, Left, Right };

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.001f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  // camera Attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 world_up;
  // euler Angles
  float yaw;
  float pitch;
  // camera options
  float movement_speed;
  float mouse_sensitivity;
  float zoom;

  // constructor with vectors
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
         float pitch = PITCH);
  // constructor with scalar values
  Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y,
         float up_z, float yaw, float pitch);

  glm::mat4 get_view_matrix() const;

  // Process keyboard input
  void process_keyboard(CameraMovement direction, float delta_time);

  // Process mouse movement input
  void process_mouse_movement(glm::vec2 offset,
                              GLboolean constrain_pitch = true);

  // Process mouse scroll input
  void process_mouse_scroll(float yoffset);

private:
  void update_camera_vectors();
};
