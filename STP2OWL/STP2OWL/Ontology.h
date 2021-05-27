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
	const int GetClassSize() const { return (int)m_classesMap.size(); }
	Class* GetClassByName(const string& name) const;
	Class* GetClassAt(const int& index) const { return m_classesVec[index]; }

	void AddObjectProperty(const string& name, ObjectProperty*& objProp);
	const int GetObjectPropertySize() const { return (int)m_objectPropertiesMap.size(); }
	ObjectProperty* GetObjectPropertyByName(const string& name) const;
	ObjectProperty* GetObjectPropertyAt(const int& index) const { return m_objectPropertiesVec[index]; }

	void AddDataProperty(const string& name, DataProperty*& datProp);
	const int GetDataPropertySize() const { return (int)m_dataPropertiesMap.size(); }
	DataProperty* GetDataPropertyByName(const string& name) const;
	DataProperty* GetDataPropertyAt(const int& index) const { return m_dataPropertiesVec[index]; }

	void AddIndividual(const string& name, Individual*& indvl);
	const int GetIndividualSize() const { return (int)m_individualsMap.size(); }
	Individual* GetIndividualByName(const string& name) const;
	Individual* GetIndividualAt(const int& index) const { return m_individualsVec[index]; }

	void AddDataType(const string& name, DataType*& datType);
	const int GetDataTypeSize() const { return (int)m_dataTypesMap.size(); }
	DataType* GetDataTypeByName(const string& name) const;
	DataType* GetDataTypeAt(const int& index) const { return m_dataTypesVec[index]; }

	void AddAnnotationProperty(const string& name, AnnotationProperty*& annoProp);
	const int GetAnnotationPropertySize() const { return (int)m_annotationPropertyMap.size(); }
	AnnotationProperty* GetAnnotationPropertyByName(const string& name) const;
	AnnotationProperty* GetAnnotationPropertyAt(const int& index) const { return m_annotationPropertyVec[index]; }

protected:
	void Clear();

private:
	// for name indexing.
	unordered_map<string, Class*> m_classesMap;
	unordered_map<string, ObjectProperty*> m_objectPropertiesMap;
	unordered_map<string, DataProperty*> m_dataPropertiesMap;
	unordered_map<string, Individual*> m_individualsMap;
	unordered_map<string, DataType*> m_dataTypesMap;
	unordered_map<string, AnnotationProperty*> m_annotationPropertyMap;

	// for number indexing, traverse.
	vector<Class*> m_classesVec;
	vector<ObjectProperty*> m_objectPropertiesVec;
	vector<DataProperty*> m_dataPropertiesVec;
	vector<Individual*> m_individualsVec;
	vector<DataType*> m_dataTypesVec;
	vector<AnnotationProperty*> m_annotationPropertyVec;
};