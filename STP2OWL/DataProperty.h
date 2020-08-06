#pragma once

#include "Object.h"

class Class;
class DataType;

class DataProperty : public Object
{
public:
	DataProperty(string name);
	~DataProperty();

	void AddDomain(Class* domain);
	int GetDomainSize() { return (int)m_domains.size(); }
	Class* GetDomainAt(int index) { return m_domains[index]; }

	void AddRange(DataType* range);
	int GetRangeSize() { return (int)m_ranges.size(); }
	DataType* GetRangeAt(int index) { return m_ranges[index]; }
	
	void SetFunctional(bool functional) { m_isFunctional = functional; }
	bool IsFunctional() { return m_isFunctional; }

protected:
	void Clear();

private:
	vector<Class*> m_domains;
	vector<DataType*> m_ranges;

	vector<Class*> m_objectUnionOf;			// ANDOR

	bool m_isFunctional;
};