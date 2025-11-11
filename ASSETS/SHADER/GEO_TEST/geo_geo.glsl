#version 460 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

void main()
{
	// Emit the original trinagle
	for(int i = 0;i<3;i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();

	// Emit offset triangle
	for(int i = 0;i<3;i++)
	{
		gl_Position = gl_in[i].gl_Position + vec4(0.3, 0.0, 0.0, 0.0);
		EmitVertex();
	}
	EndPrimitive();
}