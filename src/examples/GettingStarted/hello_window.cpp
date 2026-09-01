#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 自定义回调函数，需要被注册
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout << "framebuffer_size_callback is called" ;
}

// 处理输入的
static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        std::cout << GLFW_KEY_ESCAPE ;
    }

}

static int hello_window_example()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到。
    // 宽，高，标题，暂时忽略
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    // GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 前两个参数控制窗口左下角的位置。第三个和第四个参数控制渲染窗口的宽度和高度（像素）。
    glViewport(0, 0, 800, 600);

    // 注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数
    // 当窗口被第一次显示的时候framebuffer_size_callback也会被调用
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 保持窗口不退出
    // 渲染循环
    float R = 0.2f;
    while (!glfwWindowShouldClose(window))
    {
        // 设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清屏
        glClear(GL_COLOR_BUFFER_BIT);
        // 输入
        processInput(window);

        // 渲染指令

        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);

        if (R > 1.0f) {
            R = 0.2f;
        }
        R += 0.1f;
    }
    // 释放之前的分配的所有资源
    glfwTerminate();
    return 0;
}

int main() {
    hello_window_example();
}