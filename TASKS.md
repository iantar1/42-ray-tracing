```markdown
# miniRT — checklist from the subject (v4.1)

This checklist is derived from the **miniRT “Ray Tracer”** subject PDF (March 2026, version 4.1).

Notes from the subject:

- **Mandatory must be perfect** before anything else is evaluated.
- Options/bonuses are only considered if the mandatory part is 100% correct.
- Allowed languages: **C / C++ / Rust**.
- Rendering must be **CPU ray tracing** (no GPU raster pipeline like OpenGL/Metal/Vulkan/DirectX).
- You may use **MiniLibX** (or equivalent) to open a window, draw pixels, display an image, manage events.
- No memory leaks.

> Update rule: when you finish something, tick it and add a short note / commit hash.

## ✅ General requirements (always)

- [x] Executable is named **`rt`**.
- [ ] No memory leaks (valgrind clean when exiting normally).
- [x] Uses CPU ray tracing (no forbidden GPU rendering pipeline).
- [x] Uses allowed libs for mandatory: libc / libstdc++ (or Rust equivalents) + libm + MiniLibX.
- [x] Window opens and displays the rendered image.

## 🧱 Mandatory part (must be flawless)

### Core ray tracer

- [x] Implement ray tracing to generate an image of a 3D scene.
- [x] Scene contains **simple geometric objects** + **light sources**.
- [x] Camera/eye position and direction are easy to change.

### Required primitives (at least these 4)

Implement intersection + surface normals + shading support for:

- [x] Sphere
- [x] Plane
- [x] Cylinder
- [x] Cone

### Transformations

- [x] Objects support **translation** before rendering.
- [x] Objects support **rotation** before rendering.

### Display / performance behavior

- [x] Can **redraw** the view (or part of it) **without recalculating the entire image**.
  - [x] MiniLibX: handle expose event properly (re-put existing image buffer).

### Light management (mandatory expectations)

- [x] Different light brightness is supported.
- [x] Shadows are implemented.
- [ ] Multiple spot lights supported.
- [ ] “Shine effect” implemented (specular highlight / glossy look).

### Mandatory validation scenes (strongly advised by subject)

Reproduce at least these three scenes to ease evaluation:

- [ ] Scene 1: the 4 basic objects + 2 spots + shadows + shine
- [ ] Scene 2: same scene from a different viewpoint
- [ ] Scene 3: shadow mixing

## 🧩 Options (only graded if mandatory is PERFECT)

The subject lists options as examples (not an exhaustive list). Pick a set you can defend live.

Lighting options:

- [x] Ambient light
- [x] Direct light
- [ ] Parallel light

Geometry options:

- [ ] Limited objects (parallelograms, disks, half-spheres, tubes…)
- [ ] Composed elements (cubes, pyramids, tetrahedrons…)
- [ ] More “native” elements (paraboloid, hyperboloid, toroid…)

Material / appearance options:

- [ ] Textures
- [ ] Bump mapping / colour disruption
- [x] Reflection
- [x] Transparency
- [ ] Shadow modification according to transparency
- [ ] Negative elements
- [ ] Limit disruption / transparency / reflection depending on texture

Scene tooling options:

- [ ] External files for scene description
- [ ] In-program live configuration / modifications (for defence)

⚠️ Note from subject:

- If you parse `.pov` / `.3ds`, mandatory objects must still be handled from **equations** (not triangles/vertices).

## ⭐ Bonuses (extra credit)

- [ ] Add outstanding extra features beyond the “options” list.
- [ ] (If used) justify any extra libraries during defence.
- [ ] (Optional) Performance work (e.g., GPU computing via OpenCL/CUDA/compute shaders) — allowed for performance.

## 🧪 Self-check before you push

- [ ] Render is deterministic for the same scene/camera.
- [ ] No crashes on window close / after repeated expose events.
- [ ] Scenes used for evaluation are easy to load/switch during defence.

```
