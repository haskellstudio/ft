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
//[/Headers]

#include "midi.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include "CompList.h"


//https://github.com/WeAreROLI/JUCE/blob/master/examples/Demo/Source/Demos/DialogsDemo.cpp
static void alertBoxResultChosen(int result, midi* m)
{
	//AlertWindow::showMessageBoxAsync(AlertWindow::InfoIcon,
	//	"Alert Box",
	//	"Result code: " + String(result));

	m->_clientOrServer = result;
}
//[/MiscUserDefs]

//==============================================================================
midi::midi ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

	AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
		"OPTION",
		"this program run as server or client.",
		String("server"),
		String("client"),
		0,
		ModalCallbackFunction::forComponent(alertBoxResultChosen, this));


    //[/Constructor]
}

midi::~midi()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void midi::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
	g.drawText("midi", getLocalBounds(), Justification::centred, true);
    //[/UserPaint]
}

void midi::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
static ComponentList<midi> td((const String)("midi"), 1);
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="midi" componentName="" parentClasses="public Component"
                 constructorParams="" variableInitialisers="" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="0" initialWidth="600"
                 initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
