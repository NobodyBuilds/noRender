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
   vec2 l =aLocal * (iSize*0.5f);
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
    vec2 world = iPos + aLocal * (iSize*0.5f);
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
layout(location=0) in float aT;
layout(location=1) in vec2  iStart;
layout(location=2) in vec2  iEnd;
layout(location=3) in vec3  iColor;
out vec3 ourcolor;

uniform vec2 screenSize;

void main(){
 vec2 world = mix(iStart, iEnd, aT);
    gl_Position = vec4(
        world.x / (screenSize.x * 0.5) - 1.0,
        world.y / (screenSize.y * 0.5) - 1.0,
        0.0, 1.0
    );
    ourcolor = iColor;
}
)glsl";

inline const char *triangle3dvert = R"glsl(

#version 330 core
layout(location = 0) in vec3 aPos;
uniform vec3 uPos;
uniform mat4 uproj;
uniform mat4 uview;
uniform vec4 rot;
uniform vec3 uColor;
out vec3 ourcolor;

vec3 rotate_by_quat(vec3 v, vec4 q) {
    
   
    vec3 t = 2.0 * cross(q.xyz, v);
    return v + q.w * t + cross(q.xyz, t);
}

void main() {
    vec3 rotated = rotate_by_quat(aPos, rot);
    vec3 worldPos = rotated + uPos;
    gl_Position = uproj * uview * vec4(worldPos, 1.0);
    ourcolor = uColor;
}
)glsl";

inline const  char* quadfsVert= R"glsl(

#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec2 aUV;
uniform vec2 uPosition;
uniform vec2 uSize;
uniform vec2 screenSize;

out vec2 vUV;

void main(){
    vec2 world = uPosition + aPos * (uSize * 0.5);
    gl_Position = vec4(
        world.x / (screenSize.x * 0.5) - 1.0,
        world.y / (screenSize.y * 0.5) - 1.0,
        0.0, 1.0
    );
    vUV = aUV;
}


)glsl";


inline const  char* tri3dinstvert = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 iPos;
layout(location = 2) in vec2 iRot;
layout(location = 3) in float size;
layout(location = 4) in vec3 iColor;
uniform mat4 vp;
out vec3 ourcolor;

// quaternion multiply: a * b
vec4 quatMul(vec4 a, vec4 b) {
    return vec4(
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    );
}

// rotate vector v by quaternion q (q must be normalized)
vec3 quatRotate(vec4 q, vec3 v) {
    vec3 u = q.xyz;
    float s = q.w;
    return 2.0 * dot(u, v) * u
         + (s * s - dot(u, u)) * v
         + 2.0 * s * cross(u, v);
}

// build quat from axis-angle
vec4 quatFromAxisAngle(vec3 axis, float angle) {
    float h = angle * 0.5;
    return vec4(axis * sin(h), cos(h));
}

// conjugate (inverse for unit quats)
vec4 quatConj(vec4 q) {
    return vec4(-q.xyz, q.w);
}
vec4 qx = quatFromAxisAngle(vec3(1,0,0), iRot.x);
vec4 qy = quatFromAxisAngle(vec3(0,1,0), iRot.y);
vec4 rot = quatMul(qy, qx);


void main(){
    vec3 rotated= quatRotate(rot, aPos*size);
    vec3 worldpos=rotated + iPos;
    gl_Position = vp * vec4(worldpos, 1.0);
    ourcolor = iColor;

}


)glsl";