#pragma once


inline const char* trianglefrag= R"glsl(
#version 330 core
out vec4 fragcolor;
in vec3 ourcolor;

void main(){
fragcolor=vec4(ourcolor,1.0);
}
)glsl";

inline const char* circlefrag= R"glsl(
#version 330 core
out vec4 fragcolor;
in vec3 ourcolor;
in vec2 offset;

void main(){
float r2=dot(offset,offset);
if(r2>1.0) discard;

fragcolor=vec4(ourcolor,1.0);
}
)glsl";