#version 430 core
#define PI 3.1415926535897932384626433832795
in float norm;
in vec2 uv;
in vec3 position;
in float occlusion;
out vec4 color;

uniform vec3 normales[6] = {vec3(0.,0., -1.), vec3(-1., 0., 0.), vec3(0., -1., 0.), vec3(0.,0.,1.), vec3(1.,0.,0.), vec3(0., 1., 0.)};

uniform sampler2D texAtlas;
uniform usampler3D voxels;
float tilesetSize = 32.;

uniform bool advancedGraphics;
uniform sampler2D normAtlas;
uniform vec3 origin;
uniform float renderDistanceSteps = 40.;
uniform float renderDistanceMeters = 300.;
uniform ivec3 selectedCube;
uniform float selectedCubeNormale;
vec3 lightDir = vec3(10.,7.,10.);


vec2 getCube(vec3 x)
{
    vec4 g = texelFetch(voxels, (ivec3(x)), 0);
    return g.xy;
}


float cubeSize(ivec3 pos)
{
    return max(32.-float(texelFetch(voxels, ivec3(pos)/32, 5).x)*32.,max(16.-float(texelFetch(voxels, ivec3(pos)/16, 4).x)*16.,max(8.-float(texelFetch(voxels, ivec3(pos)/8, 0).x)*8., max(4.-float(texelFetch(voxels, ivec3(pos)/4, 2).x)*4., max(2.-float(texelFetch(voxels, ivec3(pos)/2, 1).x)*2., 1.)))));
}


float raycastLight(vec3 pos, vec3 normaleF, float steps)
{
    vec3 dir = lightDir;
    float g = 0;
    vec3 sdir = sign(dir)/2.;
    for (int i=0; i<steps && pos.y<64. && pos.y>=0.; i++)
    {
        float l = cubeSize(ivec3(pos));
        vec3 dist = (vec3(l) - mod(pos, l))*(sdir+vec3(0.5)) + mod(pos, l)*(-sdir+vec3(0.5)) + 1e-4;
        vec3 prior = dist/abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos+=dir*m;
        vec2 c = getCube(pos); //cube id + side difference bool
        if(c.x>=1.) //if not air and in the circle render
        {
            return 0.;
        }
  
    }
        
        return max(1.5*dot(normaleF, dir), 0.01);
}










void main()
{
    lightDir = normalize(lightDir);
    vec3 dist = position-origin;
    float ray = length(dist);
    vec3 c = position-normales[int(norm)]*0.5;
    vec2 cube = getCube(c);
    float tcdx = (cube.y==1.? norm:0);
    vec2 uv1 = vec2(1.)-uv;
    vec2 tc = vec2((cube.x+tcdx-1+uv1.x)/tilesetSize, uv1.y);
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);

    //vec3 normale = (!advancedGraphics? normales[int(norm)]:texture2D(normAtlas, tc).xyz);
    vec3 normale = normales[int(norm)];
    vec3 addNorm = normalize(vec3(textureLod(normAtlas, tc, min(ray/renderDistanceMeters*ray-2., 7.)).xyz)*2.-vec3(1.));
    if(advancedGraphics)
                {
                    switch(int(norm))
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



    float ambient=0., diff=0., addDiff=0.;
    ambient = 0.66*occlusion;
    ambient=(advancedGraphics? min(ambient, 0.33):ambient);
    diff = 0.3;
    addDiff = (advancedGraphics ? raycastLight(position+normales[int(norm)]*1e-4*1.8, normale, 20) : 0.);
    ambient=(addDiff>0. ? 0.33:ambient);
    vec4 special = vec4(1.);
    if(ivec3(c)==selectedCube && norm == selectedCubeNormale)
        special = vec4(1.5,0.2,0.2,1.);

    color = textureLod(texAtlas, tc, 5*ray/renderDistanceMeters)*special;
    color*= (ambient+diff+addDiff);

    /*float ambient;
    float diff;
    //ambient = 0.66;
    ambient = (!advancedGraphics?0.66:0.33);
    ambient*=occlusion;
    //ambient = max(ambient*occlusion, 0.1);
    diff = max(dot(normale, lightDir), 0.1);
    
    diff = 0.3;

    vec4 special = vec4(1.);
    //if(c==selectedCube && norm == selectedCubeNormale)
    if(ivec3(c)==selectedCube && norm == selectedCubeNormale)
        special = vec4(1.5,0.2,0.2,1.);

    color = textureLod(texAtlas, tc, 5*ray/renderDistanceMeters)*special;
    if(advancedGraphics && ray<renderDistanceMeters/3.)
        color*=(raycastLight(position+normales[int(norm)]*1e-4*1.8, normale, 20) + ambient);
    else
        color*=(diff+ambient);
    if(cube.x==0.)
        color = vec4(0.4,0.8,0.8,1.);*/

    //color.z*=occlusion;

    color.w = 0;

    //color = vec4(vec3((gl_FragCoord.z)*gl_FragCoord.z*1.5), 1.);
    //color = vec4(0.3,0.3,0.8,1.);
} 