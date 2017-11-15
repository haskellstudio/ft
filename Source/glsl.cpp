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




#define S(a, b, t) smoothstep(a,b,t);

vec4 Eye(vec2 uv)
{
	vec4 col = vec4(0.);

	return col;
}

vec4 Mouth(vec2 uv)
{
	vec4 col = vec4(0.);

	return col;
}

vec4 Head(vec2 uv)
{
	vec4 col = vec4(0.9, .65, .1, 1);

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

	return col;
}


vec4 smiley(vec2 uv)
{
	vec4 col = vec4(0.);
	vec4 head = Head(uv);
	col = mix(col, head, head.a);
	return col;
}
void mainImage(vec2 gl_FragCoord)
{
	vec2 uv = gl_FragCoord.xy() / iResolution.xy();
	uv -= 0.5f;
	uv.x *= iResolution.x / iResolution.y;





	fragColor = smiley(uv);;
}
