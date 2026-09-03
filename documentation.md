# noRender Documentation

**noRender** is a lightweight C++ rendering library built on OpenGL 3.3 (GLFW + GLAD + GLM). It removes all the boilerplate so you can create a window and start drawing shapes in a few lines of code.

---

## Requirements

| What | Version |
|---|---|
| OS | Windows 10/11 x64 |
| IDE | Visual Studio 2022 (C++20) |
| GPU | Any GPU with OpenGL 3.3 support |
| CUDA | Optional — only needed for GPU interop |

All dependencies (GLFW 3.4, GLAD, GLM 1.0.2, stb_image) are already bundled in the repo.

---

## Visual Studio Setup

1. Open `noRender.sln` in Visual Studio 2022.
2. If making your own project, set these in **Project Properties → C/C++ → General → Additional Include Directories**:
   ```
   $(SolutionDir)include;
   $(SolutionDir)glad\include;
   $(SolutionDir)glfw-3.4.bin.WIN64\include;
   $(SolutionDir)glm-1.0.2;
   ```
3. **Linker → General → Additional Library Directories**:
   ```
   $(SolutionDir)glfw-3.4.bin.WIN64\lib-vc2022;
   ```
4. **Linker → Input → Additional Dependencies**:
   ```
   opengl32.lib
   glfw3.lib
   ```
5. Add these source files to your project:
   - `glad/src/glad.c`
   - `noRender/main.cpp`
   - `noRender/drawcalls2d.cpp`
   - `noRender/draw3d.cpp`

6. Set C++ Language Standard to **C++20** (`/std:c++20`).

---

## Basics: How noRender Works

There are two global objects you use everywhere:

- **`noRender`** — handles the window, input, camera, and configuration
- **`render`** — draws all shapes (2D and 3D)

Colors are **floats from 0.0 to 1.0** (not 0-255).  
2D coordinates are in **pixels** — origin (0,0) is at the **bottom-left** of the window.

---

## 1. Your First Program — Window to Rendering to Close

This is the complete lifecycle of a noRender app. Read every comment:

```cpp
#include "norender.h"

int main()
{
    // ─── STEP 1: Create the window ───
    // Args: width (pixels), height (pixels), window title, vsync (1=on, 0=off)
    // Returns 0 on success, -1 on failure
    noRender.createWindow(1280, 720, "My App", 1);

    // ─── STEP 2: Choose 2D or 3D mode ───
    noRender.setup2D();  // enables alpha blending for transparent shapes
    // OR
    // noRender.setup3D();  // enables depth testing for 3D

    // ─── STEP 3: The render loop — runs every frame ───
    while (noRender.isWindowOpen())
    {
        // 3a. Poll keyboard/mouse events (must call every frame)
        noRender.pollEvents();

        // 3b. Clear the screen with a background color (R, G, B)
        noRender.clearScreen(0.1f, 0.1f, 0.15f);

        // 3c. Draw your shapes here
        render.circle(640, 360, 1.0f, 0.0f, 0.0f, 80);  // red circle at center

        // 3d. Present the frame to screen (must call every frame)
        noRender.swapBuffers();
    }

    // ─── STEP 4: Clean up ───
    noRender.closeWindow();
    return 0;
}
```

That's it. Every noRender app follows this pattern:
**createWindow → setup mode → loop (poll → clear → draw → swap) → closeWindow**

---

## 2. Drawing 2D Shapes

All 2D functions live on the `render` object. Position is in **pixels**, color is **0.0–1.0**.

### Triangle

```cpp
// render.triangle(x, y, r, g, b, size, rotation_degrees)
render.triangle(400, 300, 0.0f, 1.0f, 0.0f, 50, 0);     // green triangle
render.triangle(600, 300, 1.0f, 0.5f, 0.0f, 50, 45);    // orange, rotated 45°
```

### Circle

```cpp
// render.circle(x, y, r, g, b, diameter)
render.circle(640, 360, 0.0f, 0.5f, 1.0f, 100);  // blue circle, 100px wide
```

### Quad (Rectangle)

```cpp
// render.quad(x, y, r, g, b, width, height, rotation_degrees)
render.quad(640, 360, 1.0f, 1.0f, 1.0f, 200, 100, 0);   // white rectangle
render.quad(640, 360, 1.0f, 0.0f, 0.0f, 50, 50, 30);    // red square, rotated 30°
```

### Line

