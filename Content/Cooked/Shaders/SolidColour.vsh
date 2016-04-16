#version 150

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_WorldMatrix;

in vec3 v_Position;
in vec3 v_Colour;

out vec3 f_Colour;

void main( )
{
	f_Colour = v_Colour;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * vec4( v_Position, 1.0 );
}

