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
uniform vec4 SpotLightDirection;
uniform float SpotLightCutOffAngle;


// MVP matrices
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix; 

void main(void)
{
  // Transform vertex position into eye coordinates
  vec3 pos = ((viewMatrix * modelMatrix) * vec4(v_position, 1.0)).xyz;
  
  // Transform vertex normal into eye coordinates
  vec3 N = normalize((viewMatrix * modelMatrix) *vec4(v_normal, 0.0) ).xyz;
  // Unit vector direction from pos to eye
  vec3 E = normalize( -pos );
  // Unit vector direction from pos to light source
  vec3 L = normalize( LightPosition.xyz - pos );
  // Halfway vector
  vec3 H = normalize( L + E );
 
  // Reverse the direction of the unit Spot light direction
  //vec3 S = normalize(vec3(-SpotLightDirection)); 
  vec3 S = normalize(-SpotLightDirection).xyz;

  // Compute terms in the illumination equation
  float Kd = max( dot(L, N), 0.0 );
  vec4 diffuse = Kd * DiffuseProduct;
  vec4 ambient = AmbientProduct; 
  float Ks = pow( max(dot(N, H), 0.0), Shininess );
  vec4 specular = Ks * SpecularProduct;

  color = (ambient + diffuse + specular);

  float intensity = 0.0;
  // inside the cone?
  if (dot(S, L) > cos(radians(SpotLightCutOffAngle))) 
  {
    // if 90 degrees: 0 intensity
    // intensity increases as angle decresses to 0
    intensity = max(dot(N,L), 0.0);
    if (intensity > 0.0) 
    {
      // discard the specular highlight if the light's behind the vertex
      if( dot(L, N) < 0.0 )  specular = vec4(0.0, 0.0, 0.0, 1.0);
      color += intensity * (ambient + diffuse + specular);
    }
  }

  texCoord = v_texCoord;

  vec4 v = vec4(v_position, 1.0);
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  
}
