#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float time;

void main()
{
    color = vec4(fragmentColor.r * (cos(fragmentPosition.x + time) + 1.0) * 0.5, fragmentColor.g * (cos(fragmentPosition.y + time) + 1.0) * 0.5, fragmentColor.g * (cos(fragmentPosition.x + time) + 1.0) * 0.5, fragmentColor.a);
}
