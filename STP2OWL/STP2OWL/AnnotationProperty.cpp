#include "stdafx.h"
#include "AnnotationProperty.h"

AnnotationProperty::AnnotationProperty(string name)
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
}
