#pragma once

class Ontology;

class step2ont
{
public:
	step2ont(string name);
	~step2ont();

	string GetName() { return m_name; }
	Ontology* GetOntology() { return m_ontology; }

	// Translate the schema to ontology.
	void TranslateSchema(const Schema* schema);

	// Translate the instances to ontology.
	void TranslateInstances(InstMgr* instList);

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