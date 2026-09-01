#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 输入用aPos代表，输出必须把位置数据赋值给预定义的gl_Position变量，，它在幕后是vec4类型的
static const char* vertexShaderSource = "#version 330 core\
layout (location = 0) in vec3 aPos;\
void main()\
{\
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\
}\0";

static const char* fragmentShaderSource = "#version 330 core\
out vec4 FragColor;\
void main()\
{\
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\
}\0";

float vertices1[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

float vertices2[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

unsigned int indices[] = {
    // 注意索引从0开始! 
    // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
    // 这样可以由下标代表顶点组合成矩形

    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
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


static int process(unsigned int& VBO, unsigned int& VAO, unsigned int& shaderProgram, unsigned int& EBO, int& flag) {
 
    //unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 绑定VAO
    glBindVertexArray(VAO);

    // 1. 把顶点数据储存在显卡的内存中，用VBO这个顶点缓冲对象管理
    // 1.1 使用 glGenBuffers 函数生成一个带有缓冲ID的VBO对象
    //unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 1.2 用 glBindBuffer 函数把新创建的缓冲绑定到 GL_ARRAY_BUFFER（缓冲类型）目标上
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // 1.3 把用户定义的数据复制到当前绑定缓冲的函数
    // GL_STATIC_DRAW ：数据不会或几乎不会改变。
    // GL_DYNAMIC_DRAW：数据会被改变很多。
    // GL_STREAM_DRAW ：数据每次绘制时都会改变。
    //           目标缓冲的类型，指定传输数据的大小(以字节为单位)，希望发送的实际数据，希望显卡如何管理给定的数据
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    if (flag == 1) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
        std::cout << "Upload vertices1, size= << sizeof(vertices1) ";
    }
    else if (flag == 2) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
        std::cout << "Upload vertices2, size= << sizeof(vertices2) ";
        // 创建 EBO 并上传索引
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        std::cout << "Generated EBO";
    }
    else {
        std::cerr << "Invalid flag";
        return -1;
    }
   
    // 2. 编译着色器
    // 2.1 创建一个顶点着色器对象，注意还是用ID来引用的
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 2.2 把这个着色器源码附加到着色器对象上，然后编译它
    //            要编译的着色器对象，传递的源码字符串数量，顶点着色器真正的源码，
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 2.3 片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // 2.4 着色程序对象
    //unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // 把之前编译的着色器附加到程序对象上，然后用 glLinkProgram 链接它们
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    
    // 把顶点数组复制到缓冲中供OpenGL使用
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    // 指定的“槽位”，指定顶点属性的大小，指定数据的类型，定义我们是否希望数据被标准化(Normalize)，连续的顶点之间的间隔（步长），偏移值（offset）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return 0;
}

static int rendering(GLFWwindow* window, unsigned int* shaderProgram, unsigned int* VAO, int& flag) {
    int count = 0;

    while (!glfwWindowShouldClose(window))
    {
        // 设置清屏颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清屏
        glClear(GL_COLOR_BUFFER_BIT);
        // 输入
        //processInput(window);

        // 渲染指令
        glUseProgram(*shaderProgram);
        glBindVertexArray(*VAO);
        if (flag == 1) {
            // 希望绘制的类型，指定了顶点数组的起始索引，指定我们打算绘制多少个顶点
            glDrawArrays(GL_TRIANGLES, 0, 3);
            if (count == 0) {
                std::cout << "execute glDrawArrays";
                count++;
            }
        }
        else if(flag == 2){
            //绘制的模式，打算绘制顶点的个数，索引的类型，指定EBO中的偏移量
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            if (count == 0) {
                std::cout << "execute glDrawElements";
                count++;
            }
        }
        //glBindVertexArray(0);
        // 检查并调用事件，交换缓冲
        glfwPollEvents();
        glfwSwapBuffers(window);

    }
    // 释放之前的分配的所有资源
    glfwTerminate();
    return 0;
}

static int hello_Triangle_example() {
    GLFWwindow* window = configuration_window();
    if (!window) return -1; // 检查窗口是否创建成功

    int flag = 1;
    std::cout << "choose model: \n 1 DrawArrays\n 2 DrawElements";
    std::cin >> flag;
    unsigned int VBO = 0;
    unsigned int VAO = 0;
    unsigned int EBO = 0;
    unsigned int shaderProgram = 0;
    process(VBO, VAO, shaderProgram, EBO, flag);

    rendering(window, &shaderProgram, &VAO, flag);
    return 0;
}

int main() {
    hello_Triangle_example();
}