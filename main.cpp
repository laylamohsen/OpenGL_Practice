
#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "shaderClass.h"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include"Texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

GLfloat vertices[] = {
    // POSITIONS          // COLORS                        // TEX COORDS
    -0.5f, -0.5f, 0.0f,   137.0f/255.0f, 85.0f/255.0f, 112.0f/255.0f,  0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,   226.0f/255.0f, 158.0f/255.0f, 255.0f/255.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   248.0f/255.0f, 248.0f/255.0f, 158.0f/255.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,   137.0f/255.0f, 85.0f/255.0f, 112.0f/255.0f,  1.0f, 1.0f,
};

GLuint indices[] = {
    0, 1, 2,
    1, 3, 2
};

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Tell GLFW what version of OpenGL we are using (OpenGL 3.3)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile (modern functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window object of 800 by 800 pixels, naming it "OpenGL"
    GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 800, 800);

    // Load GLEW to configure OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Shaders/default.vert", "/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Shaders/default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    int widthImg, heightImg, numColCh;
//    stbi_set_flip_vertically_on_load(true);
    std::string texturePath = "/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Textures/buckooka.png";
    if (!fileExists(texturePath)) {
        std::cerr << "Texture file does not exist at path: " << texturePath << std::endl;
        return -1;
    }

    std::cout << "Loading texture from: " << texturePath << std::endl;
    unsigned char* bytes = stbi_load(texturePath.c_str(), &widthImg, &heightImg, &numColCh, 0);

    if (!bytes) {
        std::cerr << "Failed to load texture: " << stbi_failure_reason() << std::endl;
        return -1;
    } else {
        std::cout << "Loaded texture: " << widthImg << "x" << heightImg << " with " << numColCh << " channels" << std::endl;
    }
    // Texture
Texture bucky(("/Users/layla/Desktop/OpenGL_Tutorial/OpenGL_Tutorial/Resources/Textures/buckooka.png"), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    
    bucky.texUnit(shaderProgram, "tex0", 0);
//    GLuint texture;
//    glGenTextures(1, &texture);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    if (numColCh == 4) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
//    } else if (numColCh == 3) {
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
//    } else {
//        std::cerr << "Unknown number of color channels in texture" << std::endl;
//        stbi_image_free(bytes);
//        return -1;
//    }
//    glGenerateMipmap(GL_TEXTURE_2D);
//    stbi_image_free(bytes);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    // Main while loop
    while (!glfwWindowShouldClose(window)) {
        // Specify the color of the background
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT);
        // Tell OpenGL which Shader Program we want to use
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        bucky.Bind();
        /*glUniform1i(tex0Uni, 0);*/ // Ensure the uniform is set to the correct texture unit
        
//        glBindTexture(GL_TEXTURE_2D, texture);
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    bucky.Delete();
    
//    glDeleteTextures(1, &texture);
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
