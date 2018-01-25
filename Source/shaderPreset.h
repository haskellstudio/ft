#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


#include "shaderTemplate2.h"


struct ShaderPreset
{
	String name;
	String vertexShader;
	String fragmentShader;
};






struct ShaderData {
	Array<ShaderPreset*> _shaderPreset;

	ShaderData()
	{

		ShaderPreset * p = new ShaderPreset
		{
			"shader2",

			vetexShader
			,
	        fragmentShader + fragmentShader2
		};

		_shaderPreset.add(p);
	}

};


























