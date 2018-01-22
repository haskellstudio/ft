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
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class code  : public Component,
              public TextEditor::Listener,
              private Timer
{
public:
    //==============================================================================
    code (juce::ValueTree& tree);
    ~code();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.


	virtual void textEditorTextChanged(TextEditor& e) override
	{
		stopTimer();
		startTimer(3000);

	}


	virtual void timerCallback() override
	{
		stopTimer();
		if (_cSourceTree.isValid())
		{
			_cSourceTree.setProperty("cSource", textEditor->getText(), nullptr);
		}
	}
	/** Called when the user presses the return key. */
	virtual void textEditorReturnKeyPressed(TextEditor&) override {}

	/** Called when the user presses the escape key. */
	virtual void textEditorEscapeKeyPressed(TextEditor&) override {}

	/** Called when the text editor loses focus. */
	virtual void textEditorFocusLost(TextEditor&) override {}


    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    juce::ValueTree _tree;
	juce::ValueTree _cSourceTree;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> textEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (code)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
