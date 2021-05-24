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
	m_superClasses.push_back(superClass);
}

void Class::AddObjectProperty(ObjectProperty* objectProperty)
{
	assert(objectProperty);
	m_objectProperties.push_back(objectProperty);
}

void Class::AddObjectPropertyCardinality(ObjectProperty* objectProperty, int min, int max)
{
	assert(objectProperty);
	
	m_objPropMinCard.insert({ objectProperty , min});
	m_objPropMaxCard.insert({ objectProperty , max });	
}

void Class::AddDataProperty(DataProperty* dataProperty)
{
	assert(dataProperty);
	m_dataProperties.push_back(dataProperty);
}

void Class::AddDataPropertyCardinality(DataProperty* dataProperty, int min, int max)
{
	assert(dataProperty);
	
	m_datPropMinCard.insert({ dataProperty , min });
	m_datPropMaxCard.insert({ dataProperty , max });;
}

void Class::AddEquivalentClass(Class* cls)
{
	assert(cls);
	m_equivalentClasses.push_back(cls);
}

void Class::AddObjectOneOf(Individual* indvl)
{
	assert(indvl);
	m_objectOneOf.push_back(indvl);
}

void Class::AddObjectUnionOf(Class* cls)
{
	assert(cls);
	m_objectUnionOf.push_back(cls);
}

void Class::AddObjectIntersectionOf(Class* cls)
{
	assert(cls);
	m_objectIntersectionOf.push_back(cls);
}

void Class::AddDisjointUnionOf(Class* cls)
{
	assert(cls);
	m_disjointUnionOf.push_back(cls);
}

void Class::AddDisjointClass(Class* cls)
{
	assert(cls);
	m_disjointClasses.push_back(cls);
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
