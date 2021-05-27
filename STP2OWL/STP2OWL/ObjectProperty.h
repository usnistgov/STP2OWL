#pragma once

#include "Object.h"

class Class;

class ObjectProperty : public Object
{
public:
	ObjectProperty(const string& name);
	~ObjectProperty();

	void AddDomain(Class* domain);
	const int GetDomainSize() const { return (int)m_domains.size(); }
	Class* GetDomainAt(const int& index) const { return m_domains[index]; }

	void AddRange(Class* range);
	const int GetRangeSize() const { return (int)m_ranges.size(); }
	Class* GetRangeAt(const int& index) const { return m_ranges[index]; }

	void AddSuperProperty(ObjectProperty* superProperty);
	const int GetSuperPropertySize() const { return (int)m_superProperties.size(); }
	ObjectProperty* GetSuperPropertyAt(const int& index) const { return m_superProperties[index]; }
	
	void AddInverseProperty(ObjectProperty* inverseProperty);
	const int GetInversePropertySize() const { return (int)m_inverseProperties.size(); }
	ObjectProperty* GetInversePropertyAt(const int& index) const { return m_inverseProperties[index]; }

	void SetFunctional(bool functional) { m_isFunctional = functional; }
	bool IsFunctional() const { return m_isFunctional; }
	
	void SetInverseFunctional(bool inverseFunctional) { m_isInverseFunctional = inverseFunctional; }
	bool IsInverseFunctional() const { return m_isInverseFunctional; }

	void SetTransitive(bool transitive) { m_isTransitive = transitive; }
	bool IsTransitive() const { return m_isTransitive; }

	void SetSymmetric(bool symmetric) { m_isSymmetric = symmetric; }
	bool IsSymmetric() const { return m_isSymmetric; }

	void SetAsymmetric(bool asymmetric) { m_isAsymmetric = asymmetric; }
	bool IsAsymmetric() const { return m_isAsymmetric; }

	void SetReflexive(bool reflexive) { m_isReflexive = reflexive; }
	bool IsReflexive() const { return m_isReflexive; }

	void SetIrreflieive(bool irreflieive) { m_isIrreflieive = irreflieive; }
	bool IsIrreflieive() const { return m_isIrreflieive; }

protected:
	void Clear();

private:
	vector<Class*> m_domains;
	vector<Class*> m_ranges;

	vector<ObjectProperty*> m_superProperties;
	vector<ObjectProperty*> m_inverseProperties;

	bool m_isFunctional, m_isInverseFunctional, m_isTransitive, m_isSymmetric, m_isAsymmetric, m_isReflexive, m_isIrreflieive;
};