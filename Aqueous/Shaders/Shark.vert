#version 150
in vec3 Position;
in vec3 Normal;
in vec3 Color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 vColor;

void main()
{
	gl_Position = Projection * View * Model * vec4(Position, 1.0);
	vColor = Color;
}
