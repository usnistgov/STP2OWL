#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::Object(const string& name)
	:m_name(name)
{
}

Object::~Object()
{
	Clear();
}

const string Object::GetAnnotationStringByAnnotationProperty(AnnotationProperty* annoProp) const
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
	m_annotations.shrink_to_fit();
}
