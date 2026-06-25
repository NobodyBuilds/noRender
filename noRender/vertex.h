#pragma once

inline const char *trianglevert = R"glsl(
#version 330 core
layout(location=0) in vec2 apos;
uniform vec3 col;

out vec3 ourcolor;
void main(){
    gl_Position=vec4(apos,0.0f,1.0f);
    ourcolor=col;
}
)glsl";
inline const char *circlevert = R"glsl(
#version 330 core
layout(location=0) in vec2 aPos;

layout(location=2) in vec2 inoffset;
uniform vec3 col;
out vec3 ourcolor;
out vec2 offset;
void main(){
	gl_Position = vec4(aPos,0.0,1.0);
	ourcolor=col;
	offset=inoffset;
}
)glsl";

inline const char *triInstancedVert = R"glsl(
#version 330 core
layout(location=0) in vec2 aLocal;   
layout(location=1) in vec2  iPos;    
layout(location=2) in float iSize;   
layout(location=3) in vec3  iColor;
layout(location=4) in float  rot;


out vec3 ourcolor;
uniform vec2 screenSize;

void main(){
        
  
    float radian = rot * (3.14159265 / 180.0);
    float c = cos(radian);
    float S = sin(radian);

    
   vec2 local =vec2(aLocal.x *c - aLocal.y *S,aLocal.x *S + aLocal.y *c);


    vec2 world = iPos + local * iSize;
    gl_Position = vec4(
        world.x / (screenSize.x * 0.5) - 1.0,
        world.y / (screenSize.y * 0.5) - 1.0,
        0.0, 1.0
    );
    ourcolor = iColor;
}
)glsl";
inline const char *quadInstancedVert = R"glsl(
#version 330 core
layout(location=0) in vec2 aLocal;   
layout(location=1) in vec2  iPos;    
layout(location=2) in vec2 iSize;   
layout(location=3) in vec3  iColor;
layout(location=4) in float  rot;
out vec3 ourcolor;
uniform vec2 screenSize;

void main(){
   vec2 l =aLocal * iSize;
    float radian = rot * (3.14159265 / 180.0);
    float c = cos(radian);
    float S = sin(radian);

    
   vec2 local =vec2(l.x *c - l.y *S,l.x *S + l.y *c);


    vec2 world = iPos + local;
    gl_Position = vec4(
        world.x / (screenSize.x * 0.5) - 1.0,
        world.y / (screenSize.y * 0.5) - 1.0,
        0.0, 1.0
    );
    ourcolor = iColor;
}
)glsl";

inline const char *circleInstancedVert = R"glsl(
#version 330 core
layout(location=0) in vec2 aLocal;  
layout(location=1) in vec2  iPos;
layout(location=2) in float iSize;
layout(location=3) in vec3  iColor;

out vec3 ourcolor;
out vec2 offset;
uniform vec2 screenSize;

void main(){
    vec2 world = iPos + aLocal * iSize;
    gl_Position = vec4(
        world.x / (screenSize.x * 0.5) - 1.0,
        world.y / (screenSize.y * 0.5) - 1.0,
        0.0, 1.0
    );
    ourcolor = iColor;
    offset = aLocal;  
}
)glsl";

inline const char *chainVert = R"glsl(
#version 330 core
layout(location=0) in vec2 aPos;
out vec3 ourcolor;
uniform vec3 linecolor;
uniform vec2 screenSize;

void main(){
    gl_Position = vec4(
        aPos.x / (screenSize.x * 0.5) - 1.0,
        aPos.y / (screenSize.y * 0.5) - 1.0,
        0.0, 1.0
    );
    ourcolor = linecolor;
}
)glsl";

inline const char *triangle3dvert = R"glsl(

#version 330 core
layout(location = 0) in vec3 aPos;
uniform mat4 mvp;
uniform vec3 uColor;
out vec3 ourcolor;
void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    ourcolor = uColor;
}
)glsl";