/*
  ==============================================================================

    shaderTemplate2.h
    Created: 25 Jan 2018 4:25:35pm
    Author:  secci

  ==============================================================================
*/
 
#define STRINGIFY(A) #A



juce::String vetexShader =
STRINGIFY(\
	#version 120 \n
	attribute vec2 position;
attribute vec2 textureCoordIn;
varying vec2 textureCoordOut;
varying vec2 _uv;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform float _x;
uniform float _y;
uniform float _w;
uniform float _h;

float getx(){return (position.x - _x) / _w;}   // 0 ~ 1
float gety(){return (position.y - _y) / _h;}  // 0 ~ 1
vec2 getUV(){return vec2(getx(), gety());}

void main()
{
	_uv = getUV();
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0, 1.0);
	textureCoordOut = textureCoordIn;
}
);

juce::String fragmentShader =
STRINGIFY(\
	#version 120 \n





uniform vec4 lightPosition;
varying vec2 textureCoordOut;
varying vec2 _uv;
uniform sampler2D Texture_1;

//uniform sampler2D Texture_1;
uniform float iGlobalTime;
uniform float _x;
uniform float _y;
uniform float _w;
uniform float _h;
uniform float arrFloat[4];
uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;

uniform float _mx;
uniform float _my;\n

#define fragColor gl_FragColor\n
//#define color gl_FragColor \n
//#define o gl_FragColor \n


vec2 fragCoord = (_uv * iResolution);\n
//vec2 p = (_uv * iResolution);\n



float distanceToSegment(vec2 a, vec2 b, vec2 p)\n
{


	vec2 pa = p - a;
	vec2 ba = b - a;
	float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
	return length(pa - ba*h);


}

void main()\n
{

	vec2 p = fragCoord.xy / iResolution.xx;
	vec4 m = iMouse / iResolution.xxxx;

	vec3 col = vec3(0.0);

	if (m.z>0.0)
	{
		float d = distanceToSegment(m.xy, m.zw, p);
		col = mix(col, vec3(1.0, 1.0, 0.0), 1.0 - smoothstep(.004, 0.008, d));
	}

	col = mix(col, vec3(1.0, 0.0, 0.0), 1.0 - smoothstep(0.03, 0.035, length(p - m.xy)));
	col = mix(col, vec3(0.0, 0.0, 1.0), 1.0 - smoothstep(0.03, 0.035, length(p - abs(m.zw))));

	fragColor = vec4(col, 1.0);

}






);



juce::String fragmentShader2 = "";


//https://www.shadertoy.com/view/4sjczm

//https://www.shadertoy.com/view/4lfcRf


//https://www.shadertoy.com/view/Mss3zH

/*
float distanceToSegment(vec2 a, vec2 b, vec2 p)\n
{ 


vec2 pa = p - a;
vec2 ba = b - a;
float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
return length(pa - ba*h);


}

void main()\n
{ 

vec2 p = fragCoord.xy / iResolution.xx;
vec4 m = iMouse / iResolution.xxxx;

vec3 col = vec3(0.0);

if (m.z>0.0)
{
float d = distanceToSegment(m.xy, m.zw, p);
col = mix(col, vec3(1.0, 1.0, 0.0), 1.0 - smoothstep(.004, 0.008, d));
}

col = mix(col, vec3(1.0, 0.0, 0.0), 1.0 - smoothstep(0.03, 0.035, length(p - m.xy)));
col = mix(col, vec3(0.0, 0.0, 1.0), 1.0 - smoothstep(0.03, 0.035, length(p - abs(m.zw))));

fragColor = vec4(col, 1.0);

}
*/