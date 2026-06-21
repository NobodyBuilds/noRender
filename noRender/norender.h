#pragma once
#include <vector>





	class norender {

	private:
		int  mode = 2;
		int screenwidth = 0;
		int screenheight = 0;

	public:

	
		int createWindow(int width, int height, const char* name,int vsync);
		void closeWindow();
		bool WindowOpen();
		void processinputs();
		void swapbuffers();
		void clearscreen(float r,float g,float b);
		void init();
		void setup2d();
		void setup3d();

		
	

		int getMode() { return mode; }
		int getscreenwidth() { return screenwidth; }
		int getscreenheight() { return screenheight; }


	
		
		
	};

	class Render2d {
	public:

		struct Trianglevertex {
			float x, y, size, r, g, b;
		};
		struct circlevertex {
			float x, y, size, r, g, b;
		};
		struct quadvertex {
			float x, y, width, height, r, g, b;
		};
		struct chainpoint { float x, y; };

		void drawtriangle(float x, float y, float r, float g, float b, float size);
		void drawcircle(float x, float y, float r, float g, float b, float size);
		void drawquad(float x, float y, float r, float g, float b, float width, float height);
		void drawtriangleinstanced(std::vector<Trianglevertex>& instances);
		void drawcircleinstanced(std::vector<circlevertex>& instances);
		void drawQuadinstanced(std::vector<quadvertex>& instances);
		void drawline(float x1, float y1, float x2, float y2, float r, float g, float b);
		void drawlinechain(std::vector<chainpoint>& points, float r, float g, float b);
	};

	class render3d {
		//3d rendering functions
	};

	

	

	
	
	inline norender noRender;
	inline Render2d render2d;

	
		