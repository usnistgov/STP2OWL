#include "stdafx.h"
#include "AnnotationProperty.h"

AnnotationProperty::AnnotationProperty(const string& name)
	:Object(name)
{
}

AnnotationProperty::~AnnotationProperty()
{
	Clear();
}

void AnnotationProperty::AddDomain(Class* domain)
{
	assert(domain);
	m_domains.emplace_back(domain);
}

void AnnotationProperty::AddRange(DataType* range)
{
	assert(range);
	m_ranges.emplace_back(range);
}

void AnnotationProperty::Clear()
{
	m_domains.clear();
	m_ranges.clear();

	m_domains.shrink_to_fit();
	m_ranges.shrink_to_fit();
}
