#pragma once
#include <vector>





	class norender {

	private:
		int  mode = 2;
		int screenwidth = 0;
		int screenheight = 0;

	public:
		float movementSpeed = 1.0f;
		float deltaTime = 1 / 120.0f;
		int createWindow(int width, int height, const char* name,int vsync);
		void closeWindow();
		bool WindowOpen();
		void processinputs();
		void swapbuffers();
		void clearscreen(float r,float g,float b);
		void init();
		void setup2d();
		void setup3d();
		void setupCamera();
		void updateCamera();
	
		float camposx();
		float camposy();
		float camposz();
		int getMode() { return mode; }
		int getscreenwidth() { return screenwidth; }
		int getscreenheight() { return screenheight; }


	
		
		
	};
	struct Trianglevertex2d {
		float x, y, size, r, g, b, rotation;
	};
	struct circlevertex2d {
		float x, y, size, r, g, b;
	};
	struct quadvertex2d {
		float x, y, width, height, r, g, b,rotation;
	};
	struct chainpoint2d { float x, y; };

	class Render2d {
	public:

		

		void drawtriangle(float x, float y, float r, float g, float b, float size,float rotation);
		void drawcircle(float x, float y, float r, float g, float b, float size);
		void drawquad(float x, float y, float r, float g, float b, float width, float height,float rotation);
		void drawtriangleinstanced(std::vector<Trianglevertex2d>& instances);
		void drawcircleinstanced(std::vector<circlevertex2d>& instances);
		void drawQuadinstanced(std::vector<quadvertex2d>& instances);
		void drawline(float x1, float y1, float x2, float y2, float r, float g, float b);
		void drawlinechain(std::vector<chainpoint2d>& points, float r, float g, float b);
	};

	class Render3d {
	public:
		void drawtriangle3d(float x, float y, float z,
			float size,
			float rotX, float rotY, float rotZ,
			float r, float g, float b);

		void drawQuad3d(float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3,
			float x4, float y4, float z4,
			float r, float g, float b);
	};

	

	

	
	inline Render3d render3d;
	inline norender noRender;
	inline Render2d render2d;

	
		