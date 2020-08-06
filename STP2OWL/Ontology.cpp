#include "stdafx.h"
#include "Ontology.h"

#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"

Ontology::Ontology(string name)
	:Object(name)
{
}

Ontology::~Ontology()
{
	Clear();
}


void Ontology::AddClass(string name, Class*& cls)
{
	cls = GetClassByName(name);
	
	if (cls != nullptr)
		return;

	cls = new Class(name);

	m_classes.insert({ name, cls });
	m_classesVec.push_back(cls);
}

void Ontology::AddObjectProperty(string name, ObjectProperty*& objProp)
{
	objProp = GetObjectPropertyByName(name);
	
	if (objProp != nullptr)
		return;

	objProp = new ObjectProperty(name);

	m_objectProperties.insert({ name, objProp });
	m_objectPropertiesVec.push_back(objProp);
}

void Ontology::AddDataProperty(string name, DataProperty*& datProp)
{
	datProp = GetDataPropertyByName(name);

	if (datProp != nullptr)
		return;

	datProp = new DataProperty(name);

	m_dataProperties.insert({ name, datProp });
	m_dataPropertiesVec.push_back(datProp);
}

void Ontology::AddIndividual(string name, Individual*& indvl)
{
	indvl = GetIndividualByName(name);

	if (indvl != nullptr)
		return;

	indvl = new Individual(name);

	m_individuals.insert(make_pair( name, indvl ));
	m_individualsVec.push_back(indvl);
}

void Ontology::AddDataType(string name, DataType*& datType)
{
	datType = GetDataTypeByName(name);

	if (datType != nullptr)
		return;

	datType = new DataType(name);

	m_dataTypes.insert({ name, datType });
	m_dataTypesVec.push_back(datType);
}

void Ontology::AddAnnotationProperty(string name, AnnotationProperty*& annoProp)
{
	annoProp = GetAnnotationPropertyByName(name);

	if (annoProp != nullptr)
		return;

	annoProp = new AnnotationProperty(name);

	m_annotationProperty.insert({ name, annoProp });
	m_annotationPropertyVec.push_back(annoProp);
}


Class* Ontology::GetClassByName(string name)
{
	Class* cls = nullptr;
	
	if (m_classes.find(name) != m_classes.end())
		cls = m_classes.find(name)->second;
	
	return cls;
}

ObjectProperty* Ontology::GetObjectPropertyByName(string name)
{
	ObjectProperty* objProp = nullptr;

	if (m_objectProperties.find(name) != m_objectProperties.end())
		objProp = m_objectProperties.find(name)->second;

	return objProp;
}

DataProperty* Ontology::GetDataPropertyByName(string name)
{
	DataProperty* datProp = nullptr;

	if (m_dataProperties.find(name) != m_dataProperties.end())
		datProp = m_dataProperties.find(name)->second;

	return datProp;
}

Individual* Ontology::GetIndividualByName(string name)
{
	Individual* indvl = nullptr;

	if (m_individuals.find(name) != m_individuals.end())
		indvl = m_individuals.find(name)->second;

	return indvl;
}

DataType* Ontology::GetDataTypeByName(string name)
{
	DataType* datType = nullptr;

	if (m_dataTypes.find(name) != m_dataTypes.end())
		datType = m_dataTypes.find(name)->second;

	return datType;
}

AnnotationProperty* Ontology::GetAnnotationPropertyByName(string name)
{
	AnnotationProperty* annoProp = nullptr;

	if (m_annotationProperty.find(name) != m_annotationProperty.end())
		annoProp = m_annotationProperty.find(name)->second;

	return annoProp;
}



void Ontology::Clear()
{
	for (auto it = begin(m_classes); it != end(m_classes); )
	{
		delete it->second;
		m_classes.erase(it++);
	}
	
	for (auto it = begin(m_objectProperties); it != end(m_objectProperties); )
	{
		delete it->second;
		m_objectProperties.erase(it++);
	}

	for (auto it = begin(m_dataProperties); it != end(m_dataProperties); )
	{
		delete it->second;
		m_dataProperties.erase(it++);
	}

	for (auto it = begin(m_individuals); it != end(m_individuals); )
	{
		delete it->second;
		m_individuals.erase(it++);
	}

	for (auto it = begin(m_dataTypes); it != end(m_dataTypes); )
	{
		delete it->second;
		m_dataTypes.erase(it++);
	}

	for (auto it = begin(m_annotationProperty); it != end(m_annotationProperty); )
	{
		delete it->second;
		m_annotationProperty.erase(it++);
	}

	m_classesVec.clear(); 
	m_objectPropertiesVec.clear();
	m_dataPropertiesVec.clear();
	m_individualsVec.clear();
	m_dataTypesVec.clear();
	m_annotationPropertyVec.clear();
}
