/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.2.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "code.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
extern void msg(juce::String s);
//[/MiscUserDefs]

//==============================================================================
code::code (juce::ValueTree& tree)
    : _tree(tree)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (textEditor = new TextEditor ("new text editor"));
    textEditor->setMultiLine (true);
    textEditor->setReturnKeyStartsNewLine (true);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (true);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (true);
    textEditor->setText (TRANS("int main()\n"
    "{\n"
    "\tset_source_rgba(1.0, 0.0, 0.0, 1.0);\n"
    "\tset_line_width( 0.01);\n"
    "\tset_blur(0.005);\n"
    "\tmove_to(0.0, 0.0);\n"
    "\tline_to(0.5, 0.0);\n"
    "\tstroke();\n"
    "}\n"));

    textEditor->setBounds (40, 8, 280, 264);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	Component::setName("code");


	_cSourceTree = _tree.getOrCreateChildWithName("cSource", nullptr);

	textEditor->addListener(this);


	startTimer(200);


	juce::SystemStats::OperatingSystemType ost = juce::SystemStats::getOperatingSystemType();

	if (ost & juce::SystemStats::OperatingSystemType::MacOSX)
	{
		f = "~/dev/ft/Source/csource.h";
	}
	else if (ost & juce::SystemStats::OperatingSystemType::Windows)
	{
		f = "E:/juceStudy/ft/ft/Source/csource.h";
	}
	else
	{
		f = "./csource.h";
	}
	if (!f.existsAsFile())
	{
		juce::Result r = f.create();
		if (!r.wasOk())
		{
			msg("fail to create csource.h " + r.getErrorMessage());
		}
		else
		{
			lastModiy = f.getLastModificationTime();
		}

	}



    //[/Constructor]
}

code::~code()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    textEditor = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void code::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void code::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..

    auto area = getLocalBounds();
	textEditor->setBounds(area.reduced(4));


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

<JUCER_COMPONENT documentType="Component" className="code" componentName="" parentClasses="public Component, public TextEditor::Listener, private Timer"
                 constructorParams="juce::ValueTree&amp; tree" variableInitialisers="_tree(tree)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTEDITOR name="new text editor" id="40a926f4a47423e9" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="40 8 280 264" initialText="int main()&#10;{&#10;&#9;set_source_rgba(1.0, 0.0, 0.0, 1.0);&#10;&#9;set_line_width( 0.01);&#10;&#9;set_blur(0.005);&#10;&#9;move_to(0.0, 0.0);&#10;&#9;line_to(0.5, 0.0);&#10;&#9;stroke();&#10;}&#10;"
              multiline="1" retKeyStartsLine="1" readonly="0" scrollbars="1"
              caret="1" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
