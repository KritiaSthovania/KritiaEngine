# KritiaEngine
一个十分简陋迷你游戏引擎，用于锻炼C++能力以及检验自身所学。在API上尽量模仿了Unity，原因是我个人有Unity使用经验。
![截图](/Image/ScreenShot_Engine.png)

## 环境
- Visual Studio 2022
- C++ 20

## 第三方库

- 已包含：
[stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h), [imgui-docking](https://github.com/ocornut/imgui/tree/docking), [nlohmann json](https://github.com/nlohmann/json), [NativeFileDialog-Extended](https://github.com/btzy/nativefiledialog-extended), [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
- 未包含：
[glfw](https://www.glfw.org/), [glm](https://github.com/g-truc/glm), [glad](https://glad.dav1d.de/), [vulkan](https://vulkan.lunarg.com/), [assimp](https://github.com/assimp/assimp), [boost](https://www.boost.org/)，[bullet](https://github.com/bulletphysics/bullet3)
- 未包含的第三方库推荐直接使用[vcpkg](git@github.com:microsoft/vcpkg.git)安装。
  
## Roadmap
### 准备完成的：
- [x] 基本框架
- Time, Input, Scene 等非常基础的功能
- GameObject 以及 Component
- Component目前包含: Transform, Camera, Light, MeshFilter, MeshRenderer, MonoBehaviour
- 一些基本的数学函数和结构的Wrapper
- 整体框架为Component-Based，Behaviour以及Renderer都存在对应的Manager中

- [x] 基于OpenGL的基本前向渲染功能: 
- 光照和阴影 （点光源、定向光、聚光）
- 2D贴图（Diffuse, Specular, Parallax, Normal)，立方体贴图，天空盒
- 着色器 （透明、Bling-Phong)
- GPU Instancing (但是没有性能提升，原因未知，可能是测试用的模型太简单）
- 为点光源实现了基本的PBR
![PBR](/Image/ScreenShot_PBR.png)

- [x] CPU渲染器
- 仅支持了Windows平台，需要将Settings.json中的 *RenderingBackend* 改为 __Software__。
- 光照和阴影 （点光源、定向光、聚光）
- 软光栅，固定渲染管线（Bling-Phong)
- 2D贴图（Diffuse, Specular, Parallax, Normal)，立方体贴图，天空盒
![CPU渲染器效果](/Image/ScreenShot_SoftwareRendering.png)

- [x] GUI
- 基于ImGui-Docking
- 不支持CPU渲染器
- Hierachy, Inspector, AssetExplorer, Console四个窗口以及MainMenuBar和Settings窗口
- __移动、旋转以及缩放的Widget目前有bug无法使用__。

- [x] 资源管理
- 支持材质（Material），模型网格（fbx，obj），贴图（png，jpg），预制体（Prefab）的导入
- 用资源池进行资源管理以及垃圾回收: 基于最简单的Reference Counting，支持Material, Mesh以及Texture。预制体不会被回收，并且普通的GameObject会直接在Destory方法中析构。

- [x] 物理引擎
- 简单集成了[Bullet](https://github.com/bulletphysics/bullet3)（刚体和箱型碰撞体）。

- [x] Play In Editor
- 点击Play可以启动物理引擎和MonoBehaviour，但是暂时没有单独的GameMode。
### 低优先级：
- [ ] 脚本

## 参考资料
- [Learn OpenGL CN](https://learnopengl-cn.github.io/)
- [GAMES104](https://games-cn.org/games104/)
- [Unity](https://docs.unity3d.com/2022.2/Documentation/ScriptReference/index.html)
- [极客Vulkan教程](https://geek-docs.com/vulkan/vulkan-tutorial/vulkan-tutorial-index.html)
- [Vulkan Tutorial](https://vulkan-tutorial.com)
