#pragma once

class Ontology;

class STP2ONT
{
public:
	STP2ONT(string name);
	~STP2ONT();

	string GetName() { return m_name; }
	Ontology* GetOntology() { return m_ontology; }

	// Translate the schema to ontology.
	void TranslateSchema(const Schema* schema, S2O_Option* opt);

	// Translate the instances to ontology.
	void TranslateInstances(InstMgr* instList, S2O_Option* opt);

	void SetStepPrefix(string prefix) { m_schemaPrefix = prefix; }
	string GetStepPrefix() { return m_schemaPrefix; }

protected:
	void SetSchemaPrefix();

private:
	Ontology* m_ontology;

private:
	string m_name;
	string m_schemaPrefix;
};