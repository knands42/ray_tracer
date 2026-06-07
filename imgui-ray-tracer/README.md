# Ray Tracer

## Running the project

### Pre-requirements

- CMake
- OpenGL
- Git

```shell
sudo pacman -S cmake mesa glu
```

### Clone

```shell
git clone --recurse-submodules git@github.com:knands42/ray-tracing
cd ray-tracing
```

### Build

```shell
cmake -B build && cmake --build build --target run
```

Or just create an alias:

```shell
alias run='cmake -B build && cmake --build build --target run'
```