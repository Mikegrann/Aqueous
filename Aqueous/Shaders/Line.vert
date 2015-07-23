attribute vec3 vPosition;
attribute vec3 vColor;
uniform mat4 Projection;
uniform mat4 Model;
uniform mat4 View;

varying vec3 color;

void main()
{
   color = vColor;
	gl_Position = Projection * View * Model * vec4(vPosition, 1.0);

   // move positions and normals into camera space
   // cam_vertPos = MV*vertPos;
}
