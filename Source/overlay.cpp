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

#include "overlay.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...

//[/MiscUserDefs]

//==============================================================================
overlay::overlay ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	setOpaque(true);
    //[/Constructor_pre]

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("fps")));
	addAndMakeVisible(pl = new pipeLine());

    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (labelCompileResult = new Label ("label compile result",
                                                       TRANS("compile result")));
    labelCompileResult->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    labelCompileResult->setJustificationType (Justification::centredLeft);
    labelCompileResult->setEditable (false, false, false);
    labelCompileResult->setColour (TextEditor::textColourId, Colours::black);
    labelCompileResult->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	//label->setOpaque(false);
    //[/Constructor]
}

overlay::~overlay()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label = nullptr;
    labelCompileResult = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void overlay::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..

	return;
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void overlay::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label->setBounds (40, 32, 150, 24);
    labelCompileResult->setBounds (48, 88, 150, 24);
    //[UserResized] Add your own custom resize handling here..
	pl->setBounds(40, 32, 150, 24);
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="overlay" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="40233ff1728cfe67" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="40 32 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="fps" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <LABEL name="label compile result" id="f44be078bf8f7ea1" memberName="labelCompileResult"
         virtualName="" explicitFocusOrder="0" pos="48 88 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="compile result" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
