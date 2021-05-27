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
	Individual(const string& name);
	~Individual();

	void AddClassType(Class* cls);
	const int GetClassTypeSize() const { return (int)m_classTypes.size(); }
	Class* GetClassTypeAt(const int& index) const { return m_classTypes[index]; }

	void AddObjPropertyAssertion(ObjectProperty* objProp, Individual* indvl);
	const int GetObjPropertyAssertionSize() const { return (int)m_objectPropertyAssertions.size(); }
	ObjPropertyAssertion GetObjPropertyAssertionAt(const int& index) const { return m_objectPropertyAssertions[index]; }

	void AddDataPropertyAssertion(DataProperty* datProp, string str);
	const int GetDataPropertyAssertionSize() const { return (int)m_dataPropertyAssertions.size(); }
	DataPropertyAssertion GetDataPropertyAssertionAt(const int& index) const { return m_dataPropertyAssertions[index]; }

protected:
	void Clear();

private:
	vector<Class*> m_classTypes;
	unordered_map<string, Class*> m_classNameTypeMap;
	vector<ObjPropertyAssertion> m_objectPropertyAssertions;
	vector<DataPropertyAssertion> m_dataPropertyAssertions;
};