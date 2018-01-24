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
#include "compiler.h"
//[/Headers]

#include "pipe.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

#include "CompList.h"

#include "code.h"
#include "asm.h"
#include "shaderSource.h"
#include "fragmentShader.h"


//[/MiscUserDefs]

//==============================================================================
pipe::pipe ()
    : _compiler(_tree)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..


	addAndMakeVisible(advancedDock);

	advancedDock.addComponentToNewRow(pshaderSource = new shaderSource(_tree), 0);
	advancedDock.addComponentToNewRow(pfragmentShader  = new fragmentShader (_tree), 1);


	advancedDock.addComponentToDock(pasm_ = new asm_(_tree), 0);


	advancedDock.addComponentToDock(pcode = new code(_tree), 1);

	//advancedDock.addComponentToNewRow(new fragmentShader(), 2);

    //[/Constructor]
}

pipe::~pipe()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
	pasm_ = nullptr;
	pcode = nullptr;
	pfragmentShader = nullptr;
	pshaderSource = nullptr;
    //[/Destructor]
}

//==============================================================================
void pipe::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void pipe::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

	auto area = getLocalBounds();
	//label->setBounds(area.removeFromTop(150).reduced(4));
	//tabDock.setBounds(area.removeFromRight(250).reduced(4));
	advancedDock.setBounds(area.reduced(4));
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
static ComponentList<pipe> td((const String)("pipe"), 4);
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="pipe" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="_compiler(_tree)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
