#version 430 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D texAtlas;
uniform vec3 origin;
uniform ivec3 selectedCube;
uniform ivec3 selectedCubeNormale = ivec3(0,1,0);
uniform sampler3D voxels;

uniform mat4 rotation;

float c = 1920./1080.;

vec3 normales[] = {vec3(0.,0., -1.), vec3(-1., 0., 0.), vec3(0., -1., 0.), vec3(0.,0.,1.), vec3(1.,0.,0.), vec3(0., 1., 0.)};

float tilesetSize = 128.;
float renderDistanceSteps = 60.;
float renderDistanceCubes = 25.;

vec2 getCube(vec3 x)
{

    vec4 g = texelFetch(voxels, (ivec3(x)), 0);

    //if(g.x==1.)
        //return vec2(1., 1.);
    //return vec4(g.xyz, 1.);

    //if(g.x==0.)
        //return vec2(1., 1.);

    return vec2(g.xy*255.);
}

vec3 getNormale(vec3 pos, vec3 cube)
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


vec4 raycast(vec3 pos, vec3 dir)
{
    vec3 lightPos = pos;
    vec4 fcolor = vec4(0.5, 0.4, 0.5, 0.1);
    float ray = 0.;
    float ray0 = length(dir);
    for (int i=0; i<renderDistanceSteps && ray<renderDistanceCubes; i++)
    {
        vec3 sdir = sign(dir);
        vec3 dist = (vec3(1., 1., 1.) - fract(pos))*(sdir+vec3(1.))/2. + fract(pos)*(sdir-vec3(1.))/-2. + 1e-4;
        vec3 prior = dist/abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos+=dir*m;
        ray+=length(dir*m);
        vec3 cube = floor(pos);
        vec2 c = getCube(cube); //cube id + side difference bool
        if(c.x>=1.) //if not air render
        {
            vec3 normale = getNormale(pos, cube); //side-relative position + side number
            vec3 norm = normales[int(normale.z)];
            vec3 lightDir = normalize(lightPos-pos);
            float ambient = 0.3;
            float diff = max(dot(norm, lightDir), 0.0);
            float spectacularPower = 0.4;
            vec3 diffuse = diff*vec3(1.);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(dir, reflectDir), 0.0), 32.) * spectacularPower;
            
            vec2 uv = vec2((normale.x + c.x-1)/tilesetSize, 1-normale.y);
            //uv.x+=(cube.x-1)/tilesetSize;
            if(c.y==1)
                uv.x += 1/tilesetSize*normale.z;
            vec4 special = vec4(1.);
            if(cube==selectedCube && norm == selectedCubeNormale)
                special = vec4(0.2);
            return vec4(texture(texAtlas, uv))*(diff+ambient+spec) * special;
        }
  
    }


    return fcolor;
}

void main()
{
    vec3 pos = origin;
    vec2 fragPos = (TexCoord-vec2(0.5,0.5))*2;
    fragPos*=sin(35.*0.0174533) / cos(35.*0.0174533);
    vec3 camPos = vec3(0.,0.,-1.);
    vec3 direction = (vec4(normalize(vec3(fragPos.x*c, fragPos.y, 0.) - camPos), 1.)*rotation).xyz;
    color = raycast(pos, direction);
    if(abs(fragPos.x)<0.001 && abs(fragPos.y)<0.001)
        color = vec4(1.);
}