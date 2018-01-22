#pragma once
#include "../JuceLibraryCode/JuceHeader.h"






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
				juce::Array<float> res;
				for (int i = 0; i < src.length(); i++)
				{
					res.add(float(src[i]));
					setOpCode(res);
				}
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

	juce::String getOpCode(juce::Array<float> & argOpCode)
	{
		const ScopedLock sl(_oplock);
		if (_opcode.size() > 0)
		{
			argOpCode = _opcode;
			_opcode.clear();
			return "mov eax, 0";
		}
		
		return "empty";
	}

	void setOpCode(juce::Array<float> & argOpCode)
	{

		const ScopedLock sl(_oplock);
		_opcode  = argOpCode;
	}


	juce::String _csrc { "" };
	juce::Array<float> _opcode;
private:
	int interval;

	CriticalSection _srclock;

	CriticalSection _oplock;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoThread)
};



class compiler : public juce::ValueTree::Listener ,
	public juce :: Timer
{
public:
	compiler(juce::ValueTree& tree) : _tree(tree)
	{
		_cSourceTree = tree.getOrCreateChildWithName("cSource", nullptr);
		_cSourceTree.addListener(this);

		_asmTree = tree.getOrCreateChildWithName("asmOpCode", nullptr);

		_thread.startThread();

		startTimer(100);
	}


	virtual void timerCallback() override
	{
		juce::Array<float> opcode;
		juce::String strAsm = _thread.getOpCode(opcode);
		if (opcode.size() > 0)
		{

			_asmTree.setProperty("asmOpCode", strAsm, nullptr);

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
	juce::ValueTree _asmTree;

	DemoThread _thread;
};




