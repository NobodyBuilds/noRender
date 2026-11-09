@echo off
setlocal EnableExtensions

set "ROOT=%~dp0"
set "OUT=%ROOT%build"
set "DEPS=%OUT%\_deps"
set "COMPILER="

rem Prefer the Visual Studio compiler, including when launched outside its Developer Prompt.
where cl.exe >nul 2>&1
if not errorlevel 1 (
    where lib.exe >nul 2>&1
    if not errorlevel 1 set "COMPILER=msvc"
)

if not defined COMPILER call :load_visual_studio

if not defined COMPILER (
    where clang-cl.exe >nul 2>&1
    if not errorlevel 1 (
        where llvm-lib.exe >nul 2>&1
        if not errorlevel 1 set "COMPILER=clang-cl"
        if not defined COMPILER (
            where lib.exe >nul 2>&1
            if not errorlevel 1 set "COMPILER=clang-cl-msvc-lib"
        )
    )
)

if not defined COMPILER (
    where g++.exe >nul 2>&1
    if not errorlevel 1 (
        where ar.exe >nul 2>&1
        if not errorlevel 1 set "COMPILER=mingw"
    )
)

if not defined COMPILER (
    where clang++.exe >nul 2>&1
    if not errorlevel 1 (
        where llvm-ar.exe >nul 2>&1
        if not errorlevel 1 set "COMPILER=clang"
    )
)

if not defined COMPILER (
    echo ERROR: No supported C++ compiler and archiver were found.
    echo Install Visual Studio Build Tools with the C++ workload or MinGW-w64, then run this script again.
    exit /b 1
)

echo Checking and downloading GLM, GLFW, and GLAD dependencies...
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%ROOT%build-deps.ps1"
if errorlevel 1 exit /b 1

set "GLM_INCLUDE=%DEPS%\glm-1.0.2"
set "GLFW_INCLUDE=%DEPS%\glfw-3.4.bin.WIN64\include"
set "GLAD_INCLUDE=%DEPS%\glad\include"
set "GLAD_SOURCE=%DEPS%\glad\src\glad.c"

if not exist "%GLM_INCLUDE%\glm\glm.hpp" (
    echo ERROR: GLM headers were not found after dependency setup.
    exit /b 1
)
if not exist "%GLFW_INCLUDE%\GLFW\glfw3.h" (
    echo ERROR: GLFW headers were not found after dependency setup.
    exit /b 1
)
if not exist "%GLAD_INCLUDE%\glad\glad.h" (
    echo ERROR: GLAD headers were not found after dependency setup.
    exit /b 1
)
if not exist "%GLAD_SOURCE%" (
    echo ERROR: GLAD source was not generated.
    exit /b 1
)

if not exist "%OUT%" mkdir "%OUT%"

echo Building noRender with %COMPILER%...
if "%COMPILER%"=="msvc" goto build_msvc
if "%COMPILER%"=="clang-cl" goto build_clang_cl
if "%COMPILER%"=="clang-cl-msvc-lib" goto build_clang_cl_msvc_lib
if "%COMPILER%"=="mingw" goto build_mingw
if "%COMPILER%"=="clang" goto build_clang
echo ERROR: Unsupported compiler selection.
exit /b 1

:build_msvc
call :compile_msvc_cpp "%ROOT%noRender\draw3d.cpp" "%OUT%\draw3d.obj" || goto build_failed
call :compile_msvc_cpp "%ROOT%noRender\drawcalls2d.cpp" "%OUT%\drawcalls2d.obj" || goto build_failed
call :compile_msvc_cpp "%ROOT%noRender\main.cpp" "%OUT%\main.obj" || goto build_failed
cl.exe /nologo /MD /c "%GLAD_SOURCE%" /I"%GLAD_INCLUDE%" /Fo"%OUT%\glad.obj"
if errorlevel 1 goto build_failed
lib.exe /nologo /OUT:"%OUT%\noRender.lib" "%OUT%\draw3d.obj" "%OUT%\drawcalls2d.obj" "%OUT%\main.obj" "%OUT%\glad.obj"
if errorlevel 1 goto build_failed
echo Build complete: %OUT%\noRender.lib
exit /b 0

:build_clang_cl
call :compile_clang_cl_cpp "%ROOT%noRender\draw3d.cpp" "%OUT%\draw3d.obj" || goto build_failed
call :compile_clang_cl_cpp "%ROOT%noRender\drawcalls2d.cpp" "%OUT%\drawcalls2d.obj" || goto build_failed
call :compile_clang_cl_cpp "%ROOT%noRender\main.cpp" "%OUT%\main.obj" || goto build_failed
clang-cl.exe /nologo /MD /c "%GLAD_SOURCE%" /I"%GLAD_INCLUDE%" /Fo"%OUT%\glad.obj"
if errorlevel 1 goto build_failed
llvm-lib.exe /nologo /OUT:"%OUT%\noRender.lib" "%OUT%\draw3d.obj" "%OUT%\drawcalls2d.obj" "%OUT%\main.obj" "%OUT%\glad.obj"
if errorlevel 1 goto build_failed
echo Build complete: %OUT%\noRender.lib
exit /b 0

