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

#include "pixelEmu.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
#include <sstream>
#include "CompList.h"
#include "shaderEmu.h"
#include "util.h"

extern void msg(juce::String s);
//[/MiscUserDefs]

//==============================================================================
pixelEmu::pixelEmu ()
{
    //[Constructor_pre] You can add your own custom stuff here..
	setWantsKeyboardFocus(true);
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
	edtShowBin = new TextEditor("showBin");
	edtShowBin->setWantsKeyboardFocus(false);
	edtShowBin->setReadOnly(true);

	edtShowBin->setMultiLine(true, false);
	edtShowBin->setScrollbarsShown(true);
	addAndMakeVisible(edtShowBin);
	edtShowBin->setVisible(false);
    //[/Constructor]
}

pixelEmu::~pixelEmu()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
	edtShowBin = nullptr;
    //[/Destructor]
}

//==============================================================================
void pixelEmu::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..

	const vec2 iResolution(getWidth(), getHeight());

	Image canvas(Image::ARGB, getWidth(), getHeight(), true);

	_charCanvas.clear();

	for (int y = 0; y < getHeight(); y++)
	{
		juce::Array<vec4> oneRow;
		for (int x = 0; x < getWidth(); x++)
		{
			vec2 gl_FragCoord(x,  y);
			vec4 p = getRGBA(iResolution, gl_FragCoord);
			oneRow.add(p);
			canvas.setPixelAt(x, y, juce::Colour::fromFloatRGBA(p.r, p.g, p.b, p.a));
		}
		_charCanvas.add(oneRow);
	}
	_canvas = canvas;

	g.setTiledImageFill(canvas, 0, 0, 1.0);
	g.fillAll();
    //[/UserPaint]
}

void pixelEmu::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
	auto r = getLocalBounds();
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

    //[/UserResized]
}

bool pixelEmu::keyPressed (const KeyPress& key)
{
    //[UserCode_keyPressed] -- Add your code here...
	if (key == 'S')
	{
		//juce::String s;
		CMemoryStream ms;
		//std::stringstream s;
		//s <<  std::hex ;
		for (int y = 0; y < getHeight(); y++)
		{
			for (int x = 0; x < getWidth(); x++)
			{
				juce::Colour c=	_canvas.getPixelAt(x, y);

				ms.writeRGB(_charCanvas[y][x].r, _charCanvas[y][x].g, _charCanvas[y][x].b, true);
				//s << std::hex << (int)c.getRed() << (int)c.getGreen() << (int)c.getBlue();
				//g.append(std::string(c.getRed()));
			}
			//edtShowBin->insertTextAtCaret(s.str());
			//s << "\r\n";
			//s.clear();
			ms.writeBreak();
		//	if (y > 115)
			//	break;
		}
		ms.wirteZero();

		SystemClipboard::copyTextToClipboard(juce::String((char*)ms.GetMemory()));
		msg("copyed data to clip board");


	//	edtShowBin->setVisible(!edtShowBin->isVisible());
	//	edtShowBin->setText((char*)ms.GetMemory());


	}
	else if (key == KeyPress::F1Key)
	{
		repaint();
	}

		//if (key == KeyPress::F1Key)
		//	msg("F1Key");
    return false;  // Return true if your handler uses this key event, or false to allow it to be passed-on.
    //[/UserCode_keyPressed]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
static ComponentList<pixelEmu> td((const String)("pixelEmu"), 2);
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="pixelEmu" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <METHODS>
    <METHOD name="keyPressed (const KeyPress&amp; key)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
