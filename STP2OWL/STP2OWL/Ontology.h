#pragma once

#include "Object.h"

class Class;
class ObjectProperty;
class DataProperty;
class Individual;
class DataType;
class AnnotationProperty;

class Ontology : public Object
{
public:
	Ontology(string name);
	~Ontology();

	void AddClass(string name, Class*& cls);
	Class* GetClassByName(string name);
	int GetClassSize() { return (int)m_classes.size(); }
	Class* GetClassAt(int index) { return m_classesVec[index]; }

	void AddObjectProperty(string name, ObjectProperty*& objProp);
	ObjectProperty* GetObjectPropertyByName(string name);
	int GetObjectPropertySize() { return (int)m_objectProperties.size(); }
	ObjectProperty* GetObjectPropertyAt(int index) { return m_objectPropertiesVec[index]; }

	void AddDataProperty(string name, DataProperty*& datProp);
	DataProperty* GetDataPropertyByName(string name);
	int GetDataPropertySize() { return (int)m_dataProperties.size(); }
	DataProperty* GetDataPropertyAt(int index) { return m_dataPropertiesVec[index]; }

	void AddIndividual(string name, Individual*& indvl);
	Individual* GetIndividualByName(string name);
	int GetIndividualSize() { return (int)m_individuals.size(); }
	Individual* GetIndividualAt(int index) { return m_individualsVec[index]; }

	void AddDataType(string name, DataType*& datType);
	DataType* GetDataTypeByName(string name);
	int GetDataTypeSize() { return (int)m_dataTypes.size(); }
	DataType* GetDataTypeAt(int index) { return m_dataTypesVec[index]; }

	void AddAnnotationProperty(string name, AnnotationProperty*& annoProp);
	AnnotationProperty* GetAnnotationPropertyByName(string name);
	int GetAnnotationPropertySize() { return (int)m_annotationProperty.size(); }
	AnnotationProperty* GetAnnotationPropertyAt(int index) { return m_annotationPropertyVec[index]; }

protected:
	void Clear();

private:
	// for name indexing.
	map<string, Class*> m_classes;
	map<string, ObjectProperty*> m_objectProperties;
	map<string, DataProperty*> m_dataProperties;
	map<string, Individual*> m_individuals;
	map<string, DataType*> m_dataTypes;
	map<string, AnnotationProperty*> m_annotationProperty;

	// for number indexing, traverse.
	vector<Class*> m_classesVec;
	vector<ObjectProperty*> m_objectPropertiesVec;
	vector<DataProperty*> m_dataPropertiesVec;
	vector<Individual*> m_individualsVec;
	vector<DataType*> m_dataTypesVec;
	vector<AnnotationProperty*> m_annotationPropertyVec;
};