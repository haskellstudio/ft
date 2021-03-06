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
#include "pipe.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class overlay  : public Component
{
public:
    //==============================================================================
    overlay ();
    ~overlay();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.


	bool keyPressed(const KeyPress& key) override
	{
		if (key == 'S')
		{
			pl->setVisible(!pl->isVisible());
		}
		return false;
	}
	void setTxt(String s)
	{
		label->setText(s, juce::NotificationType::dontSendNotification);
	}

	void setSetCompileResult(String s)
	{
		labelCompileResult->setText(s, juce::NotificationType::dontSendNotification);

	}
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label;
	ScopedPointer<pipeLine> pl;
    ScopedPointer<Label> labelCompileResult;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (overlay)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