```cpp
// render.line(x1, y1, x2, y2, r, g, b)
render.line(100, 100, 500, 400, 1.0f, 1.0f, 0.0f);  // yellow diagonal line
```

### Complete 2D Example

```cpp
#include "norender.h"

int main()
{
    noRender.createWindow(1280, 720, "2D Shapes", 1);
    noRender.setup2D();

    while (noRender.isWindowOpen())
    {
        noRender.pollEvents();
        noRender.clearScreen(0.05f, 0.05f, 0.08f);

        // draw a scene
        render.quad(640, 100, 0.3f, 0.8f, 0.3f, 1280, 200, 0);  // green ground
        render.circle(200, 500, 1.0f, 1.0f, 0.0f, 120);          // yellow sun
        render.triangle(640, 450, 0.8f, 0.2f, 0.2f, 80, 0);      // red roof
        render.quad(640, 320, 0.6f, 0.4f, 0.2f, 120, 180, 0);    // brown house
        render.line(0, 200, 1280, 200, 0.0f, 0.5f, 0.0f);        // horizon line

        noRender.swapBuffers();
    }

    noRender.closeWindow();
}
```

---

## 3. Drawing 3D Shapes

Switch to 3D mode with `setup3D()`. 3D coordinates are in **world space** (not pixels).

### 3D Triangle

```cpp
// render.triangle3D(x, y, z, size, rotationX_deg, rotationY_deg, r, g, b)
render.triangle3D(0, 0, 0, 2.0f, 0, 0, 1.0f, 0.0f, 0.0f);   // red triangle at origin
render.triangle3D(5, 0, -3, 1.5f, 45, 30, 0.0f, 1.0f, 0.0f); // green, rotated
```

### 3D Quad (4 Vertices)

```cpp
// render.quad3D(x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4, r,g,b)
// Define a flat ground plane
render.quad3D(
    -10, 0, -10,   // vertex 1
     10, 0, -10,   // vertex 2
     10, 0,  10,   // vertex 3
    -10, 0,  10,   // vertex 4
    0.2f, 0.6f, 0.2f  // green color
);
```

### Complete 3D Example with Camera

```cpp
#include "norender.h"

int main()
{
    noRender.createWindow(1280, 720, "3D World", 1);
    noRender.setup3D();

    // Initialize camera — sets up mouse/keyboard controls
    // Camera starts at position (0, 5, 10) looking at origin
    noRender.setupCamera();

    // Tweak camera speed
    noRender.movementSpeed = 5.0f;
    noRender.deltaTime = 1.0f / 60.0f;
    noRender.fov = 70.0f;

    while (noRender.isWindowOpen())
    {
        noRender.pollEvents();
        noRender.updateCamera();  // must call every frame for camera to work
        noRender.clearScreen(0.1f, 0.1f, 0.2f);

        // ground plane
        render.quad3D(
            -20, 0, -20,   20, 0, -20,
             20, 0,  20,  -20, 0,  20,
            0.2f, 0.5f, 0.2f
        );

        // some triangles floating in space
        render.triangle3D(0, 2, 0, 1.5f, 0, 0, 1.0f, 0.0f, 0.0f);
        render.triangle3D(5, 3, -5, 1.0f, 45, 90, 0.0f, 0.0f, 1.0f);
        render.triangle3D(-4, 1, 3, 2.0f, 0, 60, 1.0f, 1.0f, 0.0f);

        noRender.swapBuffers();
    }

    noRender.closeWindow();
}
```

**Camera controls:**
- **WASD** — move forward/back/left/right
- **Q / E** — move up / down
- **Left click + drag** — look around
- **Scroll wheel** — zoom (changes FOV)

---

## 4. Batch / Instanced Rendering (Drawing Thousands of Shapes)

Drawing shapes one by one is fine for small scenes, but if you need **thousands of shapes**, use batch functions. They draw everything in one GPU call.

### Data Structs

Each shape type has a struct you fill in:

```cpp
// For triangleBatch
struct Trianglevertex2d { float x, y, size, r, g, b, rotation; };

// For circleBatch
struct circlevertex2d { float x, y, size, r, g, b; };

// For quadBatch
struct quadvertex2d { float x, y, width, height, r, g, b, rotation; };

// For lineBatch
struct linepoint2d { float ox, oy, dx, dy, r, g, b; };  // ox,oy = start, dx,dy = end

// For triangleBatch3D
struct triangle3d { float x, y, z, size, rotX, rotY, r, g, b; };
```

