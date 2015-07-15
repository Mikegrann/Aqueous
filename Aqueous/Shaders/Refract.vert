#version 150

//const int LIGHT_MAX = 4;

struct SLight
{
	vec3 Color;
	float Radius;
   vec3 Position;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform mat4 Local;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform int uLightCount;
uniform SLight uLights[4];//[LIGHT_MAX];
uniform vec3 uCameraPosition;

out vec2 vTexCoord;
out vec3 vLight[4];//[LIGHT_MAX];
out vec3 vNormal;
out vec4 vScreenPosition;


void main()
{
	vec4 Position = Model * Local * vec4(Position, 1.0);

   int i;
	for (i = 0; (i < 4) && (i < uLightCount); i = i + 1) {
		vLight[i] = uLights[i].Position - vec3(Position);
   }

	vTexCoord = TexCoord;
	vNormal = Normal;

	gl_Position = Projection * View * Position;
	vScreenPosition = gl_Position;
}
