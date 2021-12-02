#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in float normale;
layout (location = 2) in vec2 texCoord;

uniform vec3 normales[6] = {vec3(0.,0., -1.), vec3(-1., 0., 0.), vec3(0., -1., 0.), vec3(0.,0.,1.), vec3(1.,0.,0.), vec3(0., 1., 0.)};

uniform usampler3D voxels;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out float occlusion;
out float norm;
out vec3 position;
out vec2 uv;

uniform float aspectRatio = 1280./1024.;

vec2 getCube(ivec3 p)
{
    ivec3 chunkPos = ivec3(p.xz%32,0);
    vec2 chunkOffset = texelFetch(voxels, chunkPos, 6).xy;
    p.xz+=ivec2(chunkOffset)*32;
    vec4 g = texelFetch(voxels, p, 0);
    return g.xy;
}

bool cubeExists(ivec3 x)
{
    return getCube(x).x!=0;
}

void main()
{
    vec3 relPos = fract(pos);
    position = pos+relPos;


    occlusion = 1;
    //if(normale==5.)
    {
    vec3 aodir = sign(relPos-vec3(0.25));

    vec3 surfaceNormale = normales[int(normale)];

    vec4 raodir = vec4(reflect(aodir, surfaceNormale), 0.);

    vec3 cubeCenter = pos-relPos+vec3(0.5);
    vec3 AoCenter = pos-relPos+vec3(0.5)+surfaceNormale;
    bool side1 = cubeExists(ivec3(AoCenter+raodir.xww));
    bool side2 = cubeExists(ivec3(AoCenter+raodir.wyw));
    bool side3 = cubeExists(ivec3(AoCenter+raodir.wwz));
    bool corner = cubeExists(ivec3(position+aodir/2.));
    occlusion = 1.-float(cubeExists(ivec3(cubeCenter-surfaceNormale)));
    occlusion = 1.-float(cubeExists(ivec3(AoCenter)));
    occlusion = 1.-(float(side1)+float(side2)+float(side3)+float(corner))/3.;
    }

    //occlusion = 1;

    uv = texCoord;
    gl_Position = projection*view*model*vec4(position.x,position.y,position.z, 1.0f);
    //gl_Position.y*=-1;
    //gl_Position.xyz/=gl_Position.w;
    norm = normale;
}