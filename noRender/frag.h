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

inline const char* meshFrag = R"glsl(
#version 330 core
in vec3 ourcolor;
in vec4 vColor;
in vec4 vMaterial;
in vec3 fragNormal;
in vec3 fragWorldPos;

uniform int uUseModelColor;
uniform int uShowNormal;
uniform int uAlphaPass;
uniform vec3 uCameraPos;

out vec4 fragColor;

void main(){
    vec3 norm = normalize(fragNormal);
    
    if (uShowNormal == 1) {
        fragColor = vec4(norm * 0.5 + 0.5, 1.0);
        return;
    }

    vec4 baseColor = (uUseModelColor == 1) ? vColor : vec4(ourcolor, 1.0);
    float metallic = (uUseModelColor == 1) ? clamp(vMaterial.x, 0.0, 1.0) : 0.0;
    float roughness = (uUseModelColor == 1) ? clamp(vMaterial.y, 0.04, 1.0) : 0.55;
    float alphaCutoff = (uUseModelColor == 1) ? vMaterial.z : 0.5;
    float alphaMode = (uUseModelColor == 1) ? vMaterial.w : 0.0;
    
    if (alphaMode > 0.5 && alphaMode < 1.5 && baseColor.a < alphaCutoff) {
        discard;
    }

    bool blended = alphaMode > 1.5 && baseColor.a < 0.999;
    if (uAlphaPass == 0 && blended) {
        discard;
    }
    if (uAlphaPass == 1 && !blended) {
        discard;
    }

    vec3 albedo = clamp(baseColor.rgb, vec3(0.0), vec3(1.0));
    vec3 lightDir = normalize(vec3(0.45, 0.85, 0.35));
    vec3 viewDir = normalize(uCameraPos - fragWorldPos);
    vec3 halfDir = normalize(lightDir + viewDir);

    float ndotl = max(dot(norm, lightDir), 0.0);
    float ndotv = max(dot(norm, viewDir), 0.0);
    float ndoth = max(dot(norm, halfDir), 0.0);
    float shininess = mix(96.0, 12.0, roughness);
    float spec = pow(ndoth, shininess) * mix(1.1, 0.35, roughness);
    float fresnel = pow(1.0 - ndotv, 5.0);

    vec3 specColor = mix(vec3(0.04), albedo, metallic);
    vec3 diffuse = albedo * (0.16 + 0.82 * ndotl) * (1.0 - metallic * 0.86);
    vec3 reflectionTint = vec3(0.55, 0.68, 0.82) * (0.22 + 0.78 * max(norm.y, 0.0));
    vec3 color = diffuse
        + specColor * (spec * mix(0.55, 1.45, metallic) + fresnel * mix(0.08, 0.35, metallic))
        + reflectionTint * specColor * mix(0.04, 0.32, metallic) * (1.0 - roughness);

    if (blended) {
        color += vec3(0.18, 0.26, 0.34) * fresnel;
    }

    color = pow(clamp(color, vec3(0.0), vec3(1.0)), vec3(1.0 / 2.2));
    fragColor = vec4(color, baseColor.a);
}
)glsl";