### Batch Example — 10,000 Particles

```cpp
#include "norender.h"
#include <vector>
#include <cstdlib>

int main()
{
    noRender.createWindow(1280, 720, "Batch Particles", 1);
    noRender.setup2D();

    // Create 10,000 circles with random positions and colors
    const int COUNT = 10000;
    std::vector<circlevertex2d> particles(COUNT);

    for (auto& p : particles)
    {
        p.x    = (float)(rand() % 1280);
        p.y    = (float)(rand() % 720);
        p.size = 4.0f + (rand() % 10);
        p.r    = (float)rand() / RAND_MAX;
        p.g    = (float)rand() / RAND_MAX;
        p.b    = 1.0f;
    }

    while (noRender.isWindowOpen())
    {
        noRender.pollEvents();
        noRender.clearScreen(0.0f, 0.0f, 0.0f);

        // Update positions
        for (auto& p : particles)
        {
            p.y += 0.5f;
            if (p.y > 720) p.y = 0;
        }

        // ONE call draws all 10,000 circles
        render.circleBatch(particles);

        noRender.swapBuffers();
    }

    noRender.closeWindow();
}
```

You can do the same with `render.triangleBatch(...)`, `render.quadBatch(...)`, `render.lineBatch(...)`, and `render.triangleBatch3D(...)`.

---

## 5. Sprites and Texture Quads

### Loading and Drawing a Sprite

```cpp
#include "norender.h"

int main()
{
    noRender.createWindow(1280, 720, "Sprites", 1);
    noRender.setup2D();

    // Load an image (PNG, JPG, BMP supported)
    spriteData player = noRender.loadSprite("assets/player.png");

    while (noRender.isWindowOpen())
    {
        noRender.pollEvents();
        noRender.clearScreen(0.1f, 0.1f, 0.1f);

        // render.sprite(spriteData, x, y, size)
        render.sprite(player, 640, 360, 128);  // draw at center, 128px

        noRender.swapBuffers();
    }

    noRender.closeWindow();
}
```

### Texture Quad — CPU Pixel Buffer to Screen

Use this for raytracers, heatmaps, cellular automata, etc. You fill a vector of pixels and noRender uploads it to the GPU as a texture:

```cpp
#include "norender.h"
#include <vector>

int main()
{
    noRender.createWindow(1280, 720, "Pixel Canvas", 1);
    noRender.setup2D();

    const int texW = 256, texH = 256;
    std::vector<quadtexture2d> canvas(texW * texH);

    while (noRender.isWindowOpen())
    {
        noRender.pollEvents();
        noRender.clearScreen(0.0f, 0.0f, 0.0f);

        // Fill pixels (gradient pattern)
        for (int y = 0; y < texH; y++)
        {
            for (int x = 0; x < texW; x++)
            {
                canvas[y * texW + x] = {
                    (float)x / texW,    // r
                    (float)y / texH,    // g
                    0.5f,               // b
                    1.0f                // opacity
                };
            }
        }

        // render.textureQuad(pixels, posX, posY, displayWidth, displayHeight, pixelW, pixelH)
        render.textureQuad(canvas, 640, 360, 512, 512, texW, texH);

        noRender.swapBuffers();
    }

    noRender.closeWindow();
}
```

---

## 6. CUDA / GPU Interop (Complete Guide)

This is the most powerful feature of noRender. Instead of computing data on the GPU, copying it back to CPU, and uploading it again to OpenGL — you write **directly** into the OpenGL buffer from your CUDA kernel. Zero copies.

### How It Works

1. You ask noRender for the raw OpenGL **VBO ID** (via `vbo_id`)
2. You register that VBO with CUDA (`cudaGraphicsGLRegisterBuffer`)
3. Each frame: **map** the buffer → **run your kernel** (writes positions/colors directly) → **unmap**
4. Call the `*Interop` draw function — it renders straight from GPU memory

### Slot IDs

noRender keeps up to `max_interop = 10` buffer slots (defined in `renderdata.h`).
- **Slot 0** — reserved for CPU batch calls (`circleBatch`, `quadBatch`, etc.)
- **Slots 1–9** — free for your CUDA/compute interop buffers

> **Always use slot ID ≥ 1 for interop.** Slot 0 will conflict with CPU batch uploads.

### Interop Draw Functions

