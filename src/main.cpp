#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include "Misc/shader.hpp"

int width = 800, height = 600;

float vertices[] = {
    0.5f,  0.5f,  0.0f, // tpri
    0.5f,  -0.5f, 0.0f, // btri
    -0.5f, -0.5f, 0.0f, // btlf
    -0.5f, 0.5f,  0.0f  // btri
};

void framebuffer_cb(GLFWwindow *window, int width, int height);
void mouse_cb(GLFWwindow *window, double x, double y);
void scroll_cb(GLFWwindow *window, double x, double y);
void process_inp(GLFWwindow *window);

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
    glEnable(GL_DEPTH_TEST);


    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float)*3,(void*)0);
    glEnableVertexAttribArray(0);
    // glBindVertexArray(0);

    Shader shader1("shader/vertex.glsl","shader/fragment.glsl");
   
    glUseProgram(shader1.program);
    while (!glfwWindowShouldClose(window)) {
        process_inp(window);
        glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glfwTerminate();
    return 0;
}

void framebuffer_cb(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_inp(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
}
