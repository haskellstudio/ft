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
class asm_  : public Component,
              public juce::ValueTree::Listener
{
public:
    //==============================================================================
    asm_ (juce::ValueTree& tree);
    ~asm_();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.




	void valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &_property) override
	{
		static String asmStr;
		static String opArrayStr;
		static String compileReslutStr;
		static bool bCompileSuccess = false;
	
		if (_asmStrCodeTree == treeWhosePropertyHasChanged)
		{
			if (_asmStrCodeTree.hasProperty("property_asmStrCode") && _property == juce::Identifier("property_asmStrCode"))
			{
				asmStr = _asmStrCodeTree.getProperty("property_asmStrCode");
				
				//AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "source txt", _cSourceTree.getProperty("cSource"));
			}
		}

		else if (_asmOpCodeTree == treeWhosePropertyHasChanged)
		{
			if (_asmOpCodeTree.hasProperty("property_asmOpCode") && _property == juce::Identifier("property_asmOpCode") )
			{
				opArrayStr = "\nopcode:\n";
				var v = _asmOpCodeTree.getProperty("property_asmOpCode");
			//	textEditor->moveCaretToEndOfLine(true);

				
				auto a = v.getArray();
				if (a)
				{
					for (auto &i : *a)
					{
						opArrayStr = opArrayStr + i.toString() + "\n";
			
					}
				}
			}
			else if (_asmOpCodeTree.hasProperty("property_compileReslutString") && _property == juce::Identifier("property_compileReslutString"))
			{
				var v = _asmOpCodeTree.getProperty("property_compileReslutString");
				compileReslutStr = v.toString();
	
			}
			else if (_asmOpCodeTree.hasProperty("property_compileResultBool") && _property == juce::Identifier("property_compileResultBool"))
			{
				var v = _asmOpCodeTree.getProperty("property_compileResultBool");
				bCompileSuccess = v;
	
			}

		}

		String r = asmStr + "\n" + opArrayStr + compileReslutStr + "\ncompiler return: "+ String(bCompileSuccess);
		textEditor->setText(r);
		
	}


	void valueTreeChildAdded(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override {}
	void valueTreeChildRemoved(juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
	void valueTreeChildOrderChanged(juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
	void valueTreeParentChanged(juce::ValueTree &treeWhoseParentHasChanged) override {}
	void valueTreeRedirected(juce::ValueTree &treeWhichHasBeenChanged) override {}






    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	juce::ValueTree _tree;
	juce::ValueTree _asmStrCodeTree;
	juce::ValueTree _asmOpCodeTree;


    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextEditor> textEditor;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (asm_)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
