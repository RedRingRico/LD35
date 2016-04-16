#version 150
#extension GL_ARB_explicit_attrib_location : enable

uniform sampler2D u_Texture;

in vec2 f_TexCoord;
in vec4 f_WorldPosition;

layout ( location = 0 ) out vec4 o_WorldPosition;
layout ( location = 1 ) out vec4 o_Albedo;
layout ( location = 2 ) out vec4 o_TexCoord;

void main( )
{
	o_WorldPosition = f_WorldPosition;
	o_Albedo = texture( u_Texture, f_TexCoord );
	o_TexCoord = vec4( f_TexCoord, 1.0, 1.0 );
}

