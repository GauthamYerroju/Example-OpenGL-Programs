#version 330

in vec2 texCoord;
in vec4 color;

out vec4 frag_color;

uniform sampler2D gSampler;
          
void main(void)
{
	frag_color = color * texture2D(gSampler, texCoord.st);
	frag_color.a = 1.0;
             
}
