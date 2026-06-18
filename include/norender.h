#pragma once


	class norender {
	public:

		/**
 input witdh in pixels, height in pixels, window name as string, vsync as int (1 for on, 0 for off)
*/
		int createWindow(int width, int height, const char* name,int vsync);

		/** use this to close the window*/
		void closeWindow();

		bool WindowOpen();
	
	};

	
	inline norender render;