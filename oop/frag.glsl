#version 430 core
#define PI 3.1415926535897932384626433832795
out vec4 color;
  
in vec2 fragPos;
in vec3 direction;

uniform float aspectRatio = 1280./1024.;
uniform sampler2D screenTexture;
uniform sampler2D skysphere;

void main()
{ 
    color = texture(screenTexture, fragPos);
    if(color.a==1.)
    {
        float ca = acos(direction.z/length(direction.xz)*sign(direction.x))/PI;
        float cb = 1.-(direction.y/2.+0.5);
        color = texture(skysphere, vec2(ca, cb))*1.1;  
        //color = vec4(0.,0.,1.,1.);
    }
    //FragColor = vec4(0.6,0.3,0.8,1.);
}