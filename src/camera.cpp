#include "Misc/camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float FOV, float aspectratio, float near, float far,
               glm ::vec3 dir, glm::vec3 pos) {
    this->FOV = FOV;
    this->aspectratio = aspectratio;
    this->dir = dir;
    this->pos = pos;
    this->far = far;
    this->near = near;
    this->right = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f),dir));
    this->up = glm::cross(dir,right);
}
void Camera::updatevec() {
    if (!isDirty)
        return;
    if (dir != glm::vec3(0.0f, 1.0f, 0.0f))
        this->right =
            glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f),dir));
    this->up = glm::cross(dir, right);
}
void Camera::updatemat(int projloc, int viewloc) {
    if (!isDirty)
        return;
    proj = glm::perspective(FOV, aspectratio, near, far);
    view = glm::lookAt(pos, pos + dir, up);

    glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
    isDirty = false;
}
