#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in vec3 v_normal;

out vec4 color;
out vec2 texCoord;

// Material Properties
uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct; 
uniform vec4 SpecularProduct;
uniform float Shininess;

// Light Properties
uniform vec4 LightPosition;


// MVP matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix; 

void main(void)
{
  // Transform vertex position into eye coordinates
  vec3 pos = ((viewMatrix * modelMatrix) * vec4(v_position, 1.0)).xyz;
  
  vec3 L = vec3(0);
  if( LightPosition.w != 0.0 )
  {
    // point light
    // Unit vector direction from pos to light source
    L = normalize( LightPosition.xyz - pos );
  }
  else
  {
    // Unit vector to light source
    L = normalize(LightPosition.xyz);
  }
  
  // Unit vector direction from pos to eye
  vec3 E = normalize( -pos );
  // Halfway vector
  vec3 H = normalize( L + E );
  // Transform vertex normal into eye coordinates
  vec3 N = normalize((viewMatrix * modelMatrix) *vec4(v_normal, 0.0) ).xyz;

  // Compute terms in the illumination equation
  float Kd = max( dot(L, N), 0.0 );
  vec4  diffuse = Kd * DiffuseProduct;
  float Ks = pow( max(dot(N, H), 0.0), Shininess );
  vec4  specular = Ks * SpecularProduct;
  if( dot(L, N) < 0.0 )  specular = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 ambient = AmbientProduct; 
  

  color = ambient + diffuse + specular;
  color.a = 1.0;

  texCoord = v_texCoord;

  vec4 v = vec4(v_position, 1.0);
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  
}
