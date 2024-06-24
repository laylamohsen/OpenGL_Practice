typedef void* GLeglImageOES;

// MAKE SURE GLEW is included before any OpenGL headers!!
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shaderClass.h"
#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"
#include <stb_image.h>

GLfloat vertices[] = {
    // POSITIONS          //COLORS
    -0.5f, -0.5f, 0.0f,     137.0/255.0f, 85.0/255.0f, 112.0/255.0f,   // outer left
    0.5f, -0.5f, 0.0f,      226.0/255.0f, 158.0/255.0f, 255.0/255.0f,     // outer right
    0.0f,  0.5f, 0.0f,      248.0/255.0f, 248.0/255.0f, 158.0/255.0f,      // outer up
    -0.25f, 0.0f, 0.0f,     137.0/255.0f, 85/255.0f, 112.0/255.0f,   // inner left
    0.25f, 0.0f, 0.0f,      137.0/255.0f, 85/255.0f, 112.0/255.0f,    // inner right
    0.0f, -0.5f, 0.0f,      137.0/255.0f, 85/255.0f, 112.0/255.0f,    // inner down
};

// USED FOR PRIMITIVES -- THIS IS THE ORDER IN WHICH TRIANGLES ARE DRAWN AND THIS AVOIDS DUPLICATE CO-ORDINATES
GLuint indices[] = {
    0, 3, 5, // Lower left triangle
    3, 2, 4, // Lower right triangle
    5, 4, 1  // Upper triangle
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
    glViewport(0, 0, 800, 800);

    
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

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
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6*sizeof(float), (void*)(0*sizeof(float)));
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6*sizeof(float), (void*)(3*sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();
    
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
        // Bind the VAO so OpenGL knows to use it
        VAO1.Bind();
        // Draw primitives, number of indices, datatype of indices, index of indices
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
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
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();
    return 0;
}
