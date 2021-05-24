#pragma once

#include "Object.h"

class ObjectProperty;
class DataProperty;
class Individual;

class Class : public Object
{
public:
	Class(string name);
	~Class();

	void AddSuperClass(Class* superClass);
	int GetSuperClassSize() { return (int)m_superClasses.size(); }
	Class* GetSuperClassAt(int index) { return m_superClasses[index]; }

	void AddObjectProperty(ObjectProperty* objectProperty);
	int GetObjectPropertySize() { return (int)m_objectProperties.size(); }
	ObjectProperty* GetObjectPropertyAt(int index) { return m_objectProperties[index]; }
	void AddObjectPropertyCardinality(ObjectProperty* objectProperty, int min, int max);

	void AddDataProperty(DataProperty* dataProperty);
	int GetDataPropertySize() { return (int)m_dataProperties.size(); }
	DataProperty* GetDataPropertyAt(int index) { return m_dataProperties[index]; }
	void AddDataPropertyCardinality(DataProperty* dataProperty, int min, int max);
	
	void AddEquivalentClass(Class* cls);
	int GetEquivalentClassSize() { return (int)m_equivalentClasses.size(); }
	Class* GetEquivalentClassAt(int index) { return m_equivalentClasses[index]; }

	void AddObjectOneOf(Individual* indvl);
	int GetObjectOneOfSize() { return (int)m_objectOneOf.size(); }
	Individual* GetObjectOneOfAt(int index) { return m_objectOneOf[index]; }

	void AddObjectUnionOf(Class* cls);
	int GetObjectUnionOfSize() { return (int)m_objectUnionOf.size(); }
	Class* GetObjectUnionOfAt(int index) { return m_objectUnionOf[index]; }
	
	void AddObjectIntersectionOf(Class* cls);
	int GetObjectIntersectionOfSize() { return (int)m_objectIntersectionOf.size(); }
	Class* GetObjectIntersectionOfAt(int index) { return m_objectIntersectionOf[index]; }

	void AddDisjointUnionOf(Class* cls);
	int GetDisjointUnionOfSize() { return (int)m_disjointUnionOf.size(); }
	Class* GetDisjointUnionOfAt(int index) { return m_disjointUnionOf[index]; }

	void AddDisjointClass(Class* cls);
	int GetDisjointClassSize() { return (int)m_disjointClasses.size(); }
	Class* GetDisjointClassAt(int index) { return m_disjointClasses[index]; }

	int GetObjPropMinCardinarlity(ObjectProperty* objProp) { return m_objPropMinCard[objProp]; }
	int GetObjPropMaxCardinarlity(ObjectProperty* objProp) { return m_objPropMaxCard[objProp]; }

	int GetDataPropMinCardinarlity(DataProperty* dataProp) { return m_datPropMinCard[dataProp]; }
	int GetDataPropMaxCardinarlity(DataProperty* dataProp) { return m_datPropMaxCard[dataProp]; }

protected:
	void Clear();

private:
	vector<Class*> m_superClasses;
	vector<ObjectProperty*> m_objectProperties;
	vector<DataProperty*> m_dataProperties;
	vector<Individual*> m_namedIndividuals;

	map<ObjectProperty*, int> m_objPropMinCard, m_objPropMaxCard;
	map<DataProperty*, int> m_datPropMinCard, m_datPropMaxCard;

	vector<Class*> m_equivalentClasses;
	vector<Class*> m_objectUnionOf;			// ANDOR
	vector<Class*> m_objectIntersectionOf;	// AND
	vector<Class*> m_disjointUnionOf;		// ONEOF, Select
	vector<Class*> m_disjointClasses;		// ONEOF, Select

	vector<Individual*> m_objectOneOf;		// Enum
};