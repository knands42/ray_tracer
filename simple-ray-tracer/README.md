# Ray Tracer

## Showcase

<img src="assets/image.png" alt="Showcase" />

## Running the project

### Pre-requirements

- CMake
- ImageMagick

Install them with your package manager.
```shell
# Arch Linux
sudo pacman -S cmake imagemagick
```

### Running

```shell
cmake -B build && cmake --build build && ./build/ray_tracer > image.ppm && convert image.ppm assets/image.png
```

Or just create an alias:

```shell
alias run='cmake -B build && cmake --build build && ./build/ray_tracer > image.ppm && convert image.ppm assets/image.png'
```
