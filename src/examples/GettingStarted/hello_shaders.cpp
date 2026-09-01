#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../../core/shader.h"

static float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.5f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.5f,   // 左下
     0.0f,  0.5f, 0.0f,  0.5f, 0.0f, 1.0f    // 顶部
};


static GLFWwindow* configuration_window() {
    // 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }
    // 前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    glViewport(0, 0, 800, 600);

    return window;
}


static int process(unsigned int& VBO, unsigned int& VAO) {

    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 传VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 顶点颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return 0;
}

static int rendering(GLFWwindow* window, unsigned int& VAO, Shader& shaderProgram) {
    int count = 0;
    float offset = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // 设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清屏
        glClear(GL_COLOR_BUFFER_BIT);
        // 输入
        //processInput(window);

        // 渲染指令
        // 但是更新一个uniform之前你必须先使用程序（调用glUseProgram)，因为它是在当前激活的着色器程序中设置uniform的
        shaderProgram.use();

        // 更新uniform颜色
        //float timeValue = glfwGetTime();
        //float greenValue = sin(timeValue) / 2.0f + 0.5f;
        //int vertexColorLocation = glGetUniformLocation(shaderProgram.ID, "ourColor");
        ////std::cout << vertexColorLocation << std::endl;
        //glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            offset += 0.01f;
            shaderProgram.setFloat("offset", offset);

            std::cout << GLFW_KEY_D;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            offset -= 0.01f;
            shaderProgram.setFloat("offset", offset);

            std::cout << GLFW_KEY_A;
        }
        shaderProgram.setFloat("offset", 0.3f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
          
        //glBindVertexArray(0);
        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
    // 释放之前的分配的所有资源
    glfwTerminate();
    return 0;
}

static int hello_shaders_example() {
    GLFWwindow* window = configuration_window();
    if (!window) return -1; // 检查窗口是否创建成功

    
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    // 着色器处理
    Shader ourShader("../assets/shaders/GettingStarted/shadersChapter/vertexShader.glsl", "../assets/shaders/GettingStarted/shadersChapter/fragmentShader.glsl");
    process(VBO, VAO);

    rendering(window, VAO, ourShader);
    return 0;
}

int main() {
    hello_shaders_example();
}