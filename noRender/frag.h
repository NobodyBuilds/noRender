#pragma once


inline const char* trianglefrag= R"glsl(
#version 330 core
out vec4 fragcolor;
in vec3 ourcolor;

void main(){
	fragcolor=vec4(ourcolor,1.0f);
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

inline const char* triangle3dfrag = R"glsl(
#version 330 core
out vec4 fragcolor;
in vec3 ourcolor;

void main(){
fragcolor=vec4(ourcolor,1.0);
}
)glsl";

inline const char* quadfsfrag= R"glsl(

#version 330 core
in vec2 vUV;
out vec4 fragColor;
uniform sampler2D uTex;

void main(){
    fragColor = texture(uTex, vUV);
}

)glsl";