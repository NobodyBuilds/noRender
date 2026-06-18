#pragma once


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
	};

	
	inline norender render;