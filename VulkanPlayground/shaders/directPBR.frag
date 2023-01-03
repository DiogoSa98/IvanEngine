#version 450

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 WorldPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 fragTexCoord;

// material parameters
//uniform vec3  baseColor;
//uniform float metallic;
//uniform float roughness;
//uniform float ao;
vec3  baseColor = vec3(1., 0., 0.); // diffuse albedo for non-metals and specular color for metals
float metallic = .9;
float reflectance = 0.35; // specular intesity and ior of dieletric materials
float perceptualRoughness = .5;
float ao = .2;
float clearCoat = .9;
float clearCoatPerceptualRoughness = .1;

// lights
//uniform vec3 lightPosition;
//uniform vec3 lightColor;
vec3 lightPosition = vec3(0.5);
vec3 lightColor = vec3(1.);

//uniform vec3 camPos;
vec3 camPos = vec3(3.0, 1.5, 3.0);

const float PI = 3.14159265359;
  
vec3 BRDF(vec3 v, vec3 l, vec3 n, vec3 f0, vec3 diffuseColor);

// Heavily based/copied from
// Google's Filament (https://github.com/google/filament/blob/main/shaders/src/shading_model_standard.fs)
// https://learnopengl.com/PBR/Lighting
void main()
{		
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // TODO cpu side on material 
    vec3 diffuseColor = (1.0 - metallic) * baseColor; 
//    vec3 F0 = vec3(0.04); 
//    F0 = mix(F0, baseColor, metallic);
    // for metalic/dieletric materials 
    vec3 f0 = 0.16 * reflectance * reflectance * (1.0 - metallic) + baseColor * metallic;

    vec3 Lo = vec3(0.0);
//    for(int i = 0; i < 4; ++i) 
//    {
        vec3 L = normalize(lightPosition - WorldPos);

        // calculate per-light radiance
        float Ldist = length(lightPosition - WorldPos);
        float attenuation = 1.0 / (Ldist * Ldist);
        vec3 radiance = lightColor * attenuation;        
        
        vec3 col = BRDF(V, L, N, f0, diffuseColor); // (vec3 v, vec3 l, vec3 n, vec3 f0, vec3 diffuseColor)

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += col * radiance * NdotL;
//    }   
  
    vec3 ambient = vec3(0.005) * baseColor * ao;
    vec3 color = ambient + Lo;
	
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    outColor = vec4(color, 1.0);
}  


float D_GGX(float roughness, float NoH) {
    float a = NoH * roughness;
    float k = roughness / ( (1.0 - NoH * NoH) + a * a );
    float d = k * k * (1.0 / PI);
    return d;
}

vec3 F_Schlick(vec3 f0, float f90, float VoH) {
    return f0 + (f90 - f0) * pow(1.0 - VoH, 5.);
}
vec3 F_Schlick(vec3 f0, float VoH) {
    float f = pow(1.0 - VoH, 5.0);
    return f + f0 * (1.0 - f);
}
float F_Schlick(float f0, float f90, float VoH) {
    return f0 + (f90 - f0) * pow(1.0 - VoH, 5.);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float a) {
    float a2 = a * a;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float Fd_Lambert() {
    return 1.0 / PI;
}

// clearcoat visibility term
float V_Kelemen(float LoH) {
    return 0.25 / (LoH * LoH);
}

vec3 IsotropicLobe(float roughness, vec3 f0, float NoV, float NoL, float NoH, float LoH) {

    float D = D_GGX(roughness, NoH);
    float V = V_SmithGGXCorrelated(roughness, NoV, NoL);
    vec3  F = F_Schlick(f0, LoH);

    return (D * V) * F;
}

vec3 SpecularLobe(float roughness, vec3 f0, float NoV, float NoL, float NoH, float LoH) {
// TODO Anisotropic lobe
//#if defined(MATERIAL_HAS_ANISOTROPY)
//    return AnisotropicLobe(pixel, light, h, NoV, NoL, NoH, LoH);
//#else
    return IsotropicLobe(roughness, f0, NoV, NoL, NoH, LoH);
//#endif
}


float ClearCoatDistribution(float NoH, float LoH, out float F) {
    // TODO cpu side on material 
    // remapping and linearization of clear coat roughness
    clearCoatPerceptualRoughness = clamp(clearCoatPerceptualRoughness, 0.089, 1.0);
    float clearCoatRoughness = clearCoatPerceptualRoughness * clearCoatPerceptualRoughness;

    // TODO
    // If the material has a normal map, we want to use the geometric normal
    // instead to avoid applying the normal map details to the clear coat layer
    
    // clear coat BRDF
    float D = D_GGX(clearCoatRoughness, NoH);
    float V = V_Kelemen(LoH);
    F = F_Schlick(0.04, 1.0, LoH) * clearCoat; // clear coat strength;  air-polyurethane interface has an IOR of 1.5 gives f0 = 0.04
    
    // account for energy loss in the base layer
    return D * V * F;
}


vec3 BRDF(vec3 v, vec3 l, vec3 n, vec3 f0, vec3 diffuseColor) {
    vec3 h = normalize(v + l);

    float NoV = abs(dot(n, v)) + 1e-5;
    float NoL = clamp(dot(n, l), 0.0, 1.0);
    float NoH = clamp(dot(n, h), 0.0, 1.0);
    float LoH = clamp(dot(l, h), 0.0, 1.0);

    // perceptually linear roughness to roughness TODO CPU-side
    float roughness = perceptualRoughness * perceptualRoughness;

    vec3 Fr = SpecularLobe(roughness, f0, NoV, NoL, NoH, LoH);

    // TODO store IBL DGGX r value 
    //vec3 energyCompensation = 1.0 * F0 * (1.0 / dfg.y - 1.0); 
    // scale specular lobe to account for multiscattering
    // Fr *= energyCompensation;

    // diffuse BRDF
    vec3 Fd = diffuseColor * Fd_Lambert(); // TODO try out Disney diffuse
    //Fd *= (1.0 - pixel.transmission); // TODO for refractive materials

    vec3 color = Fr + Fd;

    // TODO if clearcoat
    float F;
    float clearCoat = ClearCoatDistribution(NoH, LoH, F);
    float attenuation = 1.0 - F;
    color *= attenuation;
    color += clearCoat;
    return color;
}

