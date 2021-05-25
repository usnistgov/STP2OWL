#include "stdafx.h"
#include "Individual.h"
#include "Class.h"

Individual::Individual(string name)
	:Object(name)
{
}

Individual::~Individual()
{
	Clear();
}

void Individual::AddClassType(Class* cls)
{
	assert(cls);

	if (m_classNameTypeMap.find(cls->GetName()) != m_classNameTypeMap.end())
		return;

	m_classTypes.emplace_back(cls);
	m_classNameTypeMap.emplace(cls->GetName(), cls);
}

void Individual::AddObjPropertyAssertion(ObjectProperty* objProp, Individual* indvl)
{
	assert(objProp);
	assert(indvl);
	m_objectPropertyAssertions.emplace_back(objProp, indvl);
}

void Individual::AddDataPropertyAssertion(DataProperty* datProp, string str)
{
	assert(datProp);
	m_dataPropertyAssertions.emplace_back(datProp, str);
}

void Individual::Clear()
{
	m_classTypes.clear();
	m_classNameTypeMap.clear();
	m_objectPropertyAssertions.clear();
	m_dataPropertyAssertions.clear();
}
