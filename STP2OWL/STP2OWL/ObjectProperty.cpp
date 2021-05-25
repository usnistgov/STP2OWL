#include "stdafx.h"
#include "ObjectProperty.h"

ObjectProperty::ObjectProperty(string name)
	:Object(name),
	m_isFunctional(false),
	m_isInverseFunctional(false),
	m_isTransitive(false),
	m_isSymmetric(false),
	m_isAsymmetric(false),
	m_isReflexive(false),
	m_isIrreflieive(false)
{
}

ObjectProperty::~ObjectProperty()
{
	Clear();
}

void ObjectProperty::AddDomain(Class* domain)
{
	assert(domain);
	m_domains.emplace_back(domain);
}

void ObjectProperty::AddRange(Class* range)
{
	assert(range);
	m_ranges.emplace_back(range);
}

void ObjectProperty::AddSuperProperty(ObjectProperty* superProperty)
{
	assert(superProperty);
	m_superProperties.emplace_back(superProperty);
}

void ObjectProperty::AddInverseProperty(ObjectProperty* inverseProperty)
{
	assert(inverseProperty);
	m_inverseProperties.emplace_back(inverseProperty);
}

void ObjectProperty::Clear()
{
	m_domains.clear();
	m_ranges.clear();

	m_superProperties.clear();
	m_inverseProperties.clear();
}
