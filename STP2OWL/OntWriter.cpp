#include "stdafx.h"
#include "OntWriter.h"
#include "STP2ONT.h"
#include "Ontology.h"
#include "Class.h"
#include "ObjectProperty.h"
#include "DataProperty.h"
#include "Individual.h"
#include "DataType.h"
#include "AnnotationProperty.h"

void OntWriter::WriteOWLFunctionalSyntax(STP2ONT*& s2o, S2O_Option& opt)
{
	Ontology* ont = s2o->GetOntology();
	
	string schemaName = s2o->GetName();
	string schemaPrefix = s2o->GetStepPrefix();

	string ontoSchemaPrefix = "http://www.nist.gov/OntoSTEP/" + schemaName;
	string ontoInstancePrefix = "http://www.nist.gov/OntoSTEP/instances";

	if (opt.Mode() == OutputMode::Separated
		|| opt.Mode() == OutputMode::Schema_Only
		|| opt.Mode() == OutputMode::Integrated)
	{
		stringstream ss_owl;

		// prefixes
		ss_owl << "Prefix(:=<" << ontoSchemaPrefix << "#>)\n";
		ss_owl << "Prefix(owl:=<http://www.w3.org/2002/07/owl#>)\n";
		ss_owl << "Prefix(rdf:=<http://www.w3.org/1999/02/22-rdf-syntax-ns#>)\n";
		ss_owl << "Prefix(xml:=<http://www.w3.org/XML/1998/namespace>)\n";
		ss_owl << "Prefix(xsd:=<http://www.w3.org/2001/XMLSchema#>)\n";
		ss_owl << "Prefix(rdfs:=<http://www.w3.org/2000/01/rdf-schema#>)\n";
		ss_owl << "\n\n";

		ss_owl << "Ontology(<" << ontoSchemaPrefix << ">";
		ss_owl << "\n\n";

		// declarations
		for (int i = 0; i < ont->GetClassSize(); ++i)
		{
			string name = ont->GetClassAt(i)->GetName();
			ss_owl << "Declaration(Class(:" << name << "))\n";
		}

		for (int i = 0; i < ont->GetObjectPropertySize(); ++i)
		{
			string name = ont->GetObjectPropertyAt(i)->GetName();
			ss_owl << "Declaration(ObjectProperty(:" << name << "))\n";
		}

		for (int i = 0; i < ont->GetDataPropertySize(); ++i)
		{
			string name = ont->GetDataPropertyAt(i)->GetName();
			ss_owl << "Declaration(DataProperty(:" << name << "))\n";
		}

		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			string name = ont->GetIndividualAt(i)->GetName();

			if (opt.Mode() != OutputMode::Integrated
				&& ont->GetIndividualAt(i)->GetPrefix() != schemaPrefix)
				continue;

			ss_owl << "Declaration(NamedIndividual(:" << name << "))\n";
		}

		for (int i = 0; i < ont->GetAnnotationPropertySize(); ++i)
		{
			string name = ont->GetAnnotationPropertyAt(i)->GetName();
			ss_owl << "Declaration(AnnotationProperty(:" << name << "))\n";
		}

		ss_owl << "\n\n";
		ss_owl << "############################\n";
		ss_owl << "#   Object Properties\n";
		ss_owl << "############################\n";
		ss_owl << "\n";

		// object property domain, range
		for (int i = 0; i < ont->GetObjectPropertySize(); ++i)
		{
			ObjectProperty* objProp = ont->GetObjectPropertyAt(i);
			string name = objProp->GetName();

			ss_owl << "# Object Property: :" << name << " (:" << name <<")\n\n";

			int annotationSize = objProp->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = objProp->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				ss_owl << "AnnotationAssertion(:" << annoPropName << " :" << name;
				ss_owl << " \"" << annoStr << "\"^^" << annoDataTypeName << ")\n";
			}

			if (objProp->IsFunctional())
				ss_owl << "FunctionalObjectProperty(:" << name << ")\n";

			if (objProp->IsTransitive())
				ss_owl << "TransitiveObjectProperty(:" << name << ")\n";

			int superObjPropSize = objProp->GetSuperPropertySize();

			for (int j = 0; j < superObjPropSize; ++j)
			{
				string superPropName = objProp->GetSuperPropertyAt(j)->GetName();
				ss_owl << "SubObjectPropertyOf(:" << name << " :" << superPropName << ")\n";
			}

			int invObjPropSize = objProp->GetInversePropertySize();

			for (int j = 0; j < invObjPropSize; ++j)
			{
				string invPropName = objProp->GetInversePropertyAt(j)->GetName();
				ss_owl << "InverseObjectProperties(:" << name << " :" << invPropName << ")\n";
			}

			int domainSize = objProp->GetDomainSize();

			for (int j = 0; j < domainSize; ++j)
			{
				string domainName = objProp->GetDomainAt(j)->GetName();
				ss_owl << "ObjectPropertyDomain(:" << name << " :" << domainName << ")\n";
			}

			int rangeSize = objProp->GetRangeSize();

			for (int j = 0; j < rangeSize; ++j)
			{
				string rangeName = objProp->GetRangeAt(j)->GetName();
				ss_owl << "ObjectPropertyRange(:" << name << " :" << rangeName << ")\n";
			}

			ss_owl << "\n";
		}

		ss_owl << "\n";
		ss_owl << "############################\n";
		ss_owl << "#   Data Properties\n";
		ss_owl << "############################\n";
		ss_owl << "\n";

		// data property domain, range
		for (int i = 0; i < ont->GetDataPropertySize(); ++i)
		{
			DataProperty* dataProp = ont->GetDataPropertyAt(i);
			string name = dataProp->GetName();

			ss_owl << "# Data Property: :" << name << " (:" << name << ")\n\n";

			int domainSize = dataProp->GetDomainSize();

			for (int j = 0; j < domainSize; ++j)
			{
				string domainName = dataProp->GetDomainAt(j)->GetName();
				ss_owl << "DataPropertyDomain(:" << name << " :" << domainName << ")\n";
			}

			int rangeSize = dataProp->GetRangeSize();

			for (int j = 0; j < rangeSize; ++j)
			{
				string rangeName = dataProp->GetRangeAt(j)->GetName();
				ss_owl << "DataPropertyRange(:" << name << " " << rangeName << ")\n";
			}

			ss_owl << "\n";
		}

		ss_owl << "\n";
		ss_owl << "############################\n";
		ss_owl << "#   Classes\n";
		ss_owl << "############################\n";
		ss_owl << "\n";

		// subclasses
		for (int i = 0; i < ont->GetClassSize(); ++i)
		{
			Class* cls = ont->GetClassAt(i);
			string name = cls->GetName();

			ss_owl << "# Class: :" << name << " (:" << name << ")\n\n";

			int annotationSize = cls->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = cls->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				ss_owl << "AnnotationAssertion(:" << annoPropName << " :" << name;
				ss_owl << " \"" << annoStr << "\"^^" << annoDataTypeName << ")\n";
			}

			int equivalentClassSize = cls->GetEquivalentClassSize();

			if (equivalentClassSize > 0)
			{
				ss_owl << "EquivalentClasses(:" << name;

				for (int j = 0; j < equivalentClassSize; ++j)
				{
					string equivalentClassName = cls->GetEquivalentClassAt(j)->GetName();

					ss_owl << " :" << equivalentClassName;
				}

				ss_owl << ")\n";
			}

			int objUnionOfSize = cls->GetObjectUnionOfSize();

			if (objUnionOfSize > 0)
			{
				ss_owl << "EquivalentClasses(:" << name << " ObjectUnionOf(";

				for (int j = 0; j < objUnionOfSize; ++j)
				{
					string objUnionOfName = cls->GetObjectUnionOfAt(j)->GetName();

					if (j == 0)
						ss_owl << ":" << objUnionOfName;
					else
						ss_owl << " :" << objUnionOfName;
				}

				ss_owl << "))\n";
			}

			int disjointUnionOfSize = cls->GetDisjointUnionOfSize();

			if (disjointUnionOfSize > 1) // Must be greater than 1.
			{
				ss_owl << "DisjointUnion(:" << name;

				for (int j = 0; j < disjointUnionOfSize; ++j)
				{
					string disjUnionOfName = cls->GetDisjointUnionOfAt(j)->GetName();

					ss_owl << " :" << disjUnionOfName;
				}

				ss_owl << ")\n";
			}
			else if (disjointUnionOfSize == 1)
			{
				//fprintf_s(f, "EquivalentClasses(:%s :%s)\n", name.c_str(), cls->GetDisjointUnionOfAt(0)->GetName().c_str());
			}

			int disjointClassSize = cls->GetDisjointClassSize();

			if (disjointClassSize > 0)
			{
				ss_owl << "DisjointClasses(";

				for (int j = 0; j < disjointClassSize; ++j)
				{
					string disjClassName = cls->GetDisjointClassAt(j)->GetName();

					if (j == 0)
						ss_owl << ":" << disjClassName;
					else
						ss_owl << " :" << disjClassName;
				}

				ss_owl << ")\n";
			}

			int objIntersectionOfSize = cls->GetObjectIntersectionOfSize();

			if (objIntersectionOfSize > 0)
			{
				ss_owl << "EquivalentClasses(:" << name << " ObjectIntersectionOf(";

				for (int j = 0; j < objIntersectionOfSize; ++j)
				{
					string objIntersectionOfName = cls->GetObjectIntersectionOfAt(j)->GetName();

					if (j == 0)
						ss_owl << ":" << objIntersectionOfName;
					else
						ss_owl << " :" << objIntersectionOfName;
				}

				ss_owl << "))\n";
			}

			int objOneOfSize = cls->GetObjectOneOfSize();

			if (objOneOfSize > 0)
			{
				ss_owl << "EquivalentClasses(:" << name << " ObjectOneOf(";

				for (int j = 0; j < objOneOfSize; ++j)
				{
					string objOneOfName = cls->GetObjectOneOfAt(j)->GetName();

					if (j == 0)
						ss_owl << ":" << objOneOfName;
					else
						ss_owl << " :" << objOneOfName;
				}

				ss_owl << "))\n";
			}

			int superClassSize = cls->GetSuperClassSize();

			for (int j = 0; j < superClassSize; ++j)
			{
				string superClassName = cls->GetSuperClassAt(j)->GetName();

				ss_owl << "SubClassOf(Annotation(:SupertypeIndex \"" << to_string(j) << "\"^^xsd:integer)";
				ss_owl << " :" << name << " :" << superClassName << ")\n";
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
					ss_owl << "SubClassOf(:" << name << " ObjectMaxCardinality(";
					ss_owl << to_string(objPropMaxCard) << " :" << objPropName << "))\n";

					continue;
				}

				if (objPropMinCard == 0) // optional
				{
					ss_owl << "SubClassOf(:" << name << " ObjectAllValuesFrom(:";
					ss_owl << objPropName << " ObjectUnionOf(:" << objPropRangeName << " owl:Nothing)))\n";

					if (objPropMaxCard < MAXCARD)
					{
						ss_owl << "SubClassOf(:" << name << " ObjectMaxCardinality(";
						ss_owl << to_string(objPropMaxCard ) <<" :" << objPropName << "))\n";
					}
				}
				else if (objPropMinCard == 1 // exact
					&& objPropMaxCard == 1)
				{
					ss_owl << "SubClassOf(:" << name << " ObjectAllValuesFrom(:";
					ss_owl << objPropName << " :" << objPropRangeName << "))\n";

					ss_owl << "SubClassOf(:" << name << " ObjectExactCardinality(";
					ss_owl << to_string(objPropMaxCard) << " :" << objPropName << "))\n";
				}
				else if (objPropMinCard >= 1
					&& objPropMaxCard >= 1)
				{
					ss_owl << "SubClassOf(:" << name << " ObjectAllValuesFrom(:";
					ss_owl << objPropName << " :" << objPropRangeName << "))\n";

					if (objPropMinCard < MAXCARD)
					{
						ss_owl << "SubClassOf(:" << name << " ObjectMinCardinality(";
						ss_owl << to_string(objPropMinCard) << " :" << objPropName << "))\n";
					}

					if (objPropMaxCard < MAXCARD)
					{
						ss_owl << "SubClassOf(:" << name << " ObjectMaxCardinality(";
						ss_owl << to_string(objPropMaxCard) << " :" << objPropName << "))\n";
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

				ss_owl << "SubClassOf(:" << name << " DataAllValuesFrom(:" << datPropName << " " << datPropRangeName << "))\n";
			}

			ss_owl << "\n";
		}

		ss_owl << "\n";
		ss_owl << "############################\n";
		ss_owl << "#   Named Individuals\n";
		ss_owl << "############################\n";
		ss_owl << "\n";

		// individual assertions
		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			Individual* indvl = ont->GetIndividualAt(i);
			string name = indvl->GetName();

			if (opt.Mode() == OutputMode::Integrated)
			{
				if (!IsKeepClass(indvl, opt))
					continue;
			}
			else
			{
				if (indvl->GetPrefix() != schemaPrefix)
					continue;
			}

			ss_owl << "# Individual: :" << name << " (:" << name << ")\n\n";

			int annotationSize = indvl->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = indvl->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				ss_owl << "AnnotationAssertion(:" << annoPropName << " :" << name;
				ss_owl << " \"" << annoStr << "\"^^" << annoDataTypeName << ")\n";
			}

			int clsTypeSize = indvl->GetClassTypeSize();

			for (int j = 0; j < clsTypeSize; ++j)
			{
				string clsTypeName = indvl->GetClassTypeAt(j)->GetName();
				ss_owl << "ClassAssertion(:" << clsTypeName << " :" << name << ")\n";
			}

			int objPropAssertSize = indvl->GetObjPropertyAssertionSize();

			for (int j = 0; j < objPropAssertSize; ++j)
			{
				ObjPropertyAssertion objPropAss = indvl->GetObjPropertyAssertionAt(j);

				ObjectProperty* objProp = objPropAss.first;
				Individual* rangeIndvl = objPropAss.second;

				string objPropName = objProp->GetName();
				string rangeIndvlName = rangeIndvl->GetName();
				ss_owl << "ObjectPropertyAssertion(:" << objPropName << " :" << name << " :" << rangeIndvlName << ")\n";
			}

			int datPropAssertSize = indvl->GetDataPropertyAssertionSize();

			for (int j = 0; j < datPropAssertSize; ++j)
			{
				DataPropertyAssertion datPropAss = indvl->GetDataPropertyAssertionAt(j);

				DataProperty* datProp = datPropAss.first;
				string datPropValue = datPropAss.second;

				string datPropName = datProp->GetName();
				string datPropTypeName = datProp->GetRangeAt(0)->GetName();

				// Remove expoents from the string
				if (datPropTypeName == "xsd:decimal"
					&& (StrUtil::IsExisting(datPropValue, "E")
						|| StrUtil::IsExisting(datPropValue, "e")))
				{
					double val = stod(datPropValue);
					datPropValue = to_string(val);
				}

				ss_owl << "DataPropertyAssertion(:" << datPropName << " :" << name << " \"" << datPropValue << "\"^^" << datPropTypeName << ")\n";
			}

			ss_owl << "\n";
		}

		ss_owl << "\n";

		// close ontology
		ss_owl << ")\n";

		if (opt.Profile() != OWL2Profile::OWL2_DL)
			ApplyOWL2Profile(ss_owl, opt);

		ofstream of;

		if (opt.Mode() != OutputMode::Integrated)
			of.open(opt.Output("Schema").c_str());
		else
			of.open(opt.Output("Integrated").c_str());

		of << ss_owl.str().c_str();
		of.close();
	}
	
	if (opt.Mode() == OutputMode::Separated
		|| opt.Mode() == OutputMode::Instance_Only)
	{	
		stringstream ss_owl;
		
		// prefixes
		ss_owl << "Prefix(:=<" << ontoInstancePrefix << "#>)\n";
		ss_owl << "Prefix(" << schemaPrefix << ":=<" << ontoSchemaPrefix << "#>)\n";
		ss_owl << "Prefix(owl:=<http://www.w3.org/2002/07/owl#>)\n";
		ss_owl << "Prefix(rdf:=<http://www.w3.org/1999/02/22-rdf-syntax-ns#>)\n";
		ss_owl << "Prefix(xml:=<http://www.w3.org/XML/1998/namespace>)\n";
		ss_owl << "Prefix(xsd:=<http://www.w3.org/2001/XMLSchema#>)\n";
		ss_owl << "Prefix(rdfs:=<http://www.w3.org/2000/01/rdf-schema#>)\n";
		ss_owl << "\n\n";

		// open ontology
		ss_owl << "Ontology(<" << ontoInstancePrefix << ">\n";
		ss_owl << "Import(<" << ontoSchemaPrefix << ">)\n";
		ss_owl << "\n";

		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			string name = ont->GetIndividualAt(i)->GetName();
			
			if (ont->GetIndividualAt(i)->GetPrefix() == schemaPrefix)
				continue;

			Individual* indvl = ont->GetIndividualAt(i);

			if (!IsKeepClass(indvl, opt))
				continue;

			ss_owl << "Declaration(NamedIndividual(:" << name << "))\n";
		}
		ss_owl << "\n\n";
		ss_owl << "############################\n";
		ss_owl << "#   Named Individuals\n";
		ss_owl << "############################\n";
		ss_owl << "\n";

		// individual assertions
		for (int i = 0; i < ont->GetIndividualSize(); ++i)
		{
			Individual* indvl = ont->GetIndividualAt(i);
			string name = indvl->GetName();

			if (!IsKeepClass(indvl, opt))
				continue;

			if (indvl->GetPrefix() == schemaPrefix)
				continue;

			ss_owl << "# Individual: :" << name << " (:" << name << ")\n\n";

			int annotationSize = indvl->GetAnnotationSize();

			for (int j = 0; j < annotationSize; ++j)
			{
				Annotation anno = indvl->GetAnnotationAt(j);
				string annoPropName = anno.first->GetName();
				string annoDataTypeName = anno.first->GetRangeAt(0)->GetName();
				string annoStr = anno.second;

				ss_owl << "AnnotationAssertion(" << schemaPrefix << ":" << annoPropName << " :" << name;
				ss_owl << " \"" << annoStr << "\"^^" << annoDataTypeName << ")\n";
			}

			int clsTypeSize = indvl->GetClassTypeSize();

			for (int j = 0; j < clsTypeSize; ++j)
			{
				string clsTypeName = indvl->GetClassTypeAt(j)->GetName();
				ss_owl << "ClassAssertion(" << schemaPrefix << ":" << clsTypeName << " :" << name << ")\n";
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

				ss_owl << "ObjectPropertyAssertion(" << schemaPrefix << ":" << objPropName << " :" << name << " " << rangeIndvlPrefix << ":" << rangeIndvlName << ")\n";
			}

			int datPropAssertSize = indvl->GetDataPropertyAssertionSize();

			for (int j = 0; j < datPropAssertSize; ++j)
			{
				DataPropertyAssertion datPropAss = indvl->GetDataPropertyAssertionAt(j);

				DataProperty* datProp = datPropAss.first;
				string datPropValue = datPropAss.second;

				string datPropName = datProp->GetName();
				string datPropTypeName = datProp->GetRangeAt(0)->GetName();

				// Remove expoents from the string
				if (datPropTypeName == "xsd:decimal"
					&& (StrUtil::IsExisting(datPropValue, "E")
					|| StrUtil::IsExisting(datPropValue, "e")))
				{
					double val = stod(datPropValue);
					datPropValue = to_string(val);
				}

				ss_owl << "DataPropertyAssertion(" << schemaPrefix << ":" << datPropName << " :" << name << " \"" << datPropValue << "\"^^" << datPropTypeName << ")\n";
			}

			ss_owl << "\n";
		}
		
		ss_owl << "\n";

		// close ontology
		ss_owl << ")\n";

		ofstream of;
		of.open(opt.Output("Instances").c_str());
		of << ss_owl.str().c_str();
		of.close();
	}
	//

}

