#include "stdafx.h"
#include "step2ont.h"

#include "Ontology.h"
#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"

#include "SchemaTranslator.h"
#include "InstanceTranslator.h"

step2ont::step2ont(string name)
{
	m_name = StrUtil::ToLower(name);
	m_ontology = new Ontology(m_name);

	m_schemaPrefix = "step"; // default prefix
}

step2ont::~step2ont()
{
	delete m_ontology;
}


void step2ont::TranslateSchema(const Schema* schema)
{
	SchemaTranslator* st = new SchemaTranslator(schema, m_ontology);
	st->Translate();

	delete st;

	SetSchemaPrefix();
}

void step2ont::TranslateInstances(InstMgr* instList)
{
	InstanceTranslator* it = new InstanceTranslator(instList, m_ontology);
	it->Translate();

	delete it;
}


void step2ont::SetSchemaPrefix()
{
	for (int i = 0; i < m_ontology->GetClassSize(); ++i)
		m_ontology->GetClassAt(i)->SetPrefix(m_schemaPrefix);

	for (int i = 0; i < m_ontology->GetObjectPropertySize(); ++i)
		m_ontology->GetObjectPropertyAt(i)->SetPrefix(m_schemaPrefix);

	for (int i = 0; i < m_ontology->GetDataPropertySize(); ++i)
		m_ontology->GetDataPropertyAt(i)->SetPrefix(m_schemaPrefix);

	for (int i = 0; i < m_ontology->GetIndividualSize(); ++i)
		m_ontology->GetIndividualAt(i)->SetPrefix(m_schemaPrefix);

	for (int i = 0; i < m_ontology->GetAnnotationPropertySize(); ++i)
		m_ontology->GetAnnotationPropertyAt(i)->SetPrefix(m_schemaPrefix);
}
