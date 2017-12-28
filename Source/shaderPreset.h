#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define STRINGIFY(A) #A


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
		float getx()
		{
			return (position.x - _x) / _w;
		}
		float gety()
		{
			return (position.y - _y) / _h;

		}
		vec2 getUV()
		{
			return vec2(getx(), gety());

		}
		void main()
		{
			gl_Position = projectionMatrix * viewMatrix * vec4(position, 0.0, 1.0);
			textureCoordOut = textureCoordIn;

			//_uv = getUV();
			//gl_Position =vec4(position, .0, 1.0);
		}
		)
			,

		STRINGIFY(\
				#version 120 \n
				uniform vec4 lightPosition;
		varying vec2 textureCoordOut;
		varying vec2 _uv;
		uniform sampler2D Texture_1;
		vec3 color;
		uniform float iGlobalTime;
		uniform float _x;
		uniform float _y;
		uniform float _w;
		uniform float _h;

		uniform float arrFloat[4];

		void main()
		{
			//textureCoordOut.x += .5;
			gl_FragColor = texture2D(Texture_1, textureCoordOut /*+ vec2(.5, .0)*/);

			gl_FragColor = vec4(arrFloat[0], arrFloat[1], arrFloat[2], 1.0);

			//gl_FragColor = vec4(sin (iGlobalTime), cos(iGlobalTime), 1, 1)  * lightPosition* texture2D (demoTexture, textureCoordOut);
			//gl_FragColor = vec4(uv.x,uv.y,0.,1.);
			//gl_FragColor = vec4(_uv.x,1.0,1.0,1.);
			//gl_FragColor = lightPosition;
			//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
			//the second fragment shader
		}
		)
		};


		_shaderPreset.add(p);



	}

};