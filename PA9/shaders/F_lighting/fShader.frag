#version 330

in vec2 texCoord;
// per-fragment interpolated values from the vertex shader
in vec3 fN;
in vec3 fL;
in vec3 fE;

out vec4 frag_color;

// Light and Material Properties
uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct; 
uniform vec4 SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

uniform sampler2D gSampler;

void main(void)
{
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL); //surface to light

	vec3 H = normalize( L + E );   
	vec4 ambient = AmbientProduct;

	float Kd = max(dot(L, N), 0.0);
    vec4 diffuse = Kd*DiffuseProduct;
    
    float Ks = pow(max(dot(N, H), 0.0), Shininess);
    vec4 specular = Ks*SpecularProduct;

    // discard the specular highlight if the light's behind the vertex
    if( dot(L, N) < 0.0 ) 
		specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    frag_color = (ambient + diffuse + specular) * texture2D(gSampler, texCoord.st);
    frag_color.a = 1.0;
}