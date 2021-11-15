#version 330 core
out vec4 FragColor;
  
in vec2 fragPos;

uniform sampler2D texAtlas;

void main()
{ 
    FragColor = texture(texAtlas, fragPos);
    FragColor = vec4(0.3,0.3,0.8,1.);
}