#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D sprite;
uniform vec3 spriteColor;
in vec4 VertexPosition;

uniform int Lightoff;

uniform vec2 PlayerPosition;

void main()
{
    color = vec4(spriteColor, 1.0) * texture(sprite, TexCoords);
    float distance = length(VertexPosition.xy - PlayerPosition);
    
    if(Lightoff == 1 && distance > 150.0f)
    {
        color = vec4(0.0, 0.0, 0.0, 0.0);
    }
}