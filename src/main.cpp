#include "Misc/camera.hpp"
#include <glad/glad.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLFW_INCLUDE_NONE
#include "Misc/shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

int width = 800, height = 600;

float vertices[] = {
    // --- Front Face (Z = +0.5) ---
    // Normal points +Z.
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-Left
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Bottom-Right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // Top-Right
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,   // Top-Right
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Top-Left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-Left

    // --- Back Face (Z = -0.5) ---
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // Bottom-Left (from outside view)
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom-Right
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // Top-Right
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // Top-Right
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,   // Top-Left
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  // Bottom-Left

    // --- Left Face (X = -0.5) ---
    // Normal points -X.
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Bottom-Left
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  // Bottom-Right
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   // Top-Right
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,   // Top-Right
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  // Top-Left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Bottom-Left

    // --- Right Face (X = +0.5) ---
    // Normal points +X.
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // Bottom-Left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-Right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // Top-Right
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  // Top-Right
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,   // Top-Left
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,  // Bottom-Left

    // --- Top Face (Y = +0.5) ---
    // Normal points +Y.
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-Left
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,   // Bottom-Right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Top-Right
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Top-Right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-Left
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-Left

    // --- Bottom Face (Y = -0.5) ---
    // Normal points -Y.
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom-Left
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  // Bottom-Right
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   // Top-Right
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,   // Top-Right
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,  // Top-Left
    -0.5f, -0.5f, -0.5, 0.0f, -1.0f, 0.0f   // Bottom-Left
};

Camera camera(45.0f, (float)width / height, 0.1f, 100.0f,
              glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 3.0f));

void framebuffer_cb(GLFWwindow *window, int width, int height);
void mouse_cb(GLFWwindow *window, double x, double y);
void scroll_cb(GLFWwindow *window, double x, double y);
void process_inp(GLFWwindow *window,double delta);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(width, height, "Ray Tracer", NULL, NULL);
    if (window == NULL) {
        std::cerr << "WINDOW CREATION FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD INIT FAILED" << std::endl;
        return -1;
    }
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_cb);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_cb);
    glfwSetScrollCallback(window, scroll_cb);
    glEnable(GL_DEPTH_TEST);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // glBindVertexArray(0);

    Shader shader1("shader/vertex.glsl", "shader/fragment.glsl");

    glUseProgram(shader1.program);
    int projectionmat = glGetUniformLocation(shader1.program, "projection"),
        viewmat = glGetUniformLocation(shader1.program, "view"),
        modelmat = glGetUniformLocation(shader1.program, "model");

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelmat, 1, GL_FALSE, glm::value_ptr(model));
    camera.updatemat(projectionmat, viewmat);


    double pasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double delta = time - pasttime;
        pasttime = time;
        process_inp(window,delta);
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updatevec();
        camera.updatemat(projectionmat, viewmat);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_cb(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera.aspectratio = (float)width / height;
}

void process_inp(GLFWwindow *window,double delta) {
    float camspd = 2 * delta;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos += camera.dir * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos += camera.right * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos -= camera.dir * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos -= camera.right * camspd;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos += glm::vec3(0.0f,camspd,0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)== GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos -= glm::vec3(0.0f,camspd,0.0f);
    }
}
void mouse_cb(GLFWwindow *window, double x, double y) {
    camera.isDirty = true;
    if (camera.firstmouse) {
        camera.xlast = x;
        camera.ylast = y;
        camera.firstmouse = false;
    }
    float xoff = (x - camera.xlast) * camera.mousesens;
    float yoff = (y - camera.ylast) * camera.mousesens;
    camera.xlast = x;
    camera.ylast = y;

    camera.pitch -= yoff;
    camera.yaw += xoff;

    if (camera.pitch > 90.0f)
        camera.pitch = 90.0f;
    if (camera.pitch < -90.0f)
        camera.pitch = -90.0f;

    glm::vec3 direction;
    direction.x =
        cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z =
        sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.dir = glm::normalize(direction);
}

void scroll_cb(GLFWwindow *window, double x, double y) {
    camera.isDirty = true;
    camera.FOV -= (float)y * 0.05;
    if (camera.FOV > 90.0f)
        camera.FOV = 90.0f;
    if (camera.FOV < 1.0f)
        camera.FOV = 1.0f;
}
