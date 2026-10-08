# LearnOpenGL

基于 C++17、CMake 和 OpenGL 的学习项目，示例按 LearnOpenGL 教程章节组织。当前 CMake 配置生成 PBR 与 IBL 相关示例；其他章节代码保留在源码目录中，便于逐步学习和启用。

## 目录结构

```text
.
├── assets/
│   ├── images/          # 纹理、天空盒等图像资源
│   ├── models/          # Backpack、nanosuit、planet、rock 等模型
│   └── shaders/         # 按教程章节分类的 GLSL 着色器
├── src/
│   ├── core/            # 窗口、着色器、纹理、相机、网格和模型等公共代码
│   └── examples/        # 各章节的 OpenGL 示例
│       ├── GettingStarted/
│       ├── Lighting/
│       ├── ModelLoading/
│       ├── AdvancedOpenGL/
│       ├── AdvancedLighting/
│       └── PBR/
├── CMakelists.txt       # CMake 项目配置
└── README.md
```

`assets/shaders/` 下的目录与 `src/examples/` 中的章节对应。示例运行时使用的资源路径取决于项目根目录，因此建议从项目根目录启动程序。

## Git 学习记录
git提交记录中有学后总结