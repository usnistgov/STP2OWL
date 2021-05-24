#pragma once

class Object;
class Ontology;
class Class;
class ObjectProperty;
class DataProperty;

class SchemaTranslator
{
public:
	SchemaTranslator(const Schema* schema, Ontology* ontology);
	~SchemaTranslator();

	void Translate(S2O_Option* opt);

protected:
	void AddClassesForEntities(const EntityDescriptorList* eDesList);
	void AddSuperClassesForEntities(const EntityDescriptorList* eDesList);
	void AddSubClassesFromSupertypeStatement(const EntityDescriptorList* eDesList);

	void AddSubClassesFromONEOForANDorANDOR(string str, Class*& superclass);
	void AddSubClassesFromANDorANDOR(string str, Class*& superclass);

	void AddClassesForTypes(const TypeDescriptorList* tDesList);
	void AddClassesForBaseTypes();

	void AddObjPropertiesForExplicitAttributes(Class* eClass, const AttrDescriptorList* aDesList);
	void AddObjPropertiesForInverseAttributes(Class* eClass, const Inverse_attributeList* invAttrList);
	void AddObjPropertyForAttribute(Class* eClass, const AttrDescriptor* aDes);

	void AddClassesObjPropertiesForSetBag(const TypeDescriptor* aggrTypeDes, string& aggrName);
	void AddClassesObjPropertiesForArrayList(const TypeDescriptor* aggrTypeDes, string& aggrName);

	void AddDataTypes();
	void AddDataProperties(OWL2Profile owl2Profile);
	void AddAnnotationProperties();
	void AddAnnotation(Object* obj, string annoPropName, string annoStr);

	string GetAbstractClassName() { return "abs_" + to_string(GetAbstractClassCount()); }
	int GetAbstractClassCount() { return m_abstractClassCount++; }

protected:
	void AddDataPropertiesForXYZ();

private:
	Ontology* m_ontology;
	const Schema* m_schema;

private:
	int m_abstractClassCount;
};