:build_clang_cl_msvc_lib
call :compile_clang_cl_cpp "%ROOT%noRender\draw3d.cpp" "%OUT%\draw3d.obj" || goto build_failed
call :compile_clang_cl_cpp "%ROOT%noRender\drawcalls2d.cpp" "%OUT%\drawcalls2d.obj" || goto build_failed
call :compile_clang_cl_cpp "%ROOT%noRender\main.cpp" "%OUT%\main.obj" || goto build_failed
clang-cl.exe /nologo /MD /c "%GLAD_SOURCE%" /I"%GLAD_INCLUDE%" /Fo"%OUT%\glad.obj"
if errorlevel 1 goto build_failed
lib.exe /nologo /OUT:"%OUT%\noRender.lib" "%OUT%\draw3d.obj" "%OUT%\drawcalls2d.obj" "%OUT%\main.obj" "%OUT%\glad.obj"
if errorlevel 1 goto build_failed
echo Build complete: %OUT%\noRender.lib
exit /b 0

:build_mingw
where gcc.exe >nul 2>&1
if errorlevel 1 (
    echo ERROR: g++ was found, but gcc.exe is needed to compile glad.c.
    goto build_failed
)
call :compile_gnu_cpp g++ "%ROOT%noRender\draw3d.cpp" "%OUT%\draw3d.o" || goto build_failed
call :compile_gnu_cpp g++ "%ROOT%noRender\drawcalls2d.cpp" "%OUT%\drawcalls2d.o" || goto build_failed
call :compile_gnu_cpp g++ "%ROOT%noRender\main.cpp" "%OUT%\main.o" || goto build_failed
gcc.exe -c "%GLAD_SOURCE%" -I"%GLAD_INCLUDE%" -o "%OUT%\glad.o"
if errorlevel 1 goto build_failed
ar.exe rcs "%OUT%\libnoRender.a" "%OUT%\draw3d.o" "%OUT%\drawcalls2d.o" "%OUT%\main.o" "%OUT%\glad.o"
if errorlevel 1 goto build_failed
echo Build complete: %OUT%\libnoRender.a
exit /b 0

:build_clang
where clang.exe >nul 2>&1
if errorlevel 1 (
    echo ERROR: clang++ was found, but clang.exe is needed to compile glad.c.
    goto build_failed
)
call :compile_gnu_cpp clang++ "%ROOT%noRender\draw3d.cpp" "%OUT%\draw3d.o" || goto build_failed
call :compile_gnu_cpp clang++ "%ROOT%noRender\drawcalls2d.cpp" "%OUT%\drawcalls2d.o" || goto build_failed
call :compile_gnu_cpp clang++ "%ROOT%noRender\main.cpp" "%OUT%\main.o" || goto build_failed
clang.exe -c "%GLAD_SOURCE%" -I"%GLAD_INCLUDE%" -o "%OUT%\glad.o"
if errorlevel 1 goto build_failed
llvm-ar.exe rcs "%OUT%\libnoRender.a" "%OUT%\draw3d.o" "%OUT%\drawcalls2d.o" "%OUT%\main.o" "%OUT%\glad.o"
if errorlevel 1 goto build_failed
echo Build complete: %OUT%\libnoRender.a
exit /b 0

:compile_msvc_cpp
cl.exe /nologo /std:c++20 /EHsc /MD /c "%~1" /I"%GLFW_INCLUDE%" /I"%GLAD_INCLUDE%" /I"%GLM_INCLUDE%" /Fo"%~2"
exit /b %errorlevel%

:compile_clang_cl_cpp
clang-cl.exe /nologo /std:c++20 /EHsc /MD /c "%~1" /I"%GLFW_INCLUDE%" /I"%GLAD_INCLUDE%" /I"%GLM_INCLUDE%" /Fo"%~2"
exit /b %errorlevel%

:compile_gnu_cpp
%~1 -std=c++20 -c "%~2" -I"%GLFW_INCLUDE%" -I"%GLAD_INCLUDE%" -I"%GLM_INCLUDE%" -o "%~3"
exit /b %errorlevel%

:load_visual_studio
set "PF86=%ProgramFiles(x86)%"
if not defined PF86 exit /b 0
set "VSWHERE=%PF86%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" exit /b 0
for /f "usebackq tokens=*" %%V in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSINSTALL=%%V"
if not defined VSINSTALL exit /b 0
if not exist "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" exit /b 0
call "%VSINSTALL%\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul
where cl.exe >nul 2>&1
if errorlevel 1 exit /b 0
where lib.exe >nul 2>&1
if not errorlevel 1 set "COMPILER=msvc"
exit /b 0

:build_failed
echo Build failed. Review the compiler output above.
exit /b 1
