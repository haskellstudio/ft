/*
  ==============================================================================

    glsl.cpp
    Created: 15 Nov 2017 5:59:04pm
    Author:  secci

  ==============================================================================
*/

#define GLM_SWIZZLE
#include <glm/glm.hpp>
using namespace glm;
extern vec4 fragColor ;
extern vec2 iResolution ;




#define S(a, b, t) smoothstep(a,b,t)
#define sat(x) clamp(x, 0.f, 1.f)



float remap01(float a, float b, float t)
{
	float s1 = (t - a) / (b - a);
	float s2 = sat(s1);
	return s2;
	
	//return sat((t - a) / (b - a));
}

float remap(float a, float b, float c, float d, float t)
{
	return (t - a) / (b - a) * (d - c) + c;
}

//vec4 Eye(vec2 uv)
//{
//	vec4 col = vec4(0.);
//
//	return col;
//}
//
//vec4 Mouth(vec2 uv)
//{
//	vec4 col = vec4(0.);
//
//	return col;
//}

vec4 Head(vec2 uv)
{
	vec4 col = vec4(0.9f, .65f, .1f, 1);

	float d = length(uv);


	col.a = S(.5f, .49f, d);
	/*
	if(d >.5)
	col.a = 0;
	else if(d < 0.45 )
	col.a = 1;
	else // 0.45d<0.5
	col.a = 0.5;   // smooth step
	*/

	float edgeShade = remap01(0.35f, .5f, d);

	edgeShade *= edgeShade;

	col.rgb = col.rgb * (1.f - edgeShade*.5f);
	

	col.rgb = mix((vec3)col.rgb, vec3(.6f, .3f, .1f), S(.47f, .48f, d));

	//float ss =S(.47f, .48f, d);
	//col.r = mix(col.r, .6f, ss);
	//col.g = mix(col.g, .3f, ss);
	//col.b = mix(col.b, .1f, ss);
	return col;
}


vec4 smiley(vec2 uv)
{
	vec4 col = vec4(.0f, .0f, .0f, 1.f);
	vec4 head = Head(uv);

	//head.a = 1.;
	//return head; 

	col = mix(col, head, head.a);
	return col;
}
void mainImage(vec2 gl_FragCoord)
{
	vec2 uv = gl_FragCoord.xy() / iResolution.xy();
	uv -= 0.5;
	uv.x *= iResolution.x / iResolution.y;
	fragColor = smiley(uv);

}
