![mezera gituhub 2](https://github.com/user-attachments/assets/93cf5260-d60b-400f-9e86-8c35d2fa6bfc)

# Mezera Engine

Welcome to the Mezera Engine source code!

**Mezera Engine** is a Vulkan-based experimental 3D engine designed for real-time rendering, geometry exploration, and learning low-level graphics programming.

It is a personal project focused on building a custom rendering pipeline from scratch, with emphasis on clarity, minimalism, and control over every part of the process.

---
## Build
```bash
mkdir build
cd build
cmake ..
make
./JoyApp
```
To use glTF files in a scene upload them to the resources/gltfModels folder and use in Scene.cpp :
```cpp
auto [vertices, indices] = LoadGLTFMesh_All("../resources/gltfModels/scene.gltf");
```

---
## Status

> Mezera Engine is in active development. It is not production-ready.

Current capabilities include:

- Parametric surface rendering (e.g. Klein bottle, torus, etc.)
    
- Support for custom geometry and mesh management
    
- Real-time camera movement and interaction
    
- Basic shading and color pipelines
    
- glTF model loading

---

## Features (in progress)

- Vulkan-based rendering architecture
    
- Mesh abstraction system (`Mesh` class)
    
- Interactive camera with mouse-based movement
    
- Support for parametric surface generation
    
- glTF 2.0 model import using TinyGLTF

Planned:

- Lighting (Phong, PBR)
    
- Ray tracing prototype
    
- UI overlay with stats

___
## Documentation

A detailed PDF document explaining the mathematical foundations and rendering pipeline of Mezera Engine is available in the `docs/` folder.

📄 Mezera Engine_Math.pdf

___
## 📜 License

This project is licensed under the **Mezera Engine License**.  
Use is permitted for **non-commercial and educational purposes only**.  
See LICENSE.txt for details.

---

## 🤝 Contributions

Currently closed to external contributions. Future plans include opening issues and PRs when base systems stabilize.

---

## 📬 Contact

If you have questions or are interested in collaboration or commercial licensing, contact me via GitHub or email.

> romanovinno@gmail.com
