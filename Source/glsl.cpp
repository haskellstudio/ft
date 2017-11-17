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




#define S(a, b, t) smoothstep(a,b,t)     // smoothstep is similar but not equal to remap01
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
	float res = (t - a) / (b - a) * (d - c) + c;
	return clamp(res, min(c, d), max(c, d));
}


vec2 within(vec2 uv, vec4 rect)
{
	return (uv - rect.xy) / (rect.zw - rect.xy);
}



vec4 Mouth(vec2 uv)
{
	uv -= .5f;
	vec4 col = vec4(.5f, .18f, .05f, 1.f);

	uv.y *= 1.5;
	uv.y = uv.y - uv.x*uv.x*1.5;

	float d = length(uv);
	col.a = S(.5f, .48f, d);
	

	float td = length(uv - vec2(.0f, .6f));


	vec3 toothCol = vec3(1.f) *  S(.6f, .35f, d);
	col.rgb = mix((vec3)col.rgb, toothCol, S(.4f, .37f, td));


	td = length(uv - vec2(.0f, -.5f));
	col.rgb = mix((vec3)col.rgb, vec3(1.f, .5f, .5f), S(.5f, .2f, td));

	return col;

}


vec4 Eye(vec2 uv)
{
	uv -= 0.5f;

	float d = length(uv);
	vec4 irisCol = vec4(.3f, .5f, 1.f, 1.f);
	vec4 col = mix(vec4(1.f), irisCol, S(.1f, .7f, d)*.5f);   // no.1


	col.rgb = col.rgb * (1.f - S(.45f, .5f, d)* .5f * sat(-uv.y-uv.x) ) ;
	col.rgb = mix(vec3(0.f), (vec3)col.rgb, S(0.28f, 0.3f, d));  // no.2

	irisCol.rgb = irisCol.rgb * (1.f + S(.3f, .05f, d));
	col.rgb = mix( (vec3)irisCol.rgb, (vec3)col.rgb, S(0.25f, 0.28f, d)); //no.3
	
	col.rgb = mix(vec3(0.f), (vec3)col.rgb, S(0.14f, 0.16f, d));  //no.4
	
	float highlight = S(.0f, .1f, length(uv - vec2(-.15f, .15)));
	highlight = 1 - highlight*highlight*highlight;
	col.rgb = mix((vec3)col.rgb, vec3(1.), highlight);

	 highlight = S(.0f, .07f, length(uv - vec2(.08f, -.08f)));
	highlight = 1 - highlight*highlight*highlight;
	col.rgb = mix((vec3)col.rgb, vec3(1.), highlight);


	col.a = S(.5f, .48f, d);
	return col;
}


vec4 Head(vec2 uv)
{
	vec4 col = vec4(0.9f, .65f, .1f, 1);

	float d = length(uv);


	col.a = S(.5f, .49f, d);


	float edgeShade = remap01(0.35f, .5f, d);

	//float edgeShade = remap(.35f,  0.5f, 0.f,  1.f,  d);
	
	edgeShade *= edgeShade;

	col.rgb = col.rgb * (1.f - edgeShade*.5f);

	
	float res;
	           //if=0 use col.rgb if =1 use .6 .3 .1  (0 < res < 1)
	col.rgb = mix((vec3)col.rgb, vec3(.6f, .3f, .1f), res = S(.47f, .48f, d));

	float highlight = S(0.41f, 0.405f, d);

	highlight *= remap(0.41f, .0f, 1.f, .0f, uv.y);
	highlight *= S(.18f, .19f, length(uv - vec2(.21f, .08f)));
	col.rgb = mix((vec3)col.rgb, vec3(1.f, 1.f, 1.f), highlight);


	d = length(uv - vec2(.25f, -.2f));
	float cheek = S(.2f, .0f, d) * .4f;   // 0 - > 1

	edgeShade = remap01(.2f, .15f, d);   // 0 - > 1
	cheek = cheek * edgeShade * edgeShade * edgeShade;
	
	col.rgb = mix((vec3)col.rgb, vec3(1.f, 0.f, 0.f), cheek);

	return col;
}


vec4 smiley(vec2 uv)
{
	vec4 col = vec4(.0f, .0f, .0f, 1.f);

	uv.x = glm::abs(uv.x);
	vec4 head = Head(uv);

	vec2 uvWithin = within(uv, vec4(.03f, -.1, .37, .25));
	vec4 eye = Eye(uvWithin);
	vec4 mouth = Mouth(within(uv, vec4(-.3, -.4, .3, -.1)));

	col = mix(col, head, head.a);
	col = mix(col, eye, eye.a);
	col = mix(col, mouth, mouth.a);
	return col;
}
void mainImage(vec2 gl_FragCoord)
{
	vec2 uv = gl_FragCoord.xy() / iResolution.xy();
	uv -= 0.5;
	uv.y = -uv.y;
	uv.x *= iResolution.x / iResolution.y;
	fragColor = smiley(uv);

}
