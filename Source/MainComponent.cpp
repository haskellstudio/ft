/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    setSize (600, 400);

	_lstBox = new ListBox("_lstBox");
	_lstBox->setModel(this);
	addAndMakeVisible(_lstBox);



	TCompType * compType = TCompType::getCompTypeList()["shaderEdt"];
	if (compType)
	{
		_curSubComp = nullptr;
		_curSubComp = compType->createComponent();
		addAndMakeVisible(_curSubComp);

	}
	resized();
}

MainContentComponent::~MainContentComponent()
{
	_lstBox = nullptr;
}

void MainContentComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("MainContentComponent", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
	auto r = getLocalBounds();
	int listBoxWidth = roundToInt(proportionOfWidth(0.2000f));
	if (!_lstBox)
		return;
		

	_lstBox->setBounds(r.removeFromLeft(listBoxWidth));
	FlexBox masterbox;
	masterbox.flexDirection = FlexBox::Direction::column;// column;// : FlexBox::Direction::row;
	masterbox.alignItems = FlexBox::AlignItems::stretch;
	masterbox.alignContent = FlexBox::AlignContent::stretch;
	masterbox.flexWrap = juce::FlexBox::Wrap::wrap;
	masterbox.justifyContent = FlexBox::JustifyContent::center;



	int num = getNumChildComponents();
	for (auto i = 0; i < num; i++)
	{
		auto c = getChildComponent(i);
		if (c->getName() == "_lstBox")
			continue;
		if (c)
		{
			masterbox.items.add(FlexItem(1, 1).withFlex(1).withMargin(10));
			auto& flexitem = masterbox.items.getReference(masterbox.items.size() - 1);
			flexitem.associatedComponent = c;
		}
	}
	//Rectangle<float> r = getLocalBounds().toFloat();
	//r.reduce(10.0f, 10.0f);
	masterbox.performLayout(r);

}
