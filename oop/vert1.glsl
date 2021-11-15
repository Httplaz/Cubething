#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in float normale;
layout (location = 2) in vec2 texCoord;

uniform mat4 rotation;

out vec2 fragPos;
out vec3 direction;

uniform float aspectRatio = 1280./1024.;

void main()
{
    gl_Position = vec4(position, 1.0f);
    //fragPos = position.xy*sin(35.*0.0174533) / cos(35.*0.0174533);
    fragPos = position.xy;
    //fragPos.x*=aspectRatio;
    //vec3 camPos = vec3(0.,0.,-1.);
    //direction = (vec4(normalize(vec3(fragPos.xy, 0)-camPos), 0.)*rotation).xyz;
    //direction = (vec4(normalize(vec3(fragPos.x*aspectRatio, fragPos.y, 0.) - camPos), 1.)*rotation).xyz;
}