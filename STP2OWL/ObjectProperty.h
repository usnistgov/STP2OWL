#pragma once

#include "Object.h"

class Class;

class ObjectProperty : public Object
{
public:
	ObjectProperty(string name);
	~ObjectProperty();

	void AddDomain(Class* domain);
	int GetDomainSize() { return (int)m_domains.size(); }
	Class* GetDomainAt(int index) { return m_domains[index]; }

	void AddRange(Class* range);
	int GetRangeSize() { return (int)m_ranges.size(); }
	Class* GetRangeAt(int index) { return m_ranges[index]; }

	void AddSuperProperty(ObjectProperty* superProperty);
	int GetSuperPropertySize() { return (int)m_superProperties.size(); }
	ObjectProperty* GetSuperPropertyAt(int index) { return m_superProperties[index]; }
	
	void AddInverseProperty(ObjectProperty* inverseProperty);
	int GetInversePropertySize() { return (int)m_inverseProperties.size(); }
	ObjectProperty* GetInversePropertyAt(int index) { return m_inverseProperties[index]; }

	void SetFunctional(bool functional) { m_isFunctional = functional; }
	bool IsFunctional() { return m_isFunctional; }
	
	void SetInverseFunctional(bool inverseFunctional) { m_isInverseFunctional = inverseFunctional; }
	bool IsInverseFunctional() { return m_isInverseFunctional; }

	void SetTransitive(bool transitive) { m_isTransitive = transitive; }
	bool IsTransitive() { return m_isTransitive; }

	void SetSymmetric(bool symmetric) { m_isSymmetric = symmetric; }
	bool IsSymmetric() { return m_isSymmetric; }

	void SetAsymmetric(bool asymmetric) { m_isAsymmetric = asymmetric; }
	bool IsAsymmetric() { return m_isAsymmetric; }

	void SetReflexive(bool reflexive) { m_isReflexive = reflexive; }
	bool IsReflexive() { return m_isReflexive; }

	void SetIrreflieive(bool irreflieive) { m_isIrreflieive = irreflieive; }
	bool IsIrreflieive() { return m_isIrreflieive; }

protected:
	void Clear();

private:
	vector<Class*> m_domains;
	vector<Class*> m_ranges;

	vector<ObjectProperty*> m_superProperties;
	vector<ObjectProperty*> m_inverseProperties;

	bool m_isFunctional, m_isInverseFunctional, m_isTransitive, m_isSymmetric, m_isAsymmetric, m_isReflexive, m_isIrreflieive;
};