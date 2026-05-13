#include "Misc/camera.hpp"
#include <glad/glad.h>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLFW_INCLUDE_NONE
#include "Misc/shader.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "Misc/terrain.hpp"
#include <stb/stb_image.h>
#include <vector>
#include <future>

int width = 1066, height = 600;
int render_dis = 8;

Camera camera(45.0f, (float)width / height, 0.1f, 1000.0f,
              glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 3.0f));

void framebuffer_cb(GLFWwindow *window, int width, int height);
void mouse_cb(GLFWwindow *window, double x, double y);
void scroll_cb(GLFWwindow *window, double x, double y);
void process_inp(GLFWwindow *window, double delta);

std::vector<float> meshdata, meshdata2;

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
    double st = 0, en = 0;
    st = glfwGetTime();
    int thread_chunks = render_dis / 2;


    std::vector<std::future<std::vector<float>>> futures;

    for (int i = -render_dis; i < render_dis; i++) {
        for (int j = -render_dis; j < render_dis; j++) {

            futures.push_back(
                std::async(std::launch::async, [i, j, &thread_chunks]() {
                    Chunk chunk(i, j);
                    int ***blockdata = generateWorld(chunk, thread_chunks);

                    std::vector<float> local_mesh =
                        generatefaces(blockdata, chunk, thread_chunks);

                    return local_mesh;
                }));
        }
    }

    for (auto &f : futures) {
        std::vector<float> chunk_mesh = f.get();

        meshdata.reserve(meshdata.size() + chunk_mesh.size());
        meshdata.insert(meshdata.end(), chunk_mesh.begin(), chunk_mesh.end());
    }
    en = glfwGetTime();
    std::cout << en - st << std::endl;
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, meshdata.size() * sizeof(float),
                 meshdata.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8,
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // glBindVertexArray(0);

    Shader shader1("shader/vertex.glsl", "shader/fragment.glsl");

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    stbi_set_flip_vertically_on_load(1);
    int tw1, th1, tch1, tw2, th2, tch2;
    unsigned char *sidepro = stbi_load(
        "/home/leonuraht/Downloads/grass_side(1).jpg", &tw1, &th1, &tch1, 3);
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw1, th1, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, sidepro);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    stbi_image_free(sidepro);
    unsigned char *toppro = stbi_load("/home/leonuraht/Downloads/grass_top.jpg",
                                      &tw2, &th2, &tch2, 3);
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw2, th2, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, toppro);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    stbi_image_free(toppro);

    glUseProgram(shader1.program);
    int projectionmat = glGetUniformLocation(shader1.program, "projection"),
        viewmat = glGetUniformLocation(shader1.program, "view"),
        modelmat = glGetUniformLocation(shader1.program, "model"),
        lightdir = glGetUniformLocation(shader1.program, "light.dir"),
        campos = glGetUniformLocation(shader1.program, "campos"),
        texture00 = glGetUniformLocation(shader1.program, "texture0"),
        texture01 = glGetUniformLocation(shader1.program, "texture1");

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelmat, 1, GL_FALSE, glm::value_ptr(model));
    camera.updatemat(projectionmat, viewmat);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glm::vec3 lightd = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.5f));
    glUniform3f(lightdir, lightd.x, lightd.y, lightd.z);
    glUniform1i(texture00, 0);
    glUniform1i(texture01, 1);

    double pasttime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double delta = time - pasttime;
        pasttime = time;
        process_inp(window, delta);
        glClearColor(0.3f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.updatevec();
        camera.updatemat(projectionmat, viewmat);
        glUniform3f(campos, camera.pos.x, camera.pos.y, camera.pos.z);
        glDrawArrays(GL_TRIANGLES, 0, meshdata.size() / 8);

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

void process_inp(GLFWwindow *window, double delta) {
    float camspd = 20 * delta;
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
        camera.pos += glm::vec3(0.0f, camspd, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
        camera.isDirty = true;
        camera.pos -= glm::vec3(0.0f, camspd, 0.0f);
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

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

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
