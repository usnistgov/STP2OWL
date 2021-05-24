#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::Object(string name)
	:m_name(name)
{
}

Object::~Object()
{
	Clear();
}

string Object::GetAnnotationStringByAnnotationProperty(AnnotationProperty* annoProp)
{
	string annoStr;
	
	for (int i = 0; i < m_annotations.size(); ++i)
	{
		Annotation anno = m_annotations[i];

		if (anno.first == annoProp)
		{
			annoStr = anno.second;
			return annoStr; // Each object has one annoStr for a given annoProp.
		}
	}

	return annoStr;
}

void Object::Clear()
{
	m_annotations.clear();
}
