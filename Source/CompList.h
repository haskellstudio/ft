/*
  ==============================================================================

    CompList.h
    Created: 10 Nov 2017 5:10:40pm
    Author:  sh

  ==============================================================================
*/

#pragma once



class TCompType
{
public:
	virtual Component* createComponent() = 0;
	TCompType(const String &n) : name(n)
	{
		getCompTypeList().set(n, (TCompType*)this);

	}


	static HashMap <String, TCompType*>& getCompTypeList()
	{
		static HashMap <String, TCompType*> list;
		//static Array<TCompType*> list;
		return list;
	}
	String name;
	int index;
};



template <class TComponent>
class ComponentList : TCompType
{
public:
	ComponentList(const  String & n) : TCompType(n)
	{
	}

	virtual Component* createComponent()
	{
		return new TComponent();
	}
};
