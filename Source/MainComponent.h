/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CompList.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component 
							 , public ListBoxModel
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;


/////////////////////////////
/////////////////////////////
//////  ListBoxModel virual function implementation

	int getNumRows() override
	{
		
		return  TCompType::getCompTypeList().size();
	}

	void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override
	{

		if (rowIsSelected)
			g.fillAll(Colours::deepskyblue);

		HashMap <String, TCompType*>& hm = TCompType::getCompTypeList();

		
		for (HashMap<String, TCompType*>::Iterator i(hm); i.next();)
		{
			if (i.getValue()->_index == rowNumber)
			{
				AttributedString a;
				a.setJustification(Justification::centredLeft);

				a.append(i.getKey(), Font(20.0f), Colours::white);

				a.draw(g, Rectangle<int>(width + 10, height).reduced(6, 0).toFloat());
				break;
			}
		}

	}



	void selectedRowsChanged(int lastRowSelected) override
	{
		if (curSubCompIndex == lastRowSelected)
		{
			return;
		}
		curSubCompIndex = lastRowSelected;
		HashMap <String, TCompType*>& hm = TCompType::getCompTypeList();

	
		for (HashMap<String, TCompType*>::Iterator i(hm); i.next();)
		{
			if (lastRowSelected == i.getValue()->_index)
			{
				_curSubComp = nullptr;
				_curSubComp = i.getValue()->createComponent();
				addAndMakeVisible(_curSubComp);
				resized();
				break;
			}
			
		}
	}
/////////////////////////////
/////////////////////////////

public:
	ScopedPointer<ListBox> _lstBox;

	ScopedPointer<Component> _curSubComp;

	int curSubCompIndex = 0;



private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
