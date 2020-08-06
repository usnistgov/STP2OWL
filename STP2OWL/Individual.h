#pragma once

#include "Object.h"

class Class;
class ObjectProperty;
class DataProperty;
class Individual;

typedef pair<ObjectProperty*, Individual*> ObjPropertyAssertion;
typedef pair<DataProperty*, string> DataPropertyAssertion;

class Individual : public Object
{
public:
	Individual(string name);
	~Individual();

	void AddClassType(Class* cls);
	int GetClassTypeSize() { return (int)m_classTypes.size(); }
	Class* GetClassTypeAt(int index) { return m_classTypes[index]; }

	void AddObjPropertyAssertion(ObjectProperty* objProp, Individual* indvl);
	int GetObjPropertyAssertionSize() { return (int)m_objectPropertyAssertions.size(); }
	ObjPropertyAssertion GetObjPropertyAssertionAt(int index) { return m_objectPropertyAssertions[index]; }

	void AddDataPropertyAssertion(DataProperty* datProp, string str);
	int GetDataPropertyAssertionSize() { return (int)m_dataPropertyAssertions.size(); }
	DataPropertyAssertion GetDataPropertyAssertionAt(int index) { return m_dataPropertyAssertions[index]; }

protected:
	void Clear();

private:
	vector<Class*> m_classTypes;
	map<string, Class*> m_classNameTypeMap;
	vector<ObjPropertyAssertion> m_objectPropertyAssertions;
	vector<DataPropertyAssertion> m_dataPropertyAssertions;
};