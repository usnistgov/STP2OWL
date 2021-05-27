#pragma once

#include "Object.h"

class ObjectProperty;
class DataProperty;
class Individual;

class Class : public Object
{
public:
	Class(const string& name);
	~Class();

	void AddSuperClass(Class*& superClass);
	const int GetSuperClassSize() const { return (int)m_superClasses.size(); }
	Class* GetSuperClassAt(const int& index) const { return m_superClasses[index]; }

	void AddObjectProperty(ObjectProperty*& objectProperty);
	const int GetObjectPropertySize() const { return (int)m_objectProperties.size(); }
	ObjectProperty* GetObjectPropertyAt(const int& index) const { return m_objectProperties[index]; }
	void AddObjectPropertyCardinality(ObjectProperty*& objectProperty, int min, int max);

	void AddDataProperty(DataProperty*& dataProperty);
	const int GetDataPropertySize() const { return (int)m_dataProperties.size(); }
	DataProperty* GetDataPropertyAt(const int& index) const { return m_dataProperties[index]; }
	void AddDataPropertyCardinality(DataProperty*& dataProperty, int min, int max);
	
	void AddEquivalentClass(Class*& cls);
	const int GetEquivalentClassSize() const { return (int)m_equivalentClasses.size(); }
	Class* GetEquivalentClassAt(const int& index) const { return m_equivalentClasses[index]; }

	void AddObjectOneOf(Individual*& indvl);
	const int GetObjectOneOfSize() const { return (int)m_objectOneOf.size(); }
	Individual* GetObjectOneOfAt(const int& index) const { return m_objectOneOf[index]; }

	void AddObjectUnionOf(Class*& cls);
	const int GetObjectUnionOfSize() const { return (int)m_objectUnionOf.size(); }
	Class* GetObjectUnionOfAt(const int& index) const { return m_objectUnionOf[index]; }
	
	void AddObjectIntersectionOf(Class*& cls);
	const int GetObjectIntersectionOfSize() const { return (int)m_objectIntersectionOf.size(); }
	Class* GetObjectIntersectionOfAt(const int& index) const { return m_objectIntersectionOf[index]; }

	void AddDisjointUnionOf(Class*& cls);
	const int GetDisjointUnionOfSize() const { return (int)m_disjointUnionOf.size(); }
	Class* GetDisjointUnionOfAt(const int& index) const { return m_disjointUnionOf[index]; }

	void AddDisjointClass(Class*& cls);
	const int GetDisjointClassSize() const { return (int)m_disjointClasses.size(); }
	Class* GetDisjointClassAt(const int& index) const { return m_disjointClasses[index]; }

	const int& GetObjPropMinCardinarlity(ObjectProperty*& objProp) { return m_objPropMinCard[objProp]; }
	const int& GetObjPropMaxCardinarlity(ObjectProperty*& objProp) { return m_objPropMaxCard[objProp]; }

	const int& GetDataPropMinCardinarlity(DataProperty*& dataProp) { return m_datPropMinCard[dataProp]; }
	const int& GetDataPropMaxCardinarlity(DataProperty*& dataProp) { return m_datPropMaxCard[dataProp]; }

protected:
	void Clear();

private:
	vector<Class*> m_superClasses;
	vector<ObjectProperty*> m_objectProperties;
	vector<DataProperty*> m_dataProperties;
	vector<Individual*> m_namedIndividuals;

	unordered_map<ObjectProperty*, int> m_objPropMinCard, m_objPropMaxCard;
	unordered_map<DataProperty*, int> m_datPropMinCard, m_datPropMaxCard;

	vector<Class*> m_equivalentClasses;
	vector<Class*> m_objectUnionOf;			// ANDOR
	vector<Class*> m_objectIntersectionOf;	// AND
	vector<Class*> m_disjointUnionOf;		// ONEOF, Select
	vector<Class*> m_disjointClasses;		// ONEOF, Select

	vector<Individual*> m_objectOneOf;		// Enum
};