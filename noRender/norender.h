#pragma once
#include <vector>



struct Trianglevertex {
	float x, y, size, r, g, b;
};
struct circlevertex {
	float x, y, size, r, g, b;
};
struct quadvertex {
	float x, y,  width, height ,r, g, b;
};
struct chainpoint { float x, y; };


	class norender {
	public:

		/**
 input witdh in pixels, height in pixels, window name as string, vsync as int (1 for on, 0 for off)
*/
		int createWindow(int width, int height, const char* name,int vsync);

		/** use this to close the window*/
		void closeWindow();
		/**
		use it for while loop for rendering
		*/
		bool WindowOpen();

		/**
		add this inside the while loop tyo process inputs like mouse,resize etc*/
		void processinputs();

		/**
		help with window swap buffer ,add this after the rendering in the loop
		*/
		void swapbuffers();

		/**
		clears the previous frame
		*/
		void clearscreen();


		void drawtriangle(float x, float y, float r, float g, float b, float size);
		void drawcircle(float x, float y, float r, float g, float b, float size);
		void drawquad(float x, float y, float r, float g, float b, float width,float height);
		//instanced renders
		void drawtriangleinstanced(std::vector<Trianglevertex>& instances);

		void drawcircleinstanced(std::vector<circlevertex>& instances);

		void drawQuadinstanced(std::vector<quadvertex>& instances);

		void drawline(float x1, float y1, float x2, float y2, float r, float g, float b);
		void drawlinechain(std::vector<chainpoint>& points, float r, float g, float b);
		/**
		initializes renders components use it after window creation
		*/
		void init();
		
	};
	struct data {
		int screenwidth = 0;
		int screeheight = 0;

	};

	

	
	inline data d;
	inline norender render;

	
		