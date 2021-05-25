#include "stdafx.h"
#include "DataProperty.h"

DataProperty::DataProperty(string name)
	:Object(name),
	m_isFunctional(false)
{
}

DataProperty::~DataProperty()
{
	Clear();
}

void DataProperty::AddDomain(Class* domain)
{
	assert(domain);
	m_domains.emplace_back(domain);
}

void DataProperty::AddRange(DataType* range)
{
	assert(range);
	m_ranges.emplace_back(range);
}

void DataProperty::Clear()
{
	m_domains.clear();
	m_ranges.clear();
}
