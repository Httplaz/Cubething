#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 screenCoord;

uniform mat4 rotation;

out vec2 fragPos;
out vec3 direction;

float aspectRatio = 1920./1080.;

void main()
{
    gl_Position = vec4(position, 1.0f);
    gl_Position.y = -position.y;
    fragPos = screenCoord*sin(35.*0.0174533) / cos(35.*0.0174533);
    vec3 camPos = vec3(0.,0.,-1.);
    direction = (vec4(normalize(vec3(fragPos.x*aspectRatio, fragPos.y, 0.) - camPos), 1.)*rotation).xyz;
}