```cpp
render.quadBatchInterop(int count, int slotId);     // draw quads from GPU buffer
render.circleBatchInterop(int count, int slotId);    // draw circles from GPU buffer
render.lineBatchInterop(int count, int slotId);      // draw lines from GPU buffer
render.textureQuadInterop(posX, posY, w, h, pixW, pixH, slotId);  // draw texture from GPU
```

### VBO ID Getters

```cpp
vbo_id.quad_instanced_vbo(count, slotId);      // returns OpenGL VBO ID for quads
vbo_id.circle_instanced_vbo(count, slotId);    // returns OpenGL VBO ID for circles
vbo_id.line_instanced_vbo(count, slotId);      // returns OpenGL VBO ID for lines
vbo_id.quad_texture_tex(pixW, pixH, slotId);   // returns OpenGL Texture ID
```

### Complete CUDA Interop Example — 100K Particles

This is a complete, working example with every step explained. You need a `.cu` file for this.

#### `kernel.cu` — CUDA side

```cpp
#include "norender.h"
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>

// ─────────────────────────────────────────────────
//  CUDA kernel — runs on GPU, writes directly into
//  the OpenGL VBO memory. Each thread handles one particle.
// ─────────────────────────────────────────────────
__global__ void updateParticlesKernel(quadvertex2d* particles, int count,
                                      float dt, int screenW, int screenH)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= count) return;

    // Simple physics: gravity + bounce
    // NOTE: quadvertex2d layout is { x, y, width, height, r, g, b, rotation }
    //       we store velocity in rotation (hack) for this demo
    float vy = particles[i].rotation;  // reusing rotation as velocity storage
    vy -= 100.0f * dt;                 // gravity
    particles[i].y += vy * dt;

    // Bounce off bottom
    if (particles[i].y < particles[i].height * 0.5f)
    {
        particles[i].y = particles[i].height * 0.5f;
        vy = -vy * 0.8f;  // energy loss on bounce
    }

    particles[i].rotation = vy;  // store velocity back

    // Color based on speed
    float speed = fabsf(vy);
    particles[i].r = fminf(speed / 500.0f, 1.0f);
    particles[i].g = 0.3f;
    particles[i].b = 1.0f - fminf(speed / 500.0f, 1.0f);
}

// ─────────────────────────────────────────────────
//  Initialization kernel — sets random starting positions
// ─────────────────────────────────────────────────
__global__ void initParticlesKernel(quadvertex2d* particles, int count,
                                     int screenW, int screenH, unsigned int seed)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= count) return;

    // Simple random using thread index
    unsigned int hash = (i * 1103515245 + seed) & 0x7FFFFFFF;

    particles[i].x        = (float)(hash % screenW);
    particles[i].y        = (float)((hash / screenW) % screenH);
    particles[i].width    = 4.0f;
    particles[i].height   = 4.0f;
    particles[i].r        = 0.5f;
    particles[i].g        = 0.5f;
    particles[i].b        = 1.0f;
    particles[i].rotation = 0.0f;  // initial velocity = 0
}

// ─────────────────────────────────────────────────
//  INTEROP SETUP AND RENDER — called from main()
// ─────────────────────────────────────────────────

static cudaGraphicsResource_t cudaVboResource = nullptr;

static const int PARTICLE_COUNT = 100000;
static const int SLOT_ID = 1;        // must be >= 1, slot 0 is reserved

void setupCudaInterop()
{
    // ── STEP 1: Get the OpenGL VBO ID from noRender ──
    // This also allocates the buffer on the GPU for `count` quads
    unsigned int vboId = vbo_id.quad_instanced_vbo(PARTICLE_COUNT, SLOT_ID);

    // ── STEP 2: Register the OpenGL VBO with CUDA ──
    // cudaGraphicsRegisterFlagsWriteDiscard = we will only WRITE to it from CUDA
    cudaGraphicsGLRegisterBuffer(
        &cudaVboResource,           // output: CUDA resource handle
        vboId,                      // input: OpenGL VBO ID from step 1
        cudaGraphicsRegisterFlagsWriteDiscard
    );

    // ── STEP 3: Initialize particles on GPU ──
    // Map the buffer so CUDA can access it
    cudaGraphicsMapResources(1, &cudaVboResource, 0);

    // Get the GPU pointer to write to
    quadvertex2d* devPtr = nullptr;
    size_t bufferSize = 0;
    cudaGraphicsResourceGetMappedPointer(
        (void**)&devPtr,   // output: GPU pointer you can write to
        &bufferSize,       // output: size of the buffer in bytes
        cudaVboResource    // input: the mapped resource
    );

    // Launch init kernel
    int threads = 256;
    int blocks = (PARTICLE_COUNT + threads - 1) / threads;
    initParticlesKernel<<<blocks, threads>>>(devPtr, PARTICLE_COUNT, 1280, 720, 42);
    cudaDeviceSynchronize();

    // Unmap — gives the buffer back to OpenGL
    cudaGraphicsUnmapResources(1, &cudaVboResource, 0);
}

void updateAndRenderCuda()
{
    // ── STEP 4: Map buffer for CUDA (every frame) ──
    cudaGraphicsMapResources(1, &cudaVboResource, 0);

    quadvertex2d* devPtr = nullptr;
    size_t bufferSize = 0;
    cudaGraphicsResourceGetMappedPointer((void**)&devPtr, &bufferSize, cudaVboResource);

    // ── STEP 5: Run physics kernel ──
    int threads = 256;
    int blocks = (PARTICLE_COUNT + threads - 1) / threads;
    updateParticlesKernel<<<blocks, threads>>>(
        devPtr,
        PARTICLE_COUNT,
        noRender.deltaTime,
        noRender.getScreenWidth(),
        noRender.getScreenHeight()
    );
    cudaDeviceSynchronize();

    // ── STEP 6: Unmap — gives buffer back to OpenGL ──
    cudaGraphicsUnmapResources(1, &cudaVboResource, 0);

    // ── STEP 7: Draw! noRender reads directly from GPU memory ──
    // No data upload, no CPU copy — just draw
    render.quadBatchInterop(PARTICLE_COUNT, SLOT_ID);
}

void cleanupCuda()
{
    if (cudaVboResource)
    {
        cudaGraphicsUnregisterResource(cudaVboResource);
        cudaVboResource = nullptr;
    }
}
```

