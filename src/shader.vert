#version 330
 
in vec3 a_vertex;

uniform mat4 u_view;
uniform mat4 u_model;
uniform mat4 u_projection;

in vec2 a_uv;
out vec2 v_uv;

in vec3 a_normal;
out vec3 v_normal;
out vec3 v_vertex;


void main()
{
	// position of the vertex
	gl_Position = u_projection * u_view * u_model * vec4( a_vertex , 1.0 );

	//pass the cordinates to the fragment shader
	v_uv = a_uv;
	
	v_vertex = ( u_model * vec4( a_vertex , 1.0) ).xyz;

	mat4 normalMatrix = transpose(inverse(u_model));

	v_normal = (normalMatrix * vec4(a_normal, 1.0)).xyz;
}

