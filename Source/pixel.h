/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.0.1

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "../JuceLibraryCode/JuceHeader.h"
#include "Attributes.h"
#include "sprite.h"
#include "shaderPreset.h"
extern void msg(juce::String s);
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class pixel  : public Component,
               private OpenGLRenderer,
               private Timer,
               private CodeDocument::Listener
{
public:
    //==============================================================================
    pixel ();
    ~pixel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	void updateShader()
	{
		if (_strVertex.isEmpty() && _strFragment.isEmpty())
		{
			return;
		}
		String _compileResult;
		ScopedPointer<OpenGLShaderProgram> newShader(new OpenGLShaderProgram(_openGLContext));

		if (newShader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(_strVertex))
			&& newShader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(_strFragment))
			&& newShader->link())
		{
			_shader = nullptr;
			_shader = newShader;

			_attributes = nullptr;
			_attributes = new Attributes(_openGLContext, *_shader);

			_uniforms = nullptr;
			_uniforms = new Uniforms(_openGLContext, *_shader);


			//_sprite.setShader(_shader);

		//	if (l)
			{
				_compileResult = "GLSL: v" + String(juce::OpenGLShaderProgram::getLanguageVersion(), 2);
				
				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
				//	msg(_compileResult);
					//l->setText(_compileResult, dontSendNotification);
				}

			}


		}
		else
		{
			String s = newShader->getLastError();
		//	if (l)
			{
				_compileResult = s;

				const MessageManagerLock mmLock;
				
				if (mmLock.lockWasGained())
				{
				//	msg(_compileResult);
				//	l->setText(_compileResult, dontSendNotification);
				}

			}


			//statusText = newShader->getLastError();
		}

		_strVertex = String();
		_strFragment = String();
	}



	void renderOpenGL() override
	{

		if (overLay)
		{
			overLay->setTxt(juce::String(juce::Time::getApproximateMillisecondCounter()));
		}

		bool isActive = OpenGLHelpers::isContextActive();
		if (!isActive)
			return;

		if (false == _bInit)
		{
			_bInit = true;
			_sprite.init(-.5, -.5,1., 1.);
			_dynamicTexture.applyTo(_texture);
		}

		updateShader();
		if (_shader == nullptr)
		{
			//msg(juce::String("_shader == nullptr"));
			return;
		}

		OpenGLHelpers::clear(Colour(Colours::black.withAlpha(0.0f)));


		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		_openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);

		const float desktopScale = (float)_openGLContext.getRenderingScale();
		glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

		_texture.bind();


		drawSprite(_shader, _sprite, _attributes, _uniforms, (float)(Time::getMillisecondCounter() / 140), getWidth(), getHeight());

	}




	void drawSprite(OpenGLShaderProgram* s, Sprite&  sp, Attributes* atrr, Uniforms* uf, int itime_, float winw, float winh) {

		float itime = float(itime_ % 1000);
		s->use();
		sp.bind();
		if (uf)
		{
			if (uf->lightPosition)
			{
				if (sp.isClicked == 0)
					uf->lightPosition->set(1.0f, 1.0f, 1.0f, 1.0f);
				else if (sp.isClicked == 1)
					uf->lightPosition->set(0.0f, 1.0f, 0.0f, 1.0f);

			}

			if (uf->projectionMatrix != nullptr)
				uf->projectionMatrix->setMatrix4(getProjectionMatrix().mat, 1, false);

			if (uf->viewMatrix != nullptr)
				uf->viewMatrix->setMatrix4( getViewMatrix().mat , 1, false);

			if (uf->texture)
			{
				uf->texture->set(0);
			}
			if (uf->iGlobalTime)
			{

				uf->iGlobalTime->set(itime);
			}
			if (uf->x)
			{
				uf->x->set(sp._x);
			}

			if (uf->y)
			{
				uf->y->set(sp._y);
			}

			if (uf->w)
			{
				uf->w->set(sp._width);
			}

			if (uf->h)
			{
				uf->h->set(sp._height);
			}

			if (uf->winW)
			{
				uf->winW->set(winw);
			}

			if (uf->winH)
			{
				uf->winH->set(winh);
			}
			//void OpenGLShaderProgram::Uniform::set (const GLfloat* values, GLsizei numValues)
			if (uf->arrFloat)
			{
				GLfloat v[4] = { 0.0, 1.0, 0.0, 0.8 };
				uf->arrFloat->set(v, 4);
			}
		}

		atrr->enable(_openGLContext);
		sp.draw();
		atrr->disable(_openGLContext);
		_openGLContext.extensions.glUseProgram(0);

	}

	void timerCallback() override
	{
		//stopTimer();
	}
	void codeDocumentTextInserted(const String& /*newText*/, int /*insertIndex*/) override
	{

	//	startTimer(1000);
	}

	void codeDocumentTextDeleted(int /*startIndex*/, int /*endIndex*/) override
	{
	//	startTimer(1000);
	}


	Matrix3D<float> getProjectionMatrix() const
	{
		Matrix3D<float> ortho(1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, -.01f, 0.0f,
			0.0f, 0.0f, -1.0f, 1.0f
			
			
			);
		return ortho;
	/*	float w = 1.0f / (0.5f + 0.1f);
		float h = w * getLocalBounds().toFloat().getAspectRatio(false);
		return Matrix3D<float>::fromFrustum(-w, w, -h, h, 4.0f, 30.0f);*/
	}

	Matrix3D<float> getViewMatrix() const
	{
		Matrix3D<float> viewMatrix;//(Vector3D<float>(0.0f, 0.0f, 0.0f));
		return  viewMatrix;
		/*Matrix3D<float> rotationMatrix
			= viewMatrix.rotated(Vector3D<float>(-0.3f, 5.0f * std::sin(getFrameCounter() * 0.01f), 0.0f));

		return rotationMatrix * viewMatrix;*/
	}

	void newOpenGLContextCreated() override
	{
		freeAllContextObjects();
	}
	void openGLContextClosing() override
	{
		freeAllContextObjects();
	}
	void freeAllContextObjects()
	{
		_shader = nullptr;
		_texture.release();
	}
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	bool _bInit;
	OpenGLContext _openGLContext;
	ScopedPointer<OpenGLShaderProgram> _shader;
	ScopedPointer<OpenGLShaderProgram> _shader2;

	String _strVertex;
	String _strFragment;

	DynamicTexture _dynamicTexture;

	ScopedPointer<Attributes> _attributes;
	ScopedPointer<Uniforms> _uniforms;

	ScopedPointer<overlay> overLay;

	Sprite _sprite;

	OpenGLTexture _texture;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (pixel)
};

//[EndFile] You can add extra defines here...


//const GLubyte* version = glGetString(GL_VERSION); //返回负责当前OpenGL实现厂商的名字  
//DBG(juce::String((char*)version));
//const GLubyte* vender = glGetString(GL_VENDOR); //返回负责当前OpenGL实现厂商的名字  
//DBG(juce::String((char*)vender));
//const GLubyte* render = glGetString(GL_RENDERER); //返回一个渲染器标识符，通常是个硬件平台  
//DBG(juce::String((char*)render));
//[/EndFile]
