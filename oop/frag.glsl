#version 430 core
#define PI 3.1415926535897932384626433832795
in vec2 fragPos;
in vec3 direction;
out vec4 color;

uniform bool advancedGraphics;
uniform float time;
uniform sampler2D texAtlas;
uniform sampler2D normAtlas;
uniform sampler2D skysphere;
uniform vec3 origin;
uniform ivec3 selectedCube;
uniform float selectedCubeNormale;
uniform usampler3D voxels;

uniform vec3 normales[6] = {vec3(0.,0., -1.), vec3(-1., 0., 0.), vec3(0., -1., 0.), vec3(0.,0.,1.), vec3(1.,0.,0.), vec3(0., 1., 0.)};
uniform int extents[5] = {37449,4861,585,73,9};

float tilesetSize = 32.;
uniform float renderDistanceSteps = 40.;
uniform float renderDistanceMeters = 300.;

vec3 lightDir = vec3(10.,7.,10.);

float cubeSize(ivec3 pos)
{
    /*vec4 g;
    float dist = 1.;
    for (int i=1; i<6; i++)
    {
        pos/=2;
        g = texelFetch(voxels, pos, i);
        if(g.x>0)
            return dist;
        dist*=2;
    }
    return dist;*/
    return max(32.-float(texelFetch(voxels, ivec3(pos)/32, 5).x)*32.,max(16.-float(texelFetch(voxels, ivec3(pos)/16, 4).x)*16.,max(8.-float(texelFetch(voxels, ivec3(pos)/8, 0).x)*8., max(4.-float(texelFetch(voxels, ivec3(pos)/4, 2).x)*4., max(2.-float(texelFetch(voxels, ivec3(pos)/2, 1).x)*2., 1.)))));
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
    return vec2(g.x, mod(g.y, 10.));
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
            return vec3(fpos.x * (sdist.z+1.)/2. + (1.-fpos.x) * (sdist.z-1.)/-2., fpos.y, 3 + 3*(sdist.z-1.)/2.);
        else
            return vec3((1.-fpos.z) * (sdist.x+1.)/2. + (fpos.z) * (sdist.x-1.)/-2., fpos.y, 4 + 3*(sdist.x-1.)/2.);
    }
    else
    {
        if(adist.y>adist.x)
            return vec3(fpos.z * (sdist.y+1.)/2. + (1.-fpos.z) * (sdist.y-1.)/-2., fpos.x, 5 + 3*(sdist.y-1.)/2.);
        else
            return vec3((1.-fpos.z) * (sdist.x+1.)/2. + (fpos.z) * (sdist.x-1.)/-2., fpos.y, 4 + 3*(sdist.x-1.)/2.);
    }
}


float raycastLight(vec3 pos, vec3 normale, float steps)
{
    vec3 dir = lightDir;
    float g = 0;
    vec3 sdir = sign(dir)/2.;
    for (int i=0; i<steps && pos.y<64; i++)
    {
        float l = cubeSize(ivec3(pos));
        vec3 dist = (vec3(l) - mod(pos, l))*(sdir+vec3(0.5)) + mod(pos, l)*(-sdir+vec3(0.5)) + 1e-4;
        vec3 prior = dist/abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos+=dir*m;
        //vec3 cube = floor(pos);
        vec2 c = getCube(pos); //cube id + side difference bool
        //c.x = (c.x>0.? 3:0.);
        if(c.x>=1.) //if not air and in the circle render
        {
            return 0.0;
        }
  
    }
        
        return max(1.5*dot(normale, dir), 0.0);
}


vec4 raycast(vec3 pos, vec3 dir)
{
    vec4 fcolor = vec4(0.5, 0.4, 0.5, 0.1);
    float ca = acos(dir.z/length(dir.xz)*sign(dir.x))/PI;
    float cb = 1.-(dir.y/2.+0.5);
    fcolor = texture(skysphere, vec2(ca, cb));
    float ray = 0.;
    float ray0 = length(dir);
    vec3 sdir = sign(dir);
    vec3 adir = abs(dir);


    float ambient;
    float diff;
    vec3 normale;
    vec4 sfcolor;

    for (int i=0; i<renderDistanceSteps && ray<renderDistanceMeters && !(pos.y>64. && dir.y>0.); i++)
    {
        //dir = normalize(vec3(dir.x, dir.y+dir.z*sin(0.000000000003), dir.z+dir.y*cos(0.000000000003)));
        //dir = normalize(dir-vec3(0,0.06,0));
        float l = cubeSize(ivec3(pos));
        vec3 dist = (vec3(l) - mod(pos, l))*(sdir+vec3(1.))/2. + mod(pos, l)*(sdir-vec3(1.))/-2. + 1e-4;
        vec3 prior = dist/abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos+=dir*m;
        ray+=ray0*m;
        if(ray>renderDistanceMeters+0.2)
            return fcolor;
        vec3 cube = floor(pos);
        vec2 c = getCube(cube); //cube id + side difference bool
        //c.x = (c.x>0.? 3:0.);
        
        if(c.x>=1.) //if not air and in the circle render
        {
            vec3 normaleInfo = getNormaleInfo(pos, cube); //side-relative position + side number
            normale = normales[int(normaleInfo.z)];
            //if(c.y==2 && distance(vec2(0.5), vec2(normaleInfo.x, normaleInfo.y)) <0.5)
            {
                //dir = reflect(dir, normale);
            }
            //else
            {
                pos+=normale*1e-4*1.8;
                vec2 uv = vec2((normaleInfo.x + c.x-1)/tilesetSize, (1-normaleInfo.y));
                if(c.y==1)
                    uv.x += 1/tilesetSize*normaleInfo.z;
                vec3 addNorm = normalize(vec3(textureLod(normAtlas, uv, min(ray/renderDistanceMeters*ray-2., 7.)).xyz)*2.-vec3(1.));
                if(advancedGraphics)
                {
                    switch(int(normaleInfo.z))
                    {
                        case 0:
                            normale = addNorm.xyz*vec3(-1.,1.,-1.);
                            break;
                        case 1:
                            normale = addNorm.zyx*vec3(-1,1,1);
                            break;
                        case 2:
                            normale = addNorm.xzy*vec3(-1.,-1.,-1.);
                            break;
                        case 3:
                            normale = addNorm.xyz;
                            break;
                        case 4: 
                            normale = addNorm.zyx*vec3(1.,1.,-1.);
                            break;
                        case 5:
                            normale = addNorm.yzx;
                            break;
                    }
                }
                ambient = 0.66;
                diff = max(dot(normale, lightDir), 0.1);
                //if(advancedGraphics)
                {
                   // diff = max(diff*0.0, raycastLight(pos, normale, renderDistanceSteps-i));
                }

                vec4 special = vec4(1.);
                if(cube==selectedCube && normaleInfo.z == selectedCubeNormale)
                    special = vec4(1.5,0.2,0.2,1.);
                sfcolor = vec4(textureLod(texAtlas, uv, 5*ray/renderDistanceMeters)) * special;
                if(advancedGraphics)
                    return sfcolor*(raycastLight(pos, normale, renderDistanceSteps-i) + ambient);
                else
                    return sfcolor * (diff+ambient);
            }
        }
  
    }


    return fcolor;
}

void main()
{
    //lightDir*=vec3(sin(time), 1., cos(time));
    lightDir = normalize(lightDir);
    color = raycast(origin, direction);
    if(abs(fragPos.x)<0.0015 && abs(fragPos.y)<0.0025)
        color = vec4(1.);
}