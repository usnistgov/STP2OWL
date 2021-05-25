#include "stdafx.h"
#include "Class.h"

Class::Class(string name)
	:Object(name)
{
}

Class::~Class()
{
	Clear();
}

void Class::AddSuperClass(Class* superClass)
{
	assert(superClass);
	m_superClasses.emplace_back(superClass);
}

void Class::AddObjectProperty(ObjectProperty* objectProperty)
{
	assert(objectProperty);
	m_objectProperties.emplace_back(objectProperty);
}

void Class::AddObjectPropertyCardinality(ObjectProperty* objectProperty, int min, int max)
{
	assert(objectProperty);
	
	m_objPropMinCard.emplace(objectProperty, min);
	m_objPropMaxCard.emplace(objectProperty, max);
}

void Class::AddDataProperty(DataProperty* dataProperty)
{
	assert(dataProperty);
	m_dataProperties.emplace_back(dataProperty);
}

void Class::AddDataPropertyCardinality(DataProperty* dataProperty, int min, int max)
{
	assert(dataProperty);
	
	m_datPropMinCard.emplace(dataProperty, min);
	m_datPropMaxCard.emplace(dataProperty, max);
}

void Class::AddEquivalentClass(Class* cls)
{
	assert(cls);
	m_equivalentClasses.emplace_back(cls);
}

void Class::AddObjectOneOf(Individual* indvl)
{
	assert(indvl);
	m_objectOneOf.emplace_back(indvl);
}

void Class::AddObjectUnionOf(Class* cls)
{
	assert(cls);
	m_objectUnionOf.emplace_back(cls);
}

void Class::AddObjectIntersectionOf(Class* cls)
{
	assert(cls);
	m_objectIntersectionOf.emplace_back(cls);
}

void Class::AddDisjointUnionOf(Class* cls)
{
	assert(cls);
	m_disjointUnionOf.emplace_back(cls);
}

void Class::AddDisjointClass(Class* cls)
{
	assert(cls);
	m_disjointClasses.emplace_back(cls);
}

void Class::Clear()
{
	m_superClasses.clear();
	m_objectProperties.clear();
	m_dataProperties.clear();
	m_namedIndividuals.clear();

	m_objPropMinCard.clear();
	m_objPropMaxCard.clear();
	m_datPropMinCard.clear();
	m_datPropMaxCard.clear();

	m_equivalentClasses.clear();

	m_objectOneOf.clear();
	m_objectUnionOf.clear();
	m_objectIntersectionOf.clear();
	m_disjointUnionOf.clear();
}
