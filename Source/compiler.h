#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "util.h"
#include "c.h"




class DemoThread : public Thread
{
public:
	DemoThread()
		: Thread("Juce Demo Thread")
	{
		startThread();
	}

	~DemoThread()
	{
		// allow the thread 2 seconds to stop cleanly - should be plenty of time.
		stopThread(2000);
	}

	void run() override
	{
		// this is the code that runs this thread - we'll loop continuously,
		// updating the coordinates of our blob.

		// threadShouldExit() returns true when the stopThread() method has been
		// called, so we should check it often, and exit as soon as it gets flagged.
		while (!threadShouldExit())
		{
			// sleep a bit so the threads don't all grind the CPU to a halt..
			wait(100);
			juce::String src = getSrc();
			if (src != "")
			{
				juce::Array<float> opCodeArray;

				String compileResult;
				String asmStr;
				if (lan.compile(src, opCodeArray, compileResult, asmStr))
				{

					setOpCode(opCodeArray, asmStr);
					SetCompileResult(compileResult, true);
				}
				else
				{
					setOpCode(opCodeArray, asmStr);
					SetCompileResult(compileResult, false);
				}





				/*
				for (int i = 0; i < src.length(); i++)
				{
					res.add(float(src[i]));
					setOpCode(res);
				}
				*/
			}

			//// because this is a background thread, we mustn't do any UI work without
			//// first grabbing a MessageManagerLock..
			//const MessageManagerLock mml(Thread::getCurrentThread());

			//if (!mml.lockWasGained())  // if something is trying to kill this job, the lock
			//	return;                 // will fail, in which case we'd better return..

										// now we've got the UI thread locked, we can mess about with the components
		//	moveBall();
		}
	}

	void setSrc(juce::String s)
	{
		const ScopedLock sl(_srclock);
		_csrc = s;
	}

	juce::String getSrc()
	{
		juce::String res{ "" };
		const ScopedLock sl(_srclock);
		if (_csrc != "")
		{
			res = _csrc;
			_csrc = "";
		}
		return res;
		
	}

	void getOpCode(juce::Array<var> & argOpCode, String & strRes, bool & bRes, String& asmStr)
	{
		const ScopedLock sl(_oplock);
		if (_opcode.size() > 0)
		{
			argOpCode.clear();
			strRes = _CompileResult;
			bRes = _bSuccess;
			for (float & i : _opcode)
			{
				argOpCode.add(i);
			}
			_opcode.clear();

			asmStr = _assistAsmStr;
		}
	}

	void setOpCode(juce::Array<float> & argOpCode, String asmStr)
	{

		const ScopedLock sl(_oplock);
		_opcode  = argOpCode;
		_assistAsmStr = asmStr;
	}

	void SetCompileResult(String & s, bool b)
	{
		const ScopedLock sl(_oplock);
		_CompileResult = s;
		_bSuccess = b;
	}

	//bool getCompileResult(String & s)
	//{
	//	const ScopedLock sl(_oplock);
	//	s = _CompileResult ;
	//	return _bSuccess ;
	//}


	juce::String _csrc { "" };
	juce::Array<float> _opcode;
	juce::String _assistAsmStr{ "" };


	String _CompileResult;
	bool _bSuccess;
private:
	int interval;

	CriticalSection _srclock;

	CriticalSection _oplock;

	Lan lan;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoThread)
};



class compiler : public juce::ValueTree::Listener ,
	public juce :: Timer
{
public:
	compiler(juce::ValueTree& tree) : _tree(tree)
	{
		_cSourceTree = _tree.getOrCreateChildWithName("cSource", nullptr);
		_cSourceTree.addListener(this);

		_asmStrCodeTree = _tree.getOrCreateChildWithName("property_asmStrCode", nullptr);
		
		_asmOpCodeTree = _tree.getOrCreateChildWithName("property_asmOpCode", nullptr);
		_thread.startThread();

		startTimer(100);
	}


	virtual void timerCallback() override
	{
		juce::Array<var> opcode;
		bool isCompileSuccess = false;
		String compileReslut;

		String asmString;
		 _thread.getOpCode(opcode, compileReslut, isCompileSuccess, asmString);
		if (opcode.size() > 0)
		{

			_asmStrCodeTree.setProperty("property_asmStrCode", asmString, nullptr);

			_asmOpCodeTree.setProperty("property_asmOpCode", opcode, nullptr);
			_asmOpCodeTree.setProperty("property_compileReslutString", compileReslut, nullptr);
			_asmOpCodeTree.setProperty("property_compileResultBool", isCompileSuccess, nullptr);
		
		
		}
	}


	~compiler()
	{
		stopTimer();
		_thread.stopThread(400);
	}


	void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &_property) override
    {
		if (_cSourceTree == treeWhosePropertyHasChanged)
		{
			if (_cSourceTree.hasProperty("cSource"))
			{
				//AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon, "source txt", _cSourceTree.getProperty("cSource"));
				_thread.setSrc(_cSourceTree.getProperty("cSource"));
			}
		}
    }


    void valueTreeChildAdded (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenAdded) override {}
    void valueTreeChildRemoved (juce::ValueTree &parentTree, juce::ValueTree &childWhichHasBeenRemoved, int indexFromWhichChildWasRemoved) override {}
    void valueTreeChildOrderChanged (juce::ValueTree &parentTreeWhoseChildrenHaveMoved, int oldIndex, int newIndex) override {}
    void valueTreeParentChanged (juce::ValueTree &treeWhoseParentHasChanged) override {}
    void valueTreeRedirected (juce::ValueTree &treeWhichHasBeenChanged) override {}

	
private:
	juce::ValueTree _tree;
	juce::ValueTree _cSourceTree;
	juce::ValueTree _asmStrCodeTree;
	juce::ValueTree _asmOpCodeTree;

	DemoThread _thread;
};




