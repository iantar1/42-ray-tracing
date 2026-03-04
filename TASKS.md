# miniRT / Ray Tracing — Task log

This file tracks what’s already implemented in this repo and what’s still pending.

> Update rule: when you finish something, tick it here and (optionally) drop a short note / commit hash.

## ✅ Done (implemented in this repo)

### Build & project setup
- [x] `Makefile` builds `minilibx-linux` and links X11 libs
- [x] `install` target for common Linux deps

### Windowing / display (MiniLibX)
- [x] Initialize MLX, create window, create image buffer
- [x] Write pixels directly into the MLX image buffer
- [x] Display the rendered image via `mlx_put_image_to_window`
- [x] Handle window close event (X button)

### Camera
- [x] Basic camera struct/class (position + direction + FOV)
- [x] Generate rays per pixel (`Camera::getRay`)
- [x] Camera movement with keyboard: `W/A/S/D`

### Ray/object intersection
- [x] Sphere intersection (`intersect_sphere`) for a single sphere

### Shading / lighting
- [x] Compute sphere normal at hit point
- [x] Lambert/diffuse grayscale shading using dot(normal, lightDir)
- [x] Movable light source (keyboard arrows)

### Rendering
- [x] Per-pixel render loop over `IMG_WIDTH` × `IMG_HEIGHT`
- [x] Background color when no hit

## 🚧 Remaining (next tasks)

### Project spec: .rt scene parsing (mandatory in miniRT)
- [ ] Parse a `.rt` file from argv (reject missing/extra args)
- [ ] Strict format validation + clear error messages
- [ ] Support required entities:
  - [ ] Ambient light: `A <ratio> <R,G,B>`
  - [ ] Camera: `C <pos> <dir> <fov>`
  - [ ] Light: `L <pos> <brightness> <R,G,B>`
  - [ ] Sphere: `sp <pos> <diameter> <R,G,B>`
  - [ ] Plane: `pl <pos> <normal> <R,G,B>`
  - [ ] Cylinder: `cy <pos> <axis> <diameter> <height> <R,G,B>`
- [ ] Range checks (normalized vectors, FOV 0–180, colors 0–255, ratios 0–1, etc.)

### Geometry
- [ ] Support multiple objects in a scene (iterate + closest hit)
- [ ] Plane intersection
- [ ] Cylinder intersection (+ caps if required by your interpretation)
- [ ] Surface normal for plane/cylinder

### Materials / color
- [ ] Per-object RGB color (not just grayscale)
- [ ] Ambient + diffuse light contribution (use ambient entity)
- [ ] Specular highlight (if you want, not always mandatory)
- [ ] Shadows (occlusion test between hit point and light)

### Controls / UX
- [ ] Clean exit: free MLX resources / destroy image & window
- [ ] Optional: camera rotation (look left/right/up/down)
- [ ] Optional: redraw throttling / continuous loop hook

### Code health
- [ ] Stop including `.cpp` files in `main.cpp` (compile via Makefile instead)
- [ ] Split headers/impl (`Scene.hpp` currently contains implementation)
- [ ] Warnings: enable `-Wall -Wextra -Werror` and fix issues

### Bonus ideas (optional)
- [ ] Multiple lights
- [ ] Reflections (recursive rays)
- [ ] Anti-aliasing / supersampling
- [ ] Textures / checkerboard
- [ ] BVH / acceleration structure
- [ ] Multithreading
