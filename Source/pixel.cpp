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

//[Headers] You can add your own extra header files here...
#include "overlay.h"
//[/Headers]

#include "pixel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "CompList.h"
#include "shaderPreset.h"

//[/MiscUserDefs]

//==============================================================================
pixel::pixel ()
    : _sprite(_openGLContext) , _bInit(false)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	ShaderData shaderPreset;
	if (shaderPreset._shaderPreset.size() > 0)
	{
		_strVertex = shaderPreset._shaderPreset[0]->vertexShader;
		_strFragment = shaderPreset._shaderPreset[0]->fragmentShader;
	}

	addAndMakeVisible(overLay = new overlay());
	//if (overLay)
	//	overLay->addMouseListener((MouseListener*)this, false);


	_openGLContext.setComponentPaintingEnabled(true);  // if false , the overlay component can not see.

	_openGLContext.setRenderer(this);

	_openGLContext.setContinuousRepainting(true); // true  fps may be high.

	_openGLContext.attachTo(*this);

	resized();
    startTimer(1000);
    //[/Constructor]
}

pixel::~pixel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
	_openGLContext.detach();
	overLay = nullptr;
    //[/Destructor]
}

//==============================================================================
void pixel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
	//g.setColour(juce::Colours::green);
	//g.setFont(Font(16.0f));
	//g.drawText("opengl txt", getLocalBounds(), Justification::centred, true);
    
  
	return;
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void pixel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
static ComponentList<pixel> td((const String)("pixel"), 3);
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="pixel" componentName="" parentClasses="public Component, private OpenGLRenderer, private Timer, private CodeDocument::Listener"
                 constructorParams="" variableInitialisers="_sprite(openGLContext) , _bInit(false) "
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