void OntWriter::ApplyOWL2Profile(stringstream& ss_owl, S2O_Option& opt)
{
	stringstream ss_owl_mod;
	
	string tmp;
	while (getline(ss_owl, tmp))
	{
		if (opt.Profile() == OWL2Profile::OWL2_EL
			&& !opt.IsOWL2EL(tmp))
		{
			ss_owl_mod << "#  " << tmp << endl;
		}
		else if (opt.Profile() == OWL2Profile::OWL2_QL
			&& !opt.IsOWL2QL(tmp))
		{
			ss_owl_mod << "#  " << tmp << endl;
		}
		else if (opt.Profile() == OWL2Profile::OWL2_RL
			&& !opt.IsOWL2RL(tmp))
		{
			ss_owl_mod << "#  " << tmp << endl;
		}
		else
		{
			ss_owl_mod << tmp << endl;
		}
	}
	
	ss_owl.clear();
	ss_owl.str(ss_owl_mod.str());
}

bool OntWriter::IsKeepClass(Individual* indvl, S2O_Option& opt)
{
	for (int i = 0; i < indvl->GetClassTypeSize(); ++i)
	{
		string clsName = indvl->GetClassTypeAt(i)->GetName();

		if (opt.IsKeepClass(clsName))
			return true;
	}

	return false;
}
