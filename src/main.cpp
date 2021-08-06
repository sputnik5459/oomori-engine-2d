#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"


GLfloat point[] = {
   0.0f,  0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
  -0.5f, -0.5f, 0.0f
};

GLfloat colors[] = {
   1.0f, 0.0f, 0.0f,
   0.0f, 1.0f, 0.0f,
   0.0f, 0.0f, 1.0f
};


// no define because it's hard to debug
int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;

    // show OpenGL where to draw (square)
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }
}

int main(int argc, char** argv)
{
    GLFWwindow* pWindow;

    // initialize the library
    if (!glfwInit()) {
      std::cout << "glfwInit failed!" << std::endl;
      return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a windowed mode window and its OpenGL context
    pWindow = glfwCreateWindow(
      g_windowSizeX, g_windowSizeY, "oomori-engine-2d", nullptr, nullptr
    );

    if (!pWindow)
    {
      std::cout << "glfwCreateWindow failed!" << std::endl;
      glfwTerminate();
      return -1;
    }

    // register callbacks
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

    // make the window's context current
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()) {
      std::cout << "gladLoadGL failed!" << std::endl;
      return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(0, 1, 0 ,1);


    { // new namespace in order to use destructor of ResourceManager
      ResourceManager resourceManager(argv[0]);
      auto pDefaultShaderProgram = resourceManager.loadShaders(
        "DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt"
      );
      if (!pDefaultShaderProgram)
      {
        std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
        return -1;
      }

      resourceManager.loadTexture(
              "defaultTexture",
                //TODO: add png name for testing purposes
                "res/textures/..."
              );

      GLuint points_vbo = 0;
      glGenBuffers(1, &points_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); // from RAM to graphics card's memory

      GLuint colors_vbo = 0;
      glGenBuffers(1, &colors_vbo);
      glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW); // from RAM to graphics card's memory

      GLuint vao = 0;
      glGenVertexArrays(1, &vao);
      glBindVertexArray(vao);

      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

      /* Loop until the user closes the window */
      while (!glfwWindowShouldClose(pWindow))
      {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        pDefaultShaderProgram->use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        // TODO: figure out with multiple buffering - https://en.wikipedia.org/wiki/Multiple_buffering
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
      }
    }


    // free all resources
    glfwTerminate();
    return 0;
}
