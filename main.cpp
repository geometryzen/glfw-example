#include <iostream>

// GLAD
// This should be included before GLFW.
// The include file for GLAD contains the correct OpenGL header includes.
// #include <glad/glad.h>

// GL_LITE OpenGL Loader
#define GL_LITE_IMPLEMENTATION
#include "gl_lite.h"

// GLFW
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>

static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
    {
        {-0.6f, -0.4f, 1.f, 0.f, 0.f},
        {0.6f, -0.4f, 0.f, 1.f, 0.f},
        {0.f, 0.6f, 0.f, 0.f, 1.f}};

static const char *vertex_shader_text =
    "#version 110\n"
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

static const char *fragment_shader_text =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s (%d)\n", description, error);
}

// Is called whenever a key is pressed/released via GLFW.
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

const int WIDTH = 800;
const int HEIGHT = 600;

int main(void)
{
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    // Set the error callback at the earliest opportunity.
    glfwSetErrorCallback(error_callback);

    // Init GLFW
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // Set the options for GLFW.
    // TODO: The OpenGL and GLSL versions are reported s follows:
    // OpenGL 4.6.0 NVIDIA 418.56, GLSL 4.60 NVIDIA
    // However, I can't make an explicit request for 4.6
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Context profiles are only defined for OpenGL version 3.2 and above (65540).
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions.
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL C++ GLFW", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    // Make the context current for this thread.
    glfwMakeContextCurrent(window);

    // Load OpenGL using an external loader.
    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    if (!gl_lite_init())
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void *)0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void *)(sizeof(float) * 2));

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Process input here?

        int width, height;
        mat4x4 m, p, mvp;
        // Another way to do this is by using a callback.
        // See https://learnopengl.com/Getting-started/Hello-Window
        // See https://www.glfw.org/docs/latest/quick_guide.html
        glfwGetFramebufferSize(window, &width, &height);
        float ratio = width / (float)height;
        glViewport(0, 0, width, height);

        // Set the clear color.
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Clear the color buffer.
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float)glfwGetTime());
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)mvp);

        // Render.
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions.
        glfwPollEvents();

        // Swap the screen buffers.
        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);

    // Terminte GLFW, clearing any resources alloacted by GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
