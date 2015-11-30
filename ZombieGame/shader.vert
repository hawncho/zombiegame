#version 450

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 TexCoord;

out vec2 TexCoord0;
out vec2 WorldPos;

uniform mat4 WVP;
uniform mat4 World;
uniform vec2 FrameSize;
uniform ivec2 FramePos;

void main()
{
	gl_Position = WVP * vec4(Position, 0.0f, 1.0f);

	TexCoord0 = (FramePos * FrameSize) + (TexCoord * FrameSize);
	WorldPos = (World * vec4(Position, 0.0f, 1.0f)).xy;
}