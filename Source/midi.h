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
#include "util.h"
extern void msg(juce::String s);
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class midi  : public Component,
              public ButtonListener
{
public:
    //==============================================================================
    midi ();
    ~midi();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
	int _clientOrServer{ 0 };

	void writeMidi()
	{
		CMemoryStream s;

		s.writeStr(juce::String("MThd").getCharPointer());   // midi header 

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(6));  // head description size, always 6
		
		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 1));  //   0, signle track    1, multi track and Synchronize  2, multi track and asynchronous
		
		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 1));  // how many tracks 

		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 120));  // note druation

		s.writeStr(juce::String("MTrk").getCharPointer());                     // midi track  header

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(25));                  // midi track  size
		
		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x51);													   // 51 ->  the speed

		s.writeByte(0x3);													   // 3 ->  the arg num specified by 51

		s.writeByte(0x07);													   // 3 ->  the speed
		s.writeByte(0xa1);													   //
		s.writeByte(0x20);													   //
		//7

		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x58);													   // 51 ->  the jie pai

		s.writeByte(0x4);													   // 4 ->  the arg num specified by 58

		s.writeByte(0x06);													   // 6/8 jie pai zhong de 6
		s.writeByte(0x3);													   //6/8 jie pai zhong de 8  ;  8 = 2 ^ 3
		s.writeByte(0x00);													   //  unused
		s.writeByte(0x0);													   //  unused
		//15



		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x59);													   // 59 ->  the diao hao

		s.writeByte(0x2);													   // 2 ->  the arg num specified by 59

		s.writeByte(0x3);													   // -7~-1 number of jian hao     0(C)  1~7 (sheng hao)
		s.writeByte(0x0);													   // 0 -> da diao    1-> xiao diao 
		//21

		//s.writeByte(0x00);    //time offset
		//s.writeByte(0x96);    //9x -> press a note    6->  NO.5 channel
		//s.writeByte(0x45);    //zhong yin A 
		//s.writeByte(0x70);    // the strength
		////25

		//s.writeByte(0x78 /*0x3c*/);    // after 60 ticks ,   120
		////s.writeByte(0x80);
		//s.writeByte(0x45);    // zhong yin A 
		//s.writeByte(00);      // the strength is 0
		////28

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(0xff2f00));                  //  00 FF 2F 00   end of a track
 //32
		if (!s.writeToFile("m.midi"))
		{
			msg("fail to writeToFile m.midi");
		}

		//http://phpmidiparser.com  can parse 

	}

	void writeMidi2()
	{
		CMemoryStream s;

		s.writeStr(juce::String("MThd").getCharPointer());   // midi header 

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(6));  // head description size, always 6

		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 1));  //   0, signle track    1, multi track and Synchronize  2, multi track and asynchronous

		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 1));  // how many tracks 

		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 120));  // note druation

		s.writeStr(juce::String("MTrk").getCharPointer());                     // midi track  header

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(32));                  // midi track  size

		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x51);													   // 51 ->  the speed

		s.writeByte(0x3);													   // 3 ->  the arg num specified by 51

		s.writeByte(0x07);													   // 3 ->  the speed
		s.writeByte(0xa1);													   //
		s.writeByte(0x20);													   //
																			   //7

		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x58);													   // 51 ->  the jie pai

		s.writeByte(0x4);													   // 4 ->  the arg num specified by 58

		s.writeByte(0x06);													   // 6/8 jie pai zhong de 6
		s.writeByte(0x3);													   //6/8 jie pai zhong de 8  ;  8 = 2 ^ 3
		s.writeByte(0x00);													   //  unused
		s.writeByte(0x0);													   //  unused
																			   //15



		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x59);													   // 59 ->  the diao hao

		s.writeByte(0x2);													   // 2 ->  the arg num specified by 59

		s.writeByte(0x3);													   // -7~-1 number of jian hao     0(C)  1~7 (sheng hao)
		s.writeByte(0x0);													   // 0 -> da diao    1-> xiao diao 
																			   //21

		s.writeByte(0x00);    //time offset
		s.writeByte(0x96);    //9x -> press a note    6->  NO.5 channel
		s.writeByte(0x45);    //zhong yin A 
		s.writeByte(0x70);    // the strength
							  //25

		s.writeByte(0x78 /*0x3c*/);    // after 60 ticks ,   120
									   //s.writeByte(0x80);
		s.writeByte(0x45);    // zhong yin A 
		s.writeByte(00);      // the strength is 0
							  //28

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(0xff2f00));                  //  00 FF 2F 00   end of a track
																					  //32
		if (!s.writeToFile("m.midi"))
		{
			msg("fail to writeToFile m.midi");
		}

		//http://phpmidiparser.com  can parse 

	}
	
	void writeMidi3()
	{
		CMemoryStream s;

		s.writeStr(juce::String("MThd").getCharPointer());   // midi header 

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(6));  // head description size, always 6
		
		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 1));  //   0, signle track    1, multi track and Synchronize  2, multi track and asynchronous
		
		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 1));  // how many tracks 

		s.writeWord(juce::ByteOrder::swapIfLittleEndian((juce::uint16) 120));  // note druation

		s.writeStr(juce::String("MTrk").getCharPointer());                     // midi track  header

		s.writeDword(juce::ByteOrder::swapIfLittleEndian(44));                  // midi track  size
		
		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x51);													   // 51 ->  the speed

		s.writeByte(0x3);													   // 3 ->  the arg num specified by 51

		s.writeByte(0x07);													   // 3 ->  the speed
		s.writeByte(0xa1);													   //
		s.writeByte(0x20);													   //
		//7




		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x58);													   // 51 ->  the jie pai

		s.writeByte(0x4);													   // 4 ->  the arg num specified by 58

		s.writeByte(0x06);													   // 6/8 jie pai zhong de 6
		s.writeByte(0x3);													   //6/8 jie pai zhong de 8  ;  8 = 2 ^ 3
		s.writeByte(0x00);													   //  unused
		s.writeByte(0x0);													   //  unused
		//15



		s.writeByte(0);                                                        // time offset 0

		s.writeByte(0xff);													   // ff -> other format

		s.writeByte(0x59);													   // 59 ->  the diao hao

		s.writeByte(0x2);													   // 2 ->  the arg num specified by 59

		s.writeByte(0x3);													   // -7~-1 number of jian hao     0(C)  1~7 (sheng hao)
		s.writeByte(0x0);													   // 0 -> da diao    1-> xiao diao 
		//21

		s.writeByte(0x00);    //time offset
		s.writeByte(0x96);    //9x -> press a note    6->  NO.5 channel
		s.writeByte(0x45);    //zhong yin A 
		s.writeByte(0x70);    // the strength
		//25
		

		s.writeByte(0x0);    // after 0 ticks , 
		s.writeByte(0x3e);    // zhong yin D 
		s.writeByte(0x64);      // the strength is 100
		//28

		s.writeByte(0x3c);    // after 60 ticks , 
		s.writeByte(0x45);    // zhong yin A 
		s.writeByte(00);      // the strength is 0
		//31
		
		s.writeByte(0x0);    // after 0 ticks , 
		s.writeByte(0x44);    // zhong yin G 
		s.writeByte(0x78);      // the strength is 100
		//34
		
		s.writeByte(0x78);    // after 120 = si fen yin fu ticks , 
		s.writeByte(0x44);    // zhong yin G 
		s.writeByte(0x00);     // the strength is 0
		//37

		s.writeByte(0x00);    // after 120 = si fen yin fu ticks , 
		s.writeByte(0x3e);    // zhong yin G 
		s.writeByte(0x00);     // the strength is 0
		//40

	
		s.writeDword(juce::ByteOrder::swapIfLittleEndian(0xff2f00));                  //  00 FF 2F 00   end of a track
 //44
		if (!s.writeToFile("m.midi"))
		{
			msg("fail to writeToFile m.midi");
		}

		//http://phpmidiparser.com  can parse 

	}
	
	
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
	
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<TextButton> _btnGenMidi;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (midi)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
