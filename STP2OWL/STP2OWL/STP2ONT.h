#pragma once

class Ontology;

class STP2ONT
{
public:
	STP2ONT(const string& name);
	~STP2ONT();

	const string& GetName() const { return m_name; }
	Ontology* GetOntology() const { return m_ontology; }

	// Translate the schema to ontology.
	void TranslateSchema(const Schema*& schema, S2O_Option& opt) const;

	// Translate the instances to ontology.
	void TranslateInstances(InstMgr*& instances, S2O_Option& opt);

	void SetStepPrefix(const string& prefix) { m_schemaPrefix = prefix; }
	const string& GetStepPrefix() const { return m_schemaPrefix; }

protected:
	void SetSchemaPrefix() const;

private:
	Ontology* m_ontology;

private:
	string m_name;
	string m_schemaPrefix;
};