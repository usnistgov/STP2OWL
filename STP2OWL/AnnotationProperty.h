#pragma once

#include "Object.h"

class Class;
class DataType;

class AnnotationProperty : public Object
{
public:
	AnnotationProperty(string name);
	~AnnotationProperty();

	void AddDomain(Class* domain);
	int GetDomainSize() { return (int)m_domains.size(); }
	Class* GetDomainAt(int index) { return m_domains[index]; }

	void AddRange(DataType* range);
	int GetRangeSize() { return (int)m_ranges.size(); }
	DataType* GetRangeAt(int index) { return m_ranges[index]; }

protected:
	void Clear();

private:
	vector<Class*> m_domains;
	vector<DataType*> m_ranges;
};