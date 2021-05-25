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

	m_classes.emplace(name, cls);
	m_classesVec.emplace_back(cls);
}

void Ontology::AddObjectProperty(const string& name, ObjectProperty*& objProp)
{
	objProp = GetObjectPropertyByName(name);
	
	if (objProp != nullptr)
		return;

	objProp = new ObjectProperty(name);

	m_objectProperties.emplace(name, objProp);
	m_objectPropertiesVec.emplace_back(objProp);
}

void Ontology::AddDataProperty(const string& name, DataProperty*& datProp)
{
	datProp = GetDataPropertyByName(name);

	if (datProp != nullptr)
		return;

	datProp = new DataProperty(name);

	m_dataProperties.emplace(name, datProp);
	m_dataPropertiesVec.emplace_back(datProp);
}

void Ontology::AddIndividual(const string& name, Individual*& indvl)
{
	indvl = GetIndividualByName(name);

	if (indvl != nullptr)
		return;

	indvl = new Individual(name);

	m_individuals.emplace(make_pair( name, indvl ));
	m_individualsVec.emplace_back(indvl);
}

void Ontology::AddDataType(const string& name, DataType*& datType)
{
	datType = GetDataTypeByName(name);

	if (datType != nullptr)
		return;

	datType = new DataType(name);

	m_dataTypes.emplace(name, datType);
	m_dataTypesVec.emplace_back(datType);
}

void Ontology::AddAnnotationProperty(const string& name, AnnotationProperty*& annoProp)
{
	annoProp = GetAnnotationPropertyByName(name);

	if (annoProp != nullptr)
		return;

	annoProp = new AnnotationProperty(name);

	m_annotationProperty.emplace(name, annoProp);
	m_annotationPropertyVec.emplace_back(annoProp);
}


Class* Ontology::GetClassByName(const string& name)
{
	Class* cls = nullptr;
	
	if (m_classes.find(name) != m_classes.end())
		cls = m_classes.find(name)->second;
	
	return cls;
}

ObjectProperty* Ontology::GetObjectPropertyByName(const string& name)
{
	ObjectProperty* objProp = nullptr;

	if (m_objectProperties.find(name) != m_objectProperties.end())
		objProp = m_objectProperties.find(name)->second;

	return objProp;
}

DataProperty* Ontology::GetDataPropertyByName(const string& name)
{
	DataProperty* datProp = nullptr;

	if (m_dataProperties.find(name) != m_dataProperties.end())
		datProp = m_dataProperties.find(name)->second;

	return datProp;
}

Individual* Ontology::GetIndividualByName(const string& name)
{
	Individual* indvl = nullptr;

	if (m_individuals.find(name) != m_individuals.end())
		indvl = m_individuals.find(name)->second;

	return indvl;
}

DataType* Ontology::GetDataTypeByName(const string& name)
{
	DataType* datType = nullptr;

	if (m_dataTypes.find(name) != m_dataTypes.end())
		datType = m_dataTypes.find(name)->second;

	return datType;
}

AnnotationProperty* Ontology::GetAnnotationPropertyByName(const string& name)
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
