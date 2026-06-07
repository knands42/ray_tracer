# Ray Tracing Workspace

A curated collection of ray tracing implementations developed for educational purposes.

## Projects

- **[Simple Ray Tracer](./simple-ray-tracer/README.md)**  
  A CLI-based implementation following the "Ray Tracing in One Weekend" series. Optimized for performance and core
  algorithm understanding.

- **[ImGui Ray Tracer](./imgui-ray-tracer/README.md)**  
  An interactive, real-time-focused renderer inspired by The Cherno's engine series. Features a GUI-based interface for
  live parameter tweaking.

## Showcase

### Simple Ray Tracer

![Simple Ray Tracer](./simple-ray-tracer/assets/image.png)

### ImGui Ray Tracer

![ImGui Ray Tracer](./imgui-ray-tracer/assets/image.png)

---

## Workspace Structure

The project uses a modular CMake architecture. You can build all components from the root directory:

```bash
cmake -B build
cmake --build build
```

Or follow the READMEs in each sub-directory for more specific instructions.

## Engineering Notes

I maintain a repository of technical findings, optimization strategies, and debugging logs within the `/docs` directory.
This is where I document my process and insights gained while developing these rendering engines.

### About AI

I am relying on AI mostly for exploring mathematical concepts and algorithms when relying on the resources I am using is
not enough. After all, graphics programming and all programming languages in this project (C, Cpp, C#, and Rust) are also
all new to me. 
All the docs in the root project are examples of doubts and answers I got from exploration, either from AI or from
the resources listed below.

## Engineering Principles

- **Modularity:** Each project independent module with its own dependency tree, pick and choose.
- **Reproducibility:** Projects use standard CMake `FetchContent` to manage external assets and libraries (if make
  sense).
- **Documentation:** Every sub-project contains its own technical specifications and build instructions.

## Resources

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [The Cherno's Ray Tracing Series](https://www.youtube.com/watch?v=gfW1Fhd9u9Q&list=PLlrATfBNZ98edc5GshdBtREv5asFW3yXl&index=1)