#### `main.cpp` — Application side

```cpp
#include "norender.h"

// Declared in kernel.cu
extern void setupCudaInterop();
extern void updateAndRenderCuda();
extern void cleanupCuda();

int main()
{
    noRender.createWindow(1280, 720, "CUDA Interop - 100K Particles", 0);  // vsync off for max FPS
    noRender.setup2D();
    noRender.deltaTime = 1.0f / 120.0f;

    // Register the OpenGL buffer with CUDA
    setupCudaInterop();

    while (noRender.isWindowOpen())
    {
        noRender.pollEvents();
        noRender.clearScreen(0.02f, 0.02f, 0.05f);

        // Physics + render in one call — all on GPU, no CPU copies
        updateAndRenderCuda();

        noRender.swapBuffers();
    }

    cleanupCuda();
    noRender.closeWindow();
}
```

#### Visual Studio Setup for CUDA

Add to your project properties:
- **Linker → Input**: add `cudart_static.lib`
- Right-click your `.cu` file → **Properties → Item Type** → set to **CUDA C/C++**
- Make sure NVIDIA CUDA Toolkit is installed

### Texture Interop (CUDA writes pixels to screen)

Same concept, but for textures instead of vertex buffers:

```cpp
#include <cuda_gl_interop.h>

static cudaGraphicsResource_t cudaTexResource = nullptr;
const int TEX_W = 512, TEX_H = 512;
const int TEX_SLOT = 1;

void setupTextureInterop()
{
    // Get OpenGL texture ID from noRender
    unsigned int texId = vbo_id.quad_texture_tex(TEX_W, TEX_H, TEX_SLOT);

    // Register with CUDA
    cudaGraphicsGLRegisterImage(
        &cudaTexResource, texId, GL_TEXTURE_2D,
        cudaGraphicsRegisterFlagsSurfaceLoadStore
    );
}

// In your render loop:
void renderFromCuda()
{
    cudaGraphicsMapResources(1, &cudaTexResource, 0);

    // Get CUDA array from the mapped texture
    cudaArray_t cudaArr;
    cudaGraphicsSubResourceGetMappedArray(&cudaArr, cudaTexResource, 0, 0);

    // Bind to a CUDA surface and write pixels from your kernel
    // ... your kernel writes to the surface ...

    cudaGraphicsUnmapResources(1, &cudaTexResource, 0);

    // Draw the texture to screen — no CPU data needed
    render.textureQuadInterop(640, 360, 1280, 720, TEX_W, TEX_H, TEX_SLOT);
}
```

---

## 7. VBO Access for Custom Use

