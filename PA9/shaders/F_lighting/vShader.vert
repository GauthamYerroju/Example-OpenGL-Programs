#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;
layout (location = 2) in vec3 v_normal;
          
out vec2 texCoord;
// output values that will be interpolatated per-fragment
out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform vec4 LightPosition;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix; 


void main(void)
{
  // Transform vertex position into view coordinates
  vec3 pos = ((viewMatrix * modelMatrix) * vec4(v_position, 1.0)).xyz;

  // Transform vertex normal into view coordinates
  fN = ((viewMatrix * modelMatrix) *vec4(v_normal, 0.0)).xyz;
  fE = -pos;
     
  if( LightPosition.w != 0.0 ) 
  {
    //point light
    //fL = LightPosition.xyz - v_position.xyz;
    fL = LightPosition.xyz - pos.xyz;
  }
  else
  {
    fL = LightPosition.xyz;
  }

  vec4 v = vec4(v_position, 1.0);
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  texCoord = v_texCoord;
}
