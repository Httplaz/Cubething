#version 430 core 
in vec2 fragPos;
in vec3 direction;
out vec4 color;

uniform bool advancedGraphics;
uniform float time;
uniform sampler2D texAtlas;
uniform vec3 origin;
uniform ivec3 selectedCube;
uniform float selectedCubeNormale;
uniform usampler3D voxels;

uniform vec3 normales[6] = {vec3(0.,0., -1.), vec3(-1., 0., 0.), vec3(0., -1., 0.), vec3(0.,0.,1.), vec3(1.,0.,0.), vec3(0., 1., 0.)};
uniform int extents[5] = {37449,4861,585,73,9};

float tilesetSize = 128.;
float renderDistanceSteps = 50.;
float renderDistanceMeters = 300.;

vec3 lightPos = vec3(500,260,500);


float cubeSize(vec3 pos)
{
    return max(32.-float(texelFetch(voxels, ivec3(pos)/32, 5).x)*32.,max(16.-float(texelFetch(voxels, ivec3(pos)/16, 4).x)*16.,max(8.-float(texelFetch(voxels, ivec3(pos)/8, 0).x)*8., max(4.-float(texelFetch(voxels, ivec3(pos)/4, 2).x)*4., max(2.-float(texelFetch(voxels, ivec3(pos)/2, 1).x)*2., 1.)))));
    //return max(16-texelFetch(voxels, ivec3(pos)/16, 4).x*16, max(8-texelFetch(voxels, ivec3(pos)/8, 3).x*8, max(4-texelFetch(voxels, ivec3(pos)/4, 2).x*4, max(2-texelFetch(voxels, ivec3(pos)/2, 1).x*2, 1.))));
    //return max(8-texelFetch(voxels, ivec3(pos)/8, 3).x*8, max(4-texelFetch(voxels, ivec3(pos)/4, 2).x*4, max(2-texelFetch(voxels, ivec3(pos)/2, 1).x*2, 1.)));
}

vec3 rotateNormale(vec3 normale, vec3 orient)
{
    vec3 a = orient;
    vec3 b = normale;
    return vec3(a.z*b.x+a.x*b.z+a.y*b.z, a.z*b.y+a.x*b.x+a.y*b.z, a.z*b.z+a.x*b.y+a.y*b.x);
}

vec2 getCube(vec3 x)
{
    vec4 g = texelFetch(voxels, (ivec3(x)), 0);
    //if(texelFetch(voxels, ivec3(x)/32, 5).x!=0)
        //return vec2(7.,0.);
    return vec2(g.xy);
}

vec3 getNormaleInfo(vec3 pos, vec3 cube)
{
    vec3 center = cube+vec3(0.5);
    vec3 dist = pos-center;
    vec3 adist = abs(dist);
    vec3 sdist = sign(dist);
    vec3 fpos = fract(pos);
    if(adist.z>adist.y)
    {
        if(adist.z>adist.x)
            return vec3(fpos.x, fpos.y, 3 + 3*(sign(sdist.z)-1.)/2.);
        else
            return vec3(fpos.z, fpos.y, 4 + 3*(sign(sdist.x)-1.)/2.);
    }
    else
    {
        if(adist.y>adist.x)
            return vec3(fpos.z, fpos.x, 5 + 3*(sign(sdist.y)-1.)/2.);
        else
            return vec3(fpos.z, fpos.y, 4 + 3*(sign(sdist.x)-1.)/2.);
    }
}


float raycastLight(vec3 pos, vec3 normale, float steps)
{
    //vec3 lPos = pos+vec3(100.,60.,100.);
    vec3 dir = normalize(lightPos + vec3(96.,0.,96.)-pos);
    float g = 0;
    //vec3 dir = vec3(0,1,0);
    for (int i=0; i<steps && pos.y<64; i++)
    {
        vec3 sdir = sign(dir);
        float l = cubeSize(pos);
        //float l = 1.;
        //float l = 2.;
        vec3 dist = (vec3(l) - mod(pos, l))*(sdir+vec3(1.))/2. + mod(pos, l)*(sdir-vec3(1.))/-2. + 1e-4;
        vec3 prior = dist/abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos+=dir*m;
        vec3 cube = floor(pos);
        vec2 c = getCube(cube); //cube id + side difference bool
        //c.x = (c.x>0.? 3:0.);
        if(c.x>=1.) //if not air and in the circle render
        {
            return 0.03;
        }
  
    }
        
        return 1.5 * max(dot(normale, dir), 0.02);
}


vec4 raycast(vec3 pos, vec3 dir)
{
    vec3 lPos = lightPos;
    vec4 fcolor = vec4(0.5, 0.4, 0.5, 0.1);
    float ray = 0.;
    float ray0 = length(dir);
    for (int i=0; i<renderDistanceSteps && ray<renderDistanceMeters && !(pos.y>64. && dir.y>0.); i++)
    {
        vec3 sdir = sign(dir);
        float l = cubeSize(pos);
        //float l = 2.;
        vec3 dist = (vec3(l) - mod(pos, l))*(sdir+vec3(1.))/2. + mod(pos, l)*(sdir-vec3(1.))/-2. + 1e-4;
        vec3 prior = dist/abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos+=dir*m;
        ray+=length(dir*m);
        if(ray>renderDistanceMeters+0.2)
            return fcolor;
        vec3 cube = floor(pos);
        vec2 c = getCube(cube); //cube id + side difference bool
        //c.x = (c.x>0.? 3:0.);
        if(c.x>=1.) //if not air and in the circle render
        {
            vec3 normaleInfo = getNormaleInfo(pos, cube); //side-relative position + side number
            vec3 normale = normales[int(normaleInfo.z)];
            vec2 uv = vec2((normaleInfo.x + c.x-1)/tilesetSize, (1-normaleInfo.y)/2.);
            if(c.y==1)
                uv.x += 1/tilesetSize*normaleInfo.z;
            vec3 addNorm = normalize(vec3(texture(texAtlas, uv-vec2(0,0.5)).xyz));
            if(advancedGraphics)
                normale = rotateNormale(addNorm, normale);
            vec3 lightDir = normalize(lPos);
            float ambient = 0.3;
            float diff = max(dot(normale, lightDir), 0.1);
            //float diff = 0.;
            if(advancedGraphics)
                diff = diff/4.+raycastLight(pos, normale, renderDistanceSteps-i);
            float spectacularPower = 0.4;
            vec3 reflectDir = reflect(-lightDir, normale);
            float spec = pow(max(dot(dir, reflectDir), 0.0), 32.) * spectacularPower;
            spec = 0.;

            vec4 special = vec4(1.);
            if(cube==selectedCube && normaleInfo.z == selectedCubeNormale)
                special = vec4(1.5,0.2,0.2,1.);
            return vec4(texture(texAtlas, uv))*(diff+ambient+spec) * special;
            //return vec4(vec4(addNorm, 1.))*(diff+ambient+spec) * special;
        }
  
    }


    return fcolor;
}

void main()
{
    //lightPos*=vec3(sin(time/4.), 1., cos(time/4.));
    //color = vec4(fragPos, 0., 1.);
    color = raycast(origin, direction);
    if(abs(fragPos.x)<0.0015 && abs(fragPos.y)<0.0025)
        color = vec4(1.);
}