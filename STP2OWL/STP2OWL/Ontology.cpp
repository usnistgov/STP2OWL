#include "stdafx.h"
#include "Ontology.h"

#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"

Ontology::Ontology(const string& name)
	:Object(name)
{
}

Ontology::~Ontology()
{
	Clear();
}


void Ontology::AddClass(const string& name, Class*& cls)
{
	cls = GetClassByName(name);
	
	if (cls != nullptr)
		return;

	cls = new Class(name);

	m_classesMap.emplace(name, cls);
	m_classesVec.emplace_back(cls);
}

void Ontology::AddObjectProperty(const string& name, ObjectProperty*& objProp)
{
	objProp = GetObjectPropertyByName(name);
	
	if (objProp != nullptr)
		return;

	objProp = new ObjectProperty(name);

	m_objectPropertiesMap.emplace(name, objProp);
	m_objectPropertiesVec.emplace_back(objProp);
}

void Ontology::AddDataProperty(const string& name, DataProperty*& datProp)
{
	datProp = GetDataPropertyByName(name);

	if (datProp != nullptr)
		return;

	datProp = new DataProperty(name);

	m_dataPropertiesMap.emplace(name, datProp);
	m_dataPropertiesVec.emplace_back(datProp);
}

void Ontology::AddIndividual(const string& name, Individual*& indvl)
{
	indvl = GetIndividualByName(name);

	if (indvl != nullptr)
		return;

	indvl = new Individual(name);

	m_individualsMap.emplace(make_pair( name, indvl ));
	m_individualsVec.emplace_back(indvl);
}

void Ontology::AddDataType(const string& name, DataType*& datType)
{
	datType = GetDataTypeByName(name);

	if (datType != nullptr)
		return;

	datType = new DataType(name);

	m_dataTypesMap.emplace(name, datType);
	m_dataTypesVec.emplace_back(datType);
}

void Ontology::AddAnnotationProperty(const string& name, AnnotationProperty*& annoProp)
{
	annoProp = GetAnnotationPropertyByName(name);

	if (annoProp != nullptr)
		return;

	annoProp = new AnnotationProperty(name);

	m_annotationPropertyMap.emplace(name, annoProp);
	m_annotationPropertyVec.emplace_back(annoProp);
}


Class* Ontology::GetClassByName(const string& name) const
{
	Class* cls = nullptr;
	
	if (m_classesMap.find(name) != m_classesMap.end())
		cls = m_classesMap.find(name)->second;
	
	return cls;
}

ObjectProperty* Ontology::GetObjectPropertyByName(const string& name) const
{
	ObjectProperty* objProp = nullptr;

	if (m_objectPropertiesMap.find(name) != m_objectPropertiesMap.end())
		objProp = m_objectPropertiesMap.find(name)->second;

	return objProp;
}

DataProperty* Ontology::GetDataPropertyByName(const string& name) const
{
	DataProperty* datProp = nullptr;

	if (m_dataPropertiesMap.find(name) != m_dataPropertiesMap.end())
		datProp = m_dataPropertiesMap.find(name)->second;

	return datProp;
}

Individual* Ontology::GetIndividualByName(const string& name) const
{
	Individual* indvl = nullptr;

	if (m_individualsMap.find(name) != m_individualsMap.end())
		indvl = m_individualsMap.find(name)->second;

	return indvl;
}

DataType* Ontology::GetDataTypeByName(const string& name) const
{
	DataType* datType = nullptr;

	if (m_dataTypesMap.find(name) != m_dataTypesMap.end())
		datType = m_dataTypesMap.find(name)->second;

	return datType;
}

AnnotationProperty* Ontology::GetAnnotationPropertyByName(const string& name) const
{
	AnnotationProperty* annoProp = nullptr;

	if (m_annotationPropertyMap.find(name) != m_annotationPropertyMap.end())
		annoProp = m_annotationPropertyMap.find(name)->second;

	return annoProp;
}



void Ontology::Clear()
{
	for (auto it = begin(m_classesMap); it != end(m_classesMap); )
	{
		delete it->second;
		it->second = nullptr;
		m_classesMap.erase(it++);
	}
	
	for (auto it = begin(m_objectPropertiesMap); it != end(m_objectPropertiesMap); )
	{
		delete it->second;
		it->second = nullptr;
		m_objectPropertiesMap.erase(it++);
	}

	for (auto it = begin(m_dataPropertiesMap); it != end(m_dataPropertiesMap); )
	{
		delete it->second;
		it->second = nullptr;
		m_dataPropertiesMap.erase(it++);
	}

	for (auto it = begin(m_individualsMap); it != end(m_individualsMap); )
	{
		delete it->second;
		it->second = nullptr;
		m_individualsMap.erase(it++);
	}

	for (auto it = begin(m_dataTypesMap); it != end(m_dataTypesMap); )
	{
		delete it->second;
		it->second = nullptr;
		m_dataTypesMap.erase(it++);
	}

	for (auto it = begin(m_annotationPropertyMap); it != end(m_annotationPropertyMap); )
	{
		delete it->second;
		it->second = nullptr;
		m_annotationPropertyMap.erase(it++);
	}

	m_classesVec.clear();
	m_objectPropertiesVec.clear();
	m_dataPropertiesVec.clear();
	m_individualsVec.clear();
	m_dataTypesVec.clear();
	m_annotationPropertyVec.clear();

	m_classesVec.shrink_to_fit();
	m_objectPropertiesVec.shrink_to_fit();
	m_dataPropertiesVec.shrink_to_fit();
	m_individualsVec.shrink_to_fit();
	m_dataTypesVec.shrink_to_fit();
	m_annotationPropertyVec.shrink_to_fit();
}
