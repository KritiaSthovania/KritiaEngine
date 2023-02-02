# KritiaEngine
一个在API上尽量模仿Unity的迷你游戏引擎，用于锻炼C++能力以及检验自身所学。
## 环境
- Visual Studio 2022
- C++ 20

## 第三方库
###
- 已包含：
[stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h), [imgui](https://github.com/ocornut/imgui)
- 未包含：
[glfw](https://www.glfw.org/), [glm](https://github.com/g-truc/glm), [glad](https://glad.dav1d.de/), [assimp](https://github.com/assimp/assimp)
- 未包含的第三方库推荐直接使用[vcpkg](git@github.com:microsoft/vcpkg.git)安装。
  
## Roadmap
- [x] 基于OpenGL的基本渲染功能
- [ ] GUI
- [ ] 资源管理
- [ ] 物理引擎
- [ ] CPU渲染器
- [ ] Play In Editor
- [ ] 脚本

## 参考资料
- [Learn OpenGL CN](https://learnopengl-cn.github.io/)
- [GAMES104](https://games-cn.org/games104/)