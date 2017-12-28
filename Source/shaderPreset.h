#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


#include "shaderTemplate.h"


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
	        fragmentShader
		};

		_shaderPreset.add(p);
	}

};


























