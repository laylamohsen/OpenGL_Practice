#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "shaderClass.h"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include "Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"

// Function to generate sphere vertices and indices
void createSphere(float radius, unsigned int rings, unsigned int sectors, std::vector<GLfloat>& vertices, std::vector<GLuint>& indices) {
    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);
    int r, s;

    vertices.resize(rings * sectors * 8);
    std::vector<GLfloat>::iterator v = vertices.begin();
    for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
        float const y = sin(-M_PI_2 + M_PI * r * R);
        float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
        float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

        *v++ = x * radius;
        *v++ = y * radius;
        *v++ = z * radius;

        *v++ = x;
        *v++ = y;
        *v++ = z;

        *v++ = s * S;
        *v++ = r * R;
    }

    indices.resize(rings * sectors * 6);
    std::vector<GLuint>::iterator i = indices.begin();
    for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
        *i++ = r * sectors + s;
        *i++ = r * sectors + (s + 1);
        *i++ = (r + 1) * sectors + (s + 1);

        *i++ = r * sectors + s;
        *i++ = (r + 1) * sectors + (s + 1);
        *i++ = (r + 1) * sectors + s;
    }
}

const unsigned int width = 800;
const unsigned int height = 800;

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, width, height);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    Shader shaderProgram("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Shaders/default.vert", "/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Shaders/default.frag");
    Shader lightShader("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Shaders/light.vert", "/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Shaders/light.frag");

    std::vector<GLfloat> sphereVertices;
    std::vector<GLuint> sphereIndices;
    createSphere(0.02f, 36, 18, sphereVertices, sphereIndices);

    VAO VAO1;
    VAO1.Bind();
    VBO VBO1(&sphereVertices[0], sphereVertices.size() * sizeof(GLfloat));
    EBO EBO1(&sphereIndices[0], sphereIndices.size() * sizeof(GLuint));
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 8 * sizeof(float), (void*)(8 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    std::vector<GLfloat> lightVertices;
    std::vector<GLuint> lightIndices;
    createSphere(0.5f, 50, 25, lightVertices, lightIndices);

    VAO lightVAO;
    lightVAO.Bind();
    VBO lightVBO(&lightVertices[0], lightVertices.size() * sizeof(GLfloat));
    EBO lightEBO(&lightIndices[0], lightIndices.size() * sizeof(GLuint));
    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    lightVAO.LinkAttrib(lightVBO, 1, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(4.0f, 0.5f, 0.5f);

    lightShader.Activate();
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    Texture suntexture("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Textures/sun.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    suntexture.texUnit(lightShader, "tex0", 0);
    
    
    shaderProgram.Activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Texture texture("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Textures/earth.png", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    texture.texUnit(shaderProgram, "tex0", 0);
    Texture textureSpec("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Textures/earthSec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
    textureSpec.texUnit(shaderProgram, "tex1", 1);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);
        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        
        suntexture.Bind();

        glm::mat4 lightModel = glm::mat4(1.0f);
        
        lightModel =  glm::translate(lightModel, glm::vec3(0.0f, 0.0f, 0.0f));

        
        // Translate the Earth to an initial position offset from the Sun

          // Rotate the Earth around the Sun over time
        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));

//          model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
//        glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, lightIndices.size(), GL_UNSIGNED_INT, 0);

        shaderProgram.Activate();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(shaderProgram, "camMatrix");

        texture.Bind();
        textureSpec.Bind();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
 // Initial offset from the Sun
        glm::mat4 earthRotation = glm::mat4(1.0f);
          earthRotation = glm::rotate(earthRotation, (float)glfwGetTime() * 10.0f, glm::vec3(0.0f, 1.0f, 0.0f));
          model = model * earthRotation;
//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

      
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    texture.Delete();
    textureSpec.Delete();
    lightVAO.Delete();
    lightVBO.Delete();
    lightEBO.Delete();
    lightShader.Delete();
    suntexture.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
