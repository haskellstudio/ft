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
#include "util.h"
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
               private CodeDocument::Listener,
               public juce::ValueTree::Listener
{
public:
    //==============================================================================
    pixel ();
    ~pixel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.

	void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &_property) override
	{

		if (_asmOpCodeTree == treeWhosePropertyHasChanged)
		{
			if (_asmOpCodeTree.hasProperty("property_asmOpCode") && _property == juce::Identifier("property_asmOpCode"))
			{
				var v = _asmOpCodeTree.getProperty("property_asmOpCode");
					//GLfloat v[4] = { 0.0, 1.0, 0.0, 0.8 };
				_opFloatCode.clear();

				auto a = v.getArray();
				if (a)
				{
					for (auto &i : *a)
					{
						_opFloatCode.add(i);
					}
				}

			}


			}
	}

	void valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override {}
	void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
	void valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
	void valueTreeParentChanged(juce::ValueTree &treeWhoseParentHasChanged) override {}
	void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) override {}

	void mouseDrag(const MouseEvent& event) override
	{
		mouseXf = (float)event.getPosition().x ;

		mouseYf = (float)getHeight() - (float)event.getPosition().y ;
	}
	 void mouseMove(const MouseEvent& event) override
	{

	}
	 void mouseDown(const MouseEvent& event) override
	{
		 mouseXf = (float)event.getPosition().x ;

		 mouseYf = (float)getHeight() - (float)event.getPosition().y ;

		 mouseZf = (float)event.getPosition().x ;

		 mouseWf = (float)getHeight() - (float)event.getPosition().y ;
	}

	 void mouseUp(const MouseEvent& event) override
	 {
		 mouseZf = mouseZf * -1.0;

		// mouseWf = (float)event.getPosition().y / getHeight();
	 }

	bool keyPressed(const KeyPress& key) override
	{

		return true;
	}
	void updateShader()
	{
		if (_strVertex.isEmpty() || _strFragment.isEmpty())
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

			if (overLay)
			{
				_compileResult = "GLSL: v" + String(juce::OpenGLShaderProgram::getLanguageVersion(), 2);

				const MessageManagerLock mmLock;
				if (mmLock.lockWasGained())
				{
					overLay->setSetCompileResult(_compileResult);
				    //msg(_compileResult);
					//l->setText(_compileResult, dontSendNotification);
				}

			}


		}
		else
		{
			String s = newShader->getLastError();
			if (overLay)
			{
				_compileResult = s;

				const MessageManagerLock mmLock;

				if (mmLock.lockWasGained())
				{

					overLay->setSetCompileResult(_compileResult);
				//	l->setText(_compileResult, dontSendNotification);
				}

			}


			//statusText = newShader->getLastError();
		}

	//	_strVertex = String();
		_strFragment = String();
	}



	void renderOpenGL() override
	{


		bool isActive = OpenGLHelpers::isContextActive();
		if (!isActive)
			return;
		//int i = _openGLContext.getSwapInterval();

		fps.incFrameCount();
		if (overLay)
		{
		//	const MessageManagerLock mmLock;
		//	if (mmLock.lockWasGained())
			{
				//	msg(_compileResult);
				//l->setText(_compileResult, dontSendNotification);

			}

		}

		if (false == _bInit)
		{
			_bInit = true;
			_sprite.init(-1., -1., 2., 2.);
            
            
            
            juce::SystemStats::OperatingSystemType ost = juce::SystemStats::getOperatingSystemType();
            
            if (ost & juce::SystemStats::OperatingSystemType::MacOSX)
            {
                _dynamicTexture.applyTo(_texture, "~/dev/ft//1.png");
                _dynamicTexture.applyTo(_texture2, "~/dev/ft//2.png");
                _dynamicTexture.applyTo(_texture3, "~/dev/ft/3.png");
                _dynamicTexture.applyTo(_texture4, "~/dev/ft/4.png");
            }
            else if (ost & juce::SystemStats::OperatingSystemType::Windows)
            {
                _dynamicTexture.applyTo(_texture, "E:/juceStudy/ft/ft/1.png");
                _dynamicTexture.applyTo(_texture2, "E:/juceStudy/ft/ft/2.png");
                _dynamicTexture.applyTo(_texture3, "E:/juceStudy/ft/ft/3.png");
                _dynamicTexture.applyTo(_texture4, "E:/juceStudy/ft/ft/4.png");
                
            }
                
                

			_openGLContext.setSwapInterval(0);
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
		glEnable(GL_TEXTURE_2D);


		_openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
		_texture.bind();
		_openGLContext.extensions.glActiveTexture(GL_TEXTURE1);
		_texture2.bind();
		_openGLContext.extensions.glActiveTexture(GL_TEXTURE2);
		_texture3.bind();

		_openGLContext.extensions.glActiveTexture(GL_TEXTURE2+1);
		_texture4.bind();

		const float desktopScale = (float)_openGLContext.getRenderingScale();
		glViewport(0, 0, roundToInt(desktopScale * getWidth()), roundToInt(desktopScale * getHeight()));

		
		
		
	

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
			if (uf->texture2)
			{
				uf->texture2->set(1);
			}
			if (uf->texture3)
			{
				uf->texture3->set(2);
			}
			if (uf->texture4)
			{
				uf->texture4->set(3);
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
				uf->arrFloat->set(_opFloatCode.getRawDataPointer(), _opFloatCode.size());
			}
			if (uf->iMouse)
			{
				uf->iMouse->set(mouseXf, mouseYf, mouseZf, mouseWf);
			}
			if (uf->iResolution)
			{
				uf->iResolution->set(float(getWidth()), float(getHeight()));
			}

		}

		atrr->enable(_openGLContext);
		sp.draw();
		atrr->disable(_openGLContext);
		_openGLContext.extensions.glUseProgram(0);

	}

	void timerCallback() override
	{
          overLay->setTxt(juce::String(fps.fps));


		  juce::Time t = f.getLastModificationTime();

		  if (lastModiy != t)
		  {
			  lastModiy = t;
			  juce::StringArray sa;
			  f.readLines(sa);
			  _strFragment = sa.joinIntoString("\n");

		  }
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
		_texture2.release();
		_texture3.release();
		_texture4.release();
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
	OpenGLTexture _texture2;
	OpenGLTexture _texture3;
	OpenGLTexture _texture4;

	FPS fps;

	float mouseXf{ 0 };
	float mouseYf{ 0 };
	float mouseZf{ 0 };
	float mouseWf{ 0 };


	juce::File f;
	juce::Time lastModiy;

	juce::ValueTree _tree;
	juce::ValueTree _asmOpCodeTree;
	juce::Array<float> _opFloatCode;
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
