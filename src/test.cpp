#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


/*
                            VERTEX

A | Position (2.1, 1.2, 0.2), Color (1, 0, 0)
B | Position (5.3, 11.8, -4.6), Color (0, 1, 0)
C | Position (14.8, 9.5, 2.1), Color (0, 0, 1)

ABC -> vertex shader -> fragment shader (for every pixel in triange) ->
-> pixel color

    GLSL (OpenGL Shading Language, Graphics Library Shader Language)

https://ru.wikipedia.org/wiki/OpenGL_Shading_Language

*/

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

const char* vertex_shader = "#version 460\n"
                            "layout(location = 0) in vec3 vertex_position;"
                            "layout(location = 1) in vec3 vertex_color;"
                            "out vec3 color;"
                            "void main() {"
                            "   color = vertex_color;"
                            "   gl_Position = vec4(vertex_position, 1.0);}"; //from -1 to 1

const char* fragment_shader = "#version 460\n"
                              "in vec3 color;"
                              "out vec4 frag_color;" // colors + alpha channel
                              "void main() {"
                              "   frag_color = vec4(color, 1.0);}";

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

int main(void)
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

    // Perform shaders

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);

    GLuint shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vs);
    glAttachShader(shader_program_id, fs);
    glLinkProgram(shader_program_id); // link shaders

    glDeleteShader(vs);
    glDeleteShader(fs);

    // Send to videocard (using VBO - https://ru.wikipedia.org/wiki/Vertex_Buffer_Object)

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

        glUseProgram(shader_program_id);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        // TODO: figure out with multiple buffering - https://en.wikipedia.org/wiki/Multiple_buffering
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // free all resources
    glfwTerminate();
    return 0;
}
