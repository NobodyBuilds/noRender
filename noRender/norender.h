#pragma once
#include <vector>


struct GLFWwindow;


struct Trianglevertex2d {
	float x, y, size, r, g, b, rotation;
};
struct circlevertex2d {
	float x, y, size, r, g, b;
};
struct quadvertex2d {
	float x, y, width, height, r, g, b, rotation;
};
struct linepoint2d { float ox, oy, dx, dy, r, g, b; };
struct quadtexture2d { float r, g, b, opacity; };
struct spriteData { std::vector<quadtexture2d> data; int width, height; };
struct triangle3d { float x, y, z, size, rotX, rotY, r, g, b; };
	class norender {

	private:
		int  mode = 2;
		int screenwidth = 0;
		int screenheight = 0;
		bool blockedInput = false;
	public:
		float movementSpeed = 1.0f;
		float deltaTime = 1 / 120.0f;
		float fov = 60.0f;

		// Creates a GLFW window with the given pixel dimensions, title, and vsync toggle (1=on, 0=off)
		int createWindow(int width, int height, const char* name,int vsync);

		// Destroys the window and terminates GLFW
		void closeWindow();

		// Returns true while the window should remain open
		bool isWindowOpen();

		// Polls all pending input events (call once per frame)
		void pollEvents();

		// Swaps front and back framebuffers (call at end of frame)
		void swapBuffers();

		// Clears the screen to the given RGB color (0.0 – 1.0)
		void clearScreen(float r,float g,float b);

		// Initializes the library internals
		void init();

		// Switches to 2D rendering mode (enables alpha blending)
		void setup2D();

		// Switches to 3D rendering mode (enables depth testing)
		void setup3D();

		// Initializes the 3D camera and registers mouse/scroll callbacks
		void setupCamera();

		// Updates camera position from keyboard input each frame
		void updateCamera();
	
		// Returns the camera X world-position
		float getCameraX();

		// Returns the camera Y world-position
		float getCameraY();

		// Returns the camera Z world-position
		float getCameraZ();

		// Returns current mode: 2 = 2D, 3 = 3D
		int getMode() { return mode; }

		// Returns the current window width in pixels
		int getScreenWidth() { return screenwidth; }

		// Returns the current window height in pixels
		int getScreenHeight() { return screenheight; }

		// Updates the stored screen dimensions (called on resize)
		void setScreenSize(float w ,float h);

		// Returns the raw GLFW window pointer
		GLFWwindow* getWindowHandle();

		// Blocks or unblocks camera input processing
		void setInputBlocked(bool blocked);

		// Returns true if camera input is currently blocked
		bool isInputBlocked() { return blockedInput; }

		// Loads an image file and returns it as sprite pixel data
		 spriteData loadSprite(const char* address);
		
	};
	

	// Unified rendering class for both 2D and 3D draw calls
	class Render {
	public:

		// ── 2D draw calls ──

		// Draws a single 2D triangle at (x,y) with RGB color, size, and rotation in degrees
		void triangle(float x, float y, float r, float g, float b, float size,float rotation);

		// Draws a single 2D circle at (x,y) with RGB color and diameter
		void circle(float x, float y, float r, float g, float b, float size);

		// Draws a single 2D quad at (x,y) with RGB color, width, height, and rotation in degrees
		void quad(float x, float y, float r, float g, float b, float width, float height,float rotation);

		// Draws many 2D triangles in one instanced draw call
		void triangleBatch(std::vector<Trianglevertex2d>& instances);

		// Draws many 2D circles in one instanced draw call
		void circleBatch(std::vector<circlevertex2d>& instances);

		// Draws many 2D quads in one instanced draw call
		void quadBatch(std::vector<quadvertex2d>& instances);

		// Draws instanced quads using a pre-filled GPU buffer (interop, no CPU upload)
		void quadBatchInterop(int count,int id);

		// Draws a single 2D line from (x1,y1) to (x2,y2) with RGB color
		void line(float x1, float y1, float x2, float y2, float r, float g, float b);

		// Draws many 2D lines in one instanced draw call
		void lineBatch(std::vector<linepoint2d>& points);

		// Draws instanced lines using a pre-filled GPU buffer (interop, no CPU upload)
		void lineBatchInterop(int count,int id);

		// Draws instanced circles using a pre-filled GPU buffer (interop, no CPU upload)
		void circleBatchInterop(int count, int id);

		// Renders a CPU pixel buffer as a textured quad on screen
		void textureQuad(std::vector<quadtexture2d>& quadscreen,float positonX,float positonY,float quadWidth,float quadHeight, int pixelX, int pixelY);

		// Renders a textured quad from a pre-filled GPU texture (interop, no CPU upload)
		void textureQuadInterop(float positonX,float positonY,float quadWidth,float quadHeight, int pixelX, int pixelY,int id);

		// Draws a loaded sprite at (posX,posY) scaled to the given size
		void sprite(spriteData& sprite, float positonX, float positonY, float size);

		// Reserved for raw 2D triangle access (advanced)
		void rawTriangles2D();

		// ── 3D draw calls ──

		// Draws a single 3D triangle at (x,y,z) with size, rotation (degrees), and RGB color
		void triangle3D(float x, float y, float z,
			float size,
			float rotX, float rotY,
			float r, float g, float b);

		// Draws a 3D quad defined by four corner vertices with RGB color
		void quad3D(float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3,
			float x4, float y4, float z4,
			float r, float g, float b);

		// Draws many 3D triangles in one instanced draw call
		void triangleBatch3D(std::vector<triangle3d>& instances);

		// Reserved for raw 3D triangle access (advanced)
		void rawTriangles3D();
	};

	

	inline bool firstMouse = true;
	inline double lastMouseX = 0, lastMouseY = 0;
	inline bool cameraRotating = false;

	inline float mouseSensitivity = 0.15f;
	inline float scrollSensitivity = 2.0f;

	
	inline Render render;
	inline norender noRender;
	
	
	struct vboids {
		unsigned int triangle_vbo() const;
		unsigned int triangle_instanced_vbo(int count,int id) const;
		unsigned int circle_vbo() const;
		unsigned int circle_instanced_vbo(int count,int id) const;
		unsigned int quad_vbo() const;
		unsigned int quad_instanced_vbo(int count,int id) const;
		unsigned int line_vbo() const;
		unsigned int line_instanced_vbo(int count,int id) const;
		unsigned int quad_texture_tex(int pixelwidth,int pixelheight, int id) const;

	};
	inline vboids vbo_id;
