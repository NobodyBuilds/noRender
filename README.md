# noRender

no render is a library built on opengl with a goal to simplify the window creation,drawcalls for basic shapes ,easy for prototyping

## Build on Windows

Install Visual Studio Build Tools with the C++ workload, or MinGW-w64, then run this from the repository directory:

```bat
.\build.bat
```

The script detects the installed compiler and creates a static library in `build\` (`noRender.lib` with MSVC, or `libnoRender.a` with MinGW/Clang). It downloads GLM 1.0.2 on the first build if GLM is not already available in the repository.
