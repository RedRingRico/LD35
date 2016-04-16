#version 150

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_WorldMatrix;

in vec3 v_Position;
in vec2 v_ST;

out vec2 f_TexCoord;
out vec4 f_WorldPosition;

void main( )
{
	f_TexCoord = v_ST;
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * vec4( v_Position, 1.0 );
	f_WorldPosition = u_WorldMatrix * vec4( v_Position, 1.0 );
}