The `vbo_id` global gives you raw OpenGL buffer/texture IDs. Calling any getter also ensures the buffer is allocated.

```cpp
unsigned int id;

// Single shape VBOs
id = vbo_id.triangle_vbo();       // triangle buffer
id = vbo_id.circle_vbo();         // circle buffer
id = vbo_id.quad_vbo();           // quad buffer
id = vbo_id.line_vbo();           // line buffer

// Instanced VBOs (count = how many instances, slot = 1-9 for interop)
id = vbo_id.triangle_instanced_vbo(count, slot);
id = vbo_id.circle_instanced_vbo(count, slot);
id = vbo_id.quad_instanced_vbo(count, slot);
id = vbo_id.line_instanced_vbo(count, slot);

// Texture ID
id = vbo_id.quad_texture_tex(pixelWidth, pixelHeight, slot);
```

---

## 8. Configuration

| Variable | Default | What it does |
|---|---|---|
| `noRender.movementSpeed` | `1.0` | Camera move speed (3D) |
| `noRender.deltaTime` | `1/120.0` | Frame timestep |
| `noRender.fov` | `60.0` | Field of view degrees (clamped 15–120) |
| `mouseSensitivity` | `0.15` | Mouse look sensitivity (global) |
| `scrollSensitivity` | `2.0` | Scroll zoom sensitivity (global) |
| `max_interop` | `10` | Number of interop buffer slots |

---

## API Quick Reference

### `noRender` (window & system)

```cpp
int createWindow(int w, int h, const char* title, int vsync);  // make window
void closeWindow();                                              // destroy window
bool isWindowOpen();                                             // check if still open
void pollEvents();                                               // process input
void swapBuffers();                                              // present frame
void clearScreen(float r, float g, float b);                     // clear background
void init();                                                     // init internals
void setup2D();                                                  // 2D mode
void setup3D();                                                  // 3D mode
void setupCamera();                                              // init 3D camera
void updateCamera();                                             // update camera per frame
float getCameraX();                                              // camera position
float getCameraY();
float getCameraZ();
int getMode();                                                   // 2=2D, 3=3D
int getScreenWidth();
int getScreenHeight();
void setScreenSize(float w, float h);
GLFWwindow* getWindowHandle();                                   // raw GLFW pointer
void setInputBlocked(bool blocked);                              // block camera input
bool isInputBlocked();
spriteData loadSprite(const char* path);                         // load image
```

### `render` (drawing)

```cpp
// ── 2D single shapes ──
void triangle(x, y, r, g, b, size, rotation);
void circle(x, y, r, g, b, diameter);
void quad(x, y, r, g, b, width, height, rotation);
void line(x1, y1, x2, y2, r, g, b);

// ── 2D batch (instanced) ──
void triangleBatch(vector<Trianglevertex2d>& data);
void circleBatch(vector<circlevertex2d>& data);
void quadBatch(vector<quadvertex2d>& data);
void lineBatch(vector<linepoint2d>& data);

// ── 2D interop (GPU buffer, no CPU upload) ──
void quadBatchInterop(int count, int slotId);
void circleBatchInterop(int count, int slotId);
void lineBatchInterop(int count, int slotId);

// ── Textures & sprites ──
void textureQuad(vector<quadtexture2d>& pixels, posX, posY, w, h, pixW, pixH);
void textureQuadInterop(posX, posY, w, h, pixW, pixH, slotId);
void sprite(spriteData& spr, posX, posY, size);

// ── 3D shapes ──
void triangle3D(x, y, z, size, rotX, rotY, r, g, b);
void quad3D(x1,y1,z1, x2,y2,z2, x3,y3,z3, x4,y4,z4, r,g,b);
void triangleBatch3D(vector<triangle3d>& data);
```

---

## Troubleshooting

| Problem | Solution |
|---|---|
| `ERROR: use render3D class instead of render2D` | You called a 2D draw function while in 3D mode. Use `setup2D()` or switch to 3D draw functions. |
| Shapes appear at wrong position | Remember: (0,0) is **bottom-left** in 2D, not top-left. |
| Colors look wrong | Colors are **0.0–1.0** floats, not 0–255 integers. |
| Interop draws nothing | Make sure you use slot `id >= 1`. Slot 0 is reserved for CPU batches. |
| Sprite loads but shows nothing | Check the file path. Use forward slashes or escaped backslashes in paths. |
| Camera doesn't move | Make sure you call both `setupCamera()` **and** `updateCamera()` every frame. |
