#include "stdafx.h"
#include "STP2ONT.h"

#include "Ontology.h"
#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"

#include "SchemaTranslator.h"
#include "InstanceTranslator.h"

STP2ONT::STP2ONT(string name)
{
	m_name = StrUtil::ToLower(name);
	m_ontology = new Ontology(m_name);

	m_schemaPrefix = "step"; // default prefix
}

STP2ONT::~STP2ONT()
{
	delete m_ontology;
}


void STP2ONT::TranslateSchema(const Schema*& schema, S2O_Option& opt)
{
	SchemaTranslator st(schema, m_ontology);
	st.Translate(opt);

	SetSchemaPrefix();
}

void STP2ONT::TranslateInstances(InstMgr*& instList, S2O_Option& opt)
{
	InstanceTranslator it(instList, m_ontology);
	it.Translate(opt);
}


void STP2ONT::SetSchemaPrefix()
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
