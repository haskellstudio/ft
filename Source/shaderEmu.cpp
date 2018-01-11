/*
  ==============================================================================

    shaderEmu.cpp
    Created: 15 Nov 2017 5:54:38pm
    Author:  secci

  ==============================================================================
*/

#include "shaderEmu.h"
vec4 fragColor = vec4(1, .0, 1.0, 1.0);
vec2 iResolution = vec2(100., 100.);
extern void mainImage(vec2& gl_FragCoord);

vec4 getRGBA(vec2 iResolution_, vec2 gl_FragCoord) {
	iResolution = iResolution_;
	mainImage(gl_FragCoord);
	return fragColor;

}
