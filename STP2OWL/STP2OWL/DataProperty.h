#pragma once

#include "Object.h"

class Class;
class DataType;

class DataProperty : public Object
{
public:
	DataProperty(const string& name);
	~DataProperty();

	void AddDomain(Class* domain);
	const int GetDomainSize() const { return (int)m_domains.size(); }
	Class* GetDomainAt(const int& index) const { return m_domains[index]; }

	void AddRange(DataType* range);
	const int GetRangeSize() const { return (int)m_ranges.size(); }
	DataType* GetRangeAt(const int& index) const { return m_ranges[index]; }
	
	void SetFunctional(bool functional) { m_isFunctional = functional; }
	bool IsFunctional() const { return m_isFunctional; }

protected:
	void Clear();

private:
	vector<Class*> m_domains;
	vector<DataType*> m_ranges;

	vector<Class*> m_objectUnionOf;			// ANDOR

	bool m_isFunctional;
};