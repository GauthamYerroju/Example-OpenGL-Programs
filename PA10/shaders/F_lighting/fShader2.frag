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
uniform float Shininess;

// Light Properties
uniform vec4 LightPosition;
uniform vec4 SpotLightDirection;
uniform float SpotLightCutOffAngle;

uniform sampler2D gSampler;

void main(void)
{
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	vec3 H = normalize( L + E ); 

	// Reverse the direction of the unit Spot light direction
  	vec3 S = normalize(-SpotLightDirection).xyz; 

  	float intensity = 0.0;
  	vec4  specular = vec4(0.0, 0.0, 0.0, 1.0);  

    // inside the cone?
  	if (dot(S,L) > cos(radians(SpotLightCutOffAngle))) 
  	{
    	// if 90 degrees: 0 intensity
    	// intensity increases as angle decresses to 0
    	intensity = max(dot(N,L), 0.0);  
 
    	if (intensity > 0.0) 
    	{
      		float Ks = pow( max(dot(N, H), 0.0), Shininess );
      		specular = Ks * SpecularProduct;
      		// discard the specular highlight if the light's behind the vertex
      		if( dot(L, N) < 0.0 )  specular = vec4(0.0, 0.0, 0.0, 1.0);

    	}
  	}
   
	// Compute terms in the illumination equation
  	float Kd = max( dot(L, N), 0.0 );
 	vec4  diffuse = Kd * DiffuseProduct;
 	vec4 ambient = AmbientProduct;  

    frag_color = max(intensity * diffuse + specular, ambient) * texture2D(gSampler, texCoord.st);
    frag_color.a = 1.0;
}