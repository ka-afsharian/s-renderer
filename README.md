# s-renderer

A small, modern c++ renderer built with OpenGL.

---
## Features

- Modern **OpenGL** renderer written in **C++23**
- Modular design with clear separation of engine components
- **SDL3** window and context management
- **GLAD** for OpenGL function loading
- **GLM** for math operations (matrices, vectors, transforms)
- **stb_image** for texture loading
- Example scenes and test shaders included

---

## Libraries Used

| Library | Purpose | Link |
|----------|----------|------|
| [GLAD](https://glad.dav1d.de/) | OpenGL function loader | Bundled |
| [GLM](https://github.com/g-truc/glm) | Mathematics library for graphics | Bundled |
| [stb_image / stb_image_write](https://github.com/nothings/stb) | Image loading and saving | Bundled |
| [assimp](https://github.com/assimp/assimp) | Model importing | Bundled |

All third-party dependencies are located under the `third_party/` directory or handled via CMake.

---

## Build Instructions

### Prerequisites
- C++23 or later compiler  
- CMake ≥ 3.16  
- OpenGL 4.6+ capable system  
- Your GPU must support GL_ARB_bindless_texture

### Steps
```bash
# Clone the repository
git clone https://github.com/ka-afsharian/s-renderer.git
cd s-renderer

# Compile and Run
chmod +x compile.sh
./compile


