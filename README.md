# noRender

no render is a library built on opengl with a goal to simplify the window creation,drawcalls for basic shapes ,easy for prototyping

## Build on Windows

Install Visual Studio Build Tools with the C++ workload, or MinGW-w64, then run this from the repository directory:

```bat
.\build.bat
```

The script detects the installed compiler, downloads GLM 1.0.2 and GLFW 3.4, and downloads the GLAD 1 generator to produce an OpenGL 4.6 core loader. Dependencies are cached under `build\_deps\`. Python 3 and an internet connection are needed on the first build. The static library is written to `build\` (`noRender.lib` with MSVC, or `libnoRender.a` with MinGW/Clang).
