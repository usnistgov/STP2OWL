#include "stdafx.h"
#include "OntWriter.h"
#include "step2ont.h"
#include "Ontology.h"
#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"

void OntWriter::WriteOWLFunctionalSyntax(string filePath, step2ont* s2o, int mode)
{
	Ontology* ont = s2o->GetOntology();
	
	// open file
	FILE* f = nullptr;
	fopen_s(&f, filePath.c_str(), "w");
	
	string schemaName = s2o->GetName();
	string schemaPrefix = s2o->GetStepPrefix();

	string ontoSchemaPrefix = "http://www.nist.gov/OntoSTEP/" + schemaName;
	string ontoInstancePrefix = "http://www.nist.gov/OntoSTEP/instances";

	if (mode == 0)
	{
		// prefixes
		fprintf_s(f, "Prefix(:=<%s#>)\n", ontoSchemaPrefix.c_str());
		fprintf_s(f, "Prefix(owl:=<http://www.w3.org/2002/07/owl#>)\n");
		fprintf_s(f, "Prefix(rdf:=<http://www.w3.org/1999/02/22-rdf-syntax-ns#>)\n");
		fprintf_s(f, "Prefix(xml:=<http://www.w3.org/XML/1998/namespace>)\n");
		fprintf_s(f, "Prefix(xsd:=<http://www.w3.org/2001/XMLSchema#>)\n");
		fprintf_s(f, "Prefix(rdfs:=<http://www.w3.org/2000/01/rdf-schema#>)\n");
		fprintf_s(f, "\n\n");

		// open ontology
		fprintf_s(f, "Ontology(<%s>", ontoSchemaPrefix.c_str());
		fprintf_s(f, "\n\n");

		// declarations
		for (int i = 0; i < ont->GetClassSize(); ++i)
		{
			string name = ont->GetClassAt(i)->GetName();
			fprintf_s(f, "Declaration(Class(:%s))\n", name.c_str());
		}

		for (int i = 0; i < ont->GetObjectPropertySize(); ++i)
		{
			string name = ont->GetObjectPropertyAt(i)->GetName();
			fprintf_s(f, "Declaration(ObjectProperty(:%s))\n", name.c_str());
		}

		for (int i = 0; i < ont->GetDataPropertySize(); ++i)
		{
			string name = ont->GetDataPropertyAt(i)->GetName();
			fprintf_s(f, "Declaration(DataProperty(:%s))\n", name.c_str());
		}

		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			string name = ont->GetIndividualAt(i)->GetName();

			if (ont->GetIndividualAt(i)->GetPrefix() != schemaPrefix)
				continue;

			fprintf_s(f, "Declaration(NamedIndividual(:%s))\n", name.c_str());
		}

		for (int i = 0; i < ont->GetAnnotationPropertySize(); ++i)
		{
			string name = ont->GetAnnotationPropertyAt(i)->GetName();
			fprintf_s(f, "Declaration(AnnotationProperty(:%s))\n", name.c_str());
		}

		fprintf_s(f, "\n\n");

		fprintf_s(f, "############################\n");
		fprintf_s(f, "#   Object Properties\n");
		fprintf_s(f, "############################\n");

		fprintf_s(f, "\n");

		// objectproperty domain, range
		for (int i = 0; i < ont->GetObjectPropertySize(); ++i)
		{
			ObjectProperty* objProp = ont->GetObjectPropertyAt(i);
			string name = objProp->GetName();

			fprintf_s(f, "# Object Property: :%s (:%s)\n\n", name.c_str(), name.c_str());

			int annotationSize = objProp->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = objProp->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				fprintf_s(f, "AnnotationAssertion(:%s :%s", annoPropName.c_str(), name.c_str());
				fprintf_s(f, " \"%s\"^^%s)\n", annoStr.c_str(), annoDataTypeName.c_str());
			}

			if (objProp->IsFunctional())
				fprintf_s(f, "FunctionalObjectProperty(:%s)\n", name.c_str());

			if (objProp->IsTransitive())
				fprintf_s(f, "TransitiveObjectProperty(:%s)\n", name.c_str());

			int superObjPropSize = objProp->GetSuperPropertySize();

			for (int j = 0; j < superObjPropSize; ++j)
			{
				string superPropName = objProp->GetSuperPropertyAt(j)->GetName();
				fprintf_s(f, "SubObjectPropertyOf(:%s :%s)\n", name.c_str(), superPropName.c_str());
			}

			int invObjPropSize = objProp->GetInversePropertySize();

			for (int j = 0; j < invObjPropSize; ++j)
			{
				string invPropName = objProp->GetInversePropertyAt(j)->GetName();
				fprintf_s(f, "InverseObjectProperties(:%s :%s)\n", name.c_str(), invPropName.c_str());
			}

			int domainSize = objProp->GetDomainSize();

			for (int j = 0; j < domainSize; ++j)
			{
				string domainName = objProp->GetDomainAt(j)->GetName();
				fprintf_s(f, "ObjectPropertyDomain(:%s :%s)\n", name.c_str(), domainName.c_str());
			}

			int rangeSize = objProp->GetRangeSize();

			for (int j = 0; j < rangeSize; ++j)
			{
				string rangeName = objProp->GetRangeAt(j)->GetName();
				fprintf_s(f, "ObjectPropertyRange(:%s :%s)\n", name.c_str(), rangeName.c_str());
			}

			fprintf_s(f, "\n");
		}

		fprintf_s(f, "\n");

		fprintf_s(f, "############################\n");
		fprintf_s(f, "#   Data Properties\n");
		fprintf_s(f, "############################\n");

		fprintf_s(f, "\n");

		// dataproperty domain, range
		for (int i = 0; i < ont->GetDataPropertySize(); ++i)
		{
			DataProperty* dataProp = ont->GetDataPropertyAt(i);
			string name = dataProp->GetName();

			fprintf_s(f, "# Data Property: :%s (:%s)\n\n", name.c_str(), name.c_str());

			int domainSize = dataProp->GetDomainSize();

			for (int j = 0; j < domainSize; ++j)
			{
				string domainName = dataProp->GetDomainAt(j)->GetName();
				fprintf_s(f, "DataPropertyDomain(:%s :%s)\n", name.c_str(), domainName.c_str());
			}

			int rangeSize = dataProp->GetRangeSize();

			for (int j = 0; j < rangeSize; ++j)
			{
				string rangeName = dataProp->GetRangeAt(j)->GetName();
				fprintf_s(f, "DataPropertyRange(:%s %s)\n", name.c_str(), rangeName.c_str());
			}

			fprintf_s(f, "\n");
		}

		fprintf_s(f, "\n");

		fprintf_s(f, "############################\n");
		fprintf_s(f, "#   Classes\n");
		fprintf_s(f, "############################\n");

		fprintf_s(f, "\n");

		// subclasses
		for (int i = 0; i < ont->GetClassSize(); ++i)
		{
			Class* cls = ont->GetClassAt(i);
			string name = cls->GetName();

			fprintf_s(f, "# Class: :%s (:%s)\n\n", name.c_str(), name.c_str());

			int annotationSize = cls->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = cls->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				fprintf_s(f, "AnnotationAssertion(:%s :%s", annoPropName.c_str(), name.c_str());
				fprintf_s(f, " \"%s\"^^%s)\n", annoStr.c_str(), annoDataTypeName.c_str());
			}

			int equivalentClassSize = cls->GetEquivalentClassSize();

			if (equivalentClassSize > 0)
			{
				fprintf_s(f, "EquivalentClasses(:%s", name.c_str());

				for (int j = 0; j < equivalentClassSize; ++j)
				{
					string equivalentClassName = cls->GetEquivalentClassAt(j)->GetName();

					fprintf_s(f, " :%s", equivalentClassName.c_str());
				}

				fprintf_s(f, ")\n");
			}

			int objUnionOfSize = cls->GetObjectUnionOfSize();

			if (objUnionOfSize > 0)
			{
				fprintf_s(f, "EquivalentClasses(:%s ObjectUnionOf(", name.c_str());

				for (int j = 0; j < objUnionOfSize; ++j)
				{
					string objUnionOfName = cls->GetObjectUnionOfAt(j)->GetName();

					if (j == 0)
						fprintf_s(f, ":%s", objUnionOfName.c_str());
					else
						fprintf_s(f, " :%s", objUnionOfName.c_str());
				}

				fprintf_s(f, "))\n");
			}

			int disjointUnionOfSize = cls->GetDisjointUnionOfSize();

			if (disjointUnionOfSize > 1) // Must be greater than 1.
			{
				fprintf_s(f, "DisjointUnion(:%s", name.c_str());

				for (int j = 0; j < disjointUnionOfSize; ++j)
				{
					string disjUnionOfName = cls->GetDisjointUnionOfAt(j)->GetName();

					fprintf_s(f, " :%s", disjUnionOfName.c_str());
				}

				fprintf_s(f, ")\n");
			}
			else if (disjointUnionOfSize == 1)
			{
				//fprintf_s(f, "EquivalentClasses(:%s :%s)\n", name.c_str(), cls->GetDisjointUnionOfAt(0)->GetName().c_str());
			}

			int disjointClassSize = cls->GetDisjointClassSize();

			if (disjointClassSize > 0)
			{
				fprintf_s(f, "DisjointClasses(");

				for (int j = 0; j < disjointClassSize; ++j)
				{
					string disjClassName = cls->GetDisjointClassAt(j)->GetName();

					if (j == 0)
						fprintf_s(f, ":%s", disjClassName.c_str());
					else
						fprintf_s(f, " :%s", disjClassName.c_str());
				}

				fprintf_s(f, ")\n");
			}

			int objIntersectionOfSize = cls->GetObjectIntersectionOfSize();

			if (objIntersectionOfSize > 0)
			{
				fprintf_s(f, "EquivalentClasses(:%s ObjectIntersectionOf(", name.c_str());

				for (int j = 0; j < objIntersectionOfSize; ++j)
				{
					string objIntersectionOfName = cls->GetObjectIntersectionOfAt(j)->GetName();

					if (j == 0)
						fprintf_s(f, ":%s", objIntersectionOfName.c_str());
					else
						fprintf_s(f, " :%s", objIntersectionOfName.c_str());
				}

				fprintf_s(f, "))\n");
			}

			int objOneOfSize = cls->GetObjectOneOfSize();

			if (objOneOfSize > 0)
			{
				fprintf_s(f, "EquivalentClasses(:%s ObjectOneOf(", name.c_str());

				for (int j = 0; j < objOneOfSize; ++j)
				{
					string objOneOfName = cls->GetObjectOneOfAt(j)->GetName();

					if (j == 0)
						fprintf_s(f, ":%s", objOneOfName.c_str());
					else
						fprintf_s(f, " :%s", objOneOfName.c_str());
				}

				fprintf_s(f, "))\n");
			}

			int superClassSize = cls->GetSuperClassSize();

			for (int j = 0; j < superClassSize; ++j)
			{
				string superClassName = cls->GetSuperClassAt(j)->GetName();

				fprintf(f, "SubClassOf(Annotation(:SupertypeIndex \"%d\"^^xsd:integer)", j);
				fprintf(f, " :%s :%s)\n", name.c_str(), superClassName.c_str());
			}

			int objPropSize = cls->GetObjectPropertySize();

			for (int j = 0; j < objPropSize; ++j)
			{
				ObjectProperty* objProp = cls->GetObjectPropertyAt(j);

				string objPropName = objProp->GetName();
				string objPropRangeName = objProp->GetRangeAt(0)->GetName();
				int objPropMinCard = cls->GetObjPropMinCardinarlity(objProp);
				int objPropMaxCard = cls->GetObjPropMaxCardinarlity(objProp);

				if (objPropMaxCard == 0) // e.g. emptylist
				{
					fprintf(f, "SubClassOf(:%s ObjectMaxCardinality(", name.c_str());
					fprintf(f, "%d :%s))\n", objPropMaxCard, objPropName.c_str());

					continue;
				}

				if (objPropMinCard == 0) // optional
				{
					fprintf(f, "SubClassOf(:%s ObjectAllValuesFrom(:", name.c_str());
					fprintf(f, "%s ObjectUnionOf(:%s owl:Nothing)))\n", objPropName.c_str(), objPropRangeName.c_str());

					if (objPropMaxCard < MAXCARD)
					{
						fprintf(f, "SubClassOf(:%s ObjectMaxCardinality(", name.c_str());
						fprintf(f, "%d :%s))\n", objPropMaxCard, objPropName.c_str());
					}
				}
				else if (objPropMinCard == 1 // exact
					&& objPropMaxCard == 1)
				{
					fprintf(f, "SubClassOf(:%s ObjectAllValuesFrom(:", name.c_str());
					fprintf(f, "%s :%s))\n", objPropName.c_str(), objPropRangeName.c_str());

					fprintf(f, "SubClassOf(:%s ObjectExactCardinality(", name.c_str());
					fprintf(f, "%d :%s))\n", objPropMaxCard, objPropName.c_str());
				}
				else if (objPropMinCard >= 1
					&& objPropMaxCard >= 1)
				{
					fprintf(f, "SubClassOf(:%s ObjectAllValuesFrom(:", name.c_str());
					fprintf(f, "%s :%s))\n", objPropName.c_str(), objPropRangeName.c_str());

					if (objPropMinCard < MAXCARD)
					{
						fprintf(f, "SubClassOf(:%s ObjectMinCardinality(", name.c_str());
						fprintf(f, "%d :%s))\n", objPropMinCard, objPropName.c_str());
					}

					if (objPropMaxCard < MAXCARD)
					{
						fprintf(f, "SubClassOf(:%s ObjectMaxCardinality(", name.c_str());
						fprintf(f, "%d :%s))\n", objPropMaxCard, objPropName.c_str());
					}
				}
				else
				{
					// if required
				}
			}

			int datPropSize = cls->GetDataPropertySize();

			for (int j = 0; j < datPropSize; ++j)
			{
				DataProperty* datProp = cls->GetDataPropertyAt(j);

				string datPropName = datProp->GetName();
				string datPropRangeName = datProp->GetRangeAt(0)->GetName();
				int datPropMinCard = cls->GetDataPropMinCardinarlity(datProp);
				int datPropMaxCard = cls->GetDataPropMaxCardinarlity(datProp);

				fprintf(f, "SubClassOf(:%s DataAllValuesFrom(:%s %s))\n", name.c_str(), datPropName.c_str(), datPropRangeName.c_str());
			}

			fprintf_s(f, "\n");
		}

		fprintf_s(f, "\n");

		fprintf_s(f, "############################\n");
		fprintf_s(f, "#   Named Individuals\n");
		fprintf_s(f, "############################\n");

		fprintf_s(f, "\n");

		// individual assertions
		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			Individual* indvl = ont->GetIndividualAt(i);
			string name = indvl->GetName();

			if (indvl->GetPrefix() != schemaPrefix)
				continue;

			fprintf_s(f, "# Individual: :%s (:%s)\n\n", name.c_str(), name.c_str());

			int annotationSize = indvl->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = indvl->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				fprintf_s(f, "AnnotationAssertion(:%s :%s", annoPropName.c_str(), name.c_str());
				fprintf_s(f, " \"%s\"^^%s)\n", annoStr.c_str(), annoDataTypeName.c_str());
			}

			int clsTypeSize = indvl->GetClassTypeSize();

			for (int j = 0; j < clsTypeSize; ++j)
			{
				string clsTypeName = indvl->GetClassTypeAt(j)->GetName();
				fprintf_s(f, "ClassAssertion(:%s :%s)\n", clsTypeName.c_str(), name.c_str());
			}

			int objPropAssertSize = indvl->GetObjPropertyAssertionSize();

			for (int j = 0; j < objPropAssertSize; ++j)
			{
				ObjPropertyAssertion objPropAss = indvl->GetObjPropertyAssertionAt(j);

				ObjectProperty* objProp = objPropAss.first;
				Individual* rangeIndvl = objPropAss.second;

				string objPropName = objProp->GetName();
				string rangeIndvlName = rangeIndvl->GetName();
				fprintf_s(f, "ObjectPropertyAssertion(:%s :%s :%s)\n", objPropName.c_str(), name.c_str(), rangeIndvlName.c_str());
			}

			int datPropAssertSize = indvl->GetDataPropertyAssertionSize();

			for (int j = 0; j < datPropAssertSize; ++j)
			{
				DataPropertyAssertion datPropAss = indvl->GetDataPropertyAssertionAt(j);

				DataProperty* datProp = datPropAss.first;
				string datPropValue = datPropAss.second;

				string datPropName = datProp->GetName();
				string datPropTypeName = datProp->GetRangeAt(0)->GetName();

				fprintf_s(f, "DataPropertyAssertion(:%s :%s \"%s\"^^%s)\n", datPropName.c_str(), name.c_str(), datPropValue.c_str(), datPropTypeName.c_str());
			}

			fprintf_s(f, "\n");
		}

		fprintf_s(f, "\n");

		// close ontology
		fprintf_s(f, ")\n");
	}
	else if (mode == 1)
	{	
		// prefixes
		fprintf_s(f, "Prefix(:=<%s#>)\n", ontoInstancePrefix.c_str());
		fprintf_s(f, "Prefix(%s:=<%s#>)\n", schemaPrefix.c_str(), ontoSchemaPrefix.c_str());
		fprintf_s(f, "Prefix(owl:=<http://www.w3.org/2002/07/owl#>)\n");
		fprintf_s(f, "Prefix(rdf:=<http://www.w3.org/1999/02/22-rdf-syntax-ns#>)\n");
		fprintf_s(f, "Prefix(xml:=<http://www.w3.org/XML/1998/namespace>)\n");
		fprintf_s(f, "Prefix(xsd:=<http://www.w3.org/2001/XMLSchema#>)\n");
		fprintf_s(f, "Prefix(rdfs:=<http://www.w3.org/2000/01/rdf-schema#>)\n");
		fprintf_s(f, "\n\n");

		// open ontology
		fprintf_s(f, "Ontology(<%s>\n", ontoInstancePrefix.c_str());
		fprintf_s(f, "Import(<%s>)\n", ontoSchemaPrefix.c_str());

		fprintf_s(f, "\n");

		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			Individual* indv = ont->GetIndividualAt(i);
			string name = ont->GetIndividualAt(i)->GetName();
			
			if (ont->GetIndividualAt(i)->GetPrefix() == schemaPrefix)
				continue;

			fprintf_s(f, "Declaration(NamedIndividual(:%s))\n", name.c_str());
		}
		
		fprintf_s(f, "\n\n");

		fprintf_s(f, "############################\n");
		fprintf_s(f, "#   Named Individuals\n");
		fprintf_s(f, "############################\n");

		fprintf_s(f, "\n");

		// individual assertions
		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			Individual* indvl = ont->GetIndividualAt(i);
			string name = indvl->GetName();

			string indvPfx = indvl->GetPrefix();

			if (indvPfx == schemaPrefix)
				continue;

			fprintf_s(f, "# Individual: :%s (:%s)\n\n", name.c_str(), name.c_str());

			int annotationSize = indvl->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = indvl->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				fprintf_s(f, "AnnotationAssertion(%s:%s :%s", schemaPrefix.c_str(), annoPropName.c_str(), name.c_str());
				fprintf_s(f, " \"%s\"^^%s)\n", annoStr.c_str(), annoDataTypeName.c_str());
			}

			int clsTypeSize = indvl->GetClassTypeSize();

			for (int j = 0; j < clsTypeSize; ++j)
			{
				string clsTypeName = indvl->GetClassTypeAt(j)->GetName();
				fprintf_s(f, "ClassAssertion(%s:%s :%s)\n", schemaPrefix.c_str(), clsTypeName.c_str(), name.c_str());
			}

			int objPropAssertSize = indvl->GetObjPropertyAssertionSize();

			for (int j = 0; j < objPropAssertSize; ++j)
			{
				ObjPropertyAssertion objPropAss = indvl->GetObjPropertyAssertionAt(j);

				ObjectProperty* objProp = objPropAss.first;
				Individual* rangeIndvl = objPropAss.second;

				string objPropName = objProp->GetName();
				string rangeIndvlName = rangeIndvl->GetName();
				string rangeIndvlPrefix = rangeIndvl->GetPrefix();

				fprintf_s(f, "ObjectPropertyAssertion(%s:%s :%s %s:%s)\n", schemaPrefix.c_str(), objPropName.c_str(), name.c_str(), rangeIndvlPrefix.c_str(), rangeIndvlName.c_str());
			}

			int datPropAssertSize = indvl->GetDataPropertyAssertionSize();

			for (int j = 0; j < datPropAssertSize; ++j)
			{
				DataPropertyAssertion datPropAss = indvl->GetDataPropertyAssertionAt(j);

				DataProperty* datProp = datPropAss.first;
				string datPropValue = datPropAss.second;

				string datPropName = datProp->GetName();
				string datPropTypeName = datProp->GetRangeAt(0)->GetName();

				fprintf_s(f, "DataPropertyAssertion(%s:%s :%s \"%s\"^^%s)\n", schemaPrefix.c_str(), datPropName.c_str(), name.c_str(), datPropValue.c_str(), datPropTypeName.c_str());
			}

			fprintf_s(f, "\n");
		}

		fprintf_s(f, "\n");

		// close ontology
		fprintf_s(f, ")\n");
	}

	// close file
	fclose(f);
}
