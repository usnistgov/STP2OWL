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
	Ontology(const string& name);
	~Ontology();

	void AddClass(const string& name, Class*& cls);
	Class* GetClassByName(const string& name);
	int GetClassSize() { return (int)m_classes.size(); }
	Class* GetClassAt(const int& index) { return m_classesVec[index]; }

	void AddObjectProperty(const string& name, ObjectProperty*& objProp);
	ObjectProperty* GetObjectPropertyByName(const string& name);
	int GetObjectPropertySize() { return (int)m_objectProperties.size(); }
	ObjectProperty* GetObjectPropertyAt(const int& index) { return m_objectPropertiesVec[index]; }

	void AddDataProperty(const string& name, DataProperty*& datProp);
	DataProperty* GetDataPropertyByName(const string& name);
	int GetDataPropertySize() { return (int)m_dataProperties.size(); }
	DataProperty* GetDataPropertyAt(const int& index) { return m_dataPropertiesVec[index]; }

	void AddIndividual(const string& name, Individual*& indvl);
	Individual* GetIndividualByName(const string& name);
	int GetIndividualSize() { return (int)m_individuals.size(); }
	Individual* GetIndividualAt(const int& index) { return m_individualsVec[index]; }

	void AddDataType(const string& name, DataType*& datType);
	DataType* GetDataTypeByName(const string& name);
	int GetDataTypeSize() { return (int)m_dataTypes.size(); }
	DataType* GetDataTypeAt(const int& index) { return m_dataTypesVec[index]; }

	void AddAnnotationProperty(const string& name, AnnotationProperty*& annoProp);
	AnnotationProperty* GetAnnotationPropertyByName(const string& name);
	int GetAnnotationPropertySize() { return (int)m_annotationProperty.size(); }
	AnnotationProperty* GetAnnotationPropertyAt(const int& index) { return m_annotationPropertyVec[index]; }

protected:
	void Clear();

private:
	// for name indexing.
	unordered_map<string, Class*> m_classes;
	unordered_map<string, ObjectProperty*> m_objectProperties;
	unordered_map<string, DataProperty*> m_dataProperties;
	unordered_map<string, Individual*> m_individuals;
	unordered_map<string, DataType*> m_dataTypes;
	unordered_map<string, AnnotationProperty*> m_annotationProperty;

	// for number indexing, traverse.
	vector<Class*> m_classesVec;
	vector<ObjectProperty*> m_objectPropertiesVec;
	vector<DataProperty*> m_dataPropertiesVec;
	vector<Individual*> m_individualsVec;
	vector<DataType*> m_dataTypesVec;
	vector<AnnotationProperty*> m_annotationPropertyVec;
};