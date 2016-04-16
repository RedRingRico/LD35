#version 150

in vec3 f_Colour;
out vec4 o_FinalColour;

void main( )
{
	o_FinalColour = vec4( f_Colour, 1.0 );
}

