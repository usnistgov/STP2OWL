#pragma once

#include "Object.h"

class Class;
class DataType;

class AnnotationProperty : public Object
{
public:
	AnnotationProperty(const string& name);
	~AnnotationProperty();

	void AddDomain(Class* domain);
	const int GetDomainSize() const { return (int)m_domains.size(); }
	Class* GetDomainAt(const int& index) const { return m_domains[index]; }

	void AddRange(DataType* range);
	const int GetRangeSize() const { return (int)m_ranges.size(); }
	DataType* GetRangeAt(const int& index) const { return m_ranges[index]; }

protected:
	void Clear();

private:
	vector<Class*> m_domains;
	vector<DataType*> m_ranges;
};