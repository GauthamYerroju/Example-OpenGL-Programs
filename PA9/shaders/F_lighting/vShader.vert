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
  fN = v_normal;
  fE = v_position.xyz;
  fL = LightPosition.xyz;
    
  if( LightPosition.w != 0.0 ) 
  {
    fL = LightPosition.xyz - v_position.xyz;
  }

  vec4 v = vec4(v_position, 1.0);
  gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * v;
  texCoord = v_texCoord;
}
