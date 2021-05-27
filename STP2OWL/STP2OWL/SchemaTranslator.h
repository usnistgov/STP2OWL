#pragma once

class Object;
class Ontology;
class Class;
class ObjectProperty;
class DataProperty;

class SchemaTranslator
{
public:
	SchemaTranslator(const Schema*& schema, Ontology* ontology);
	~SchemaTranslator();

	void Translate(S2O_Option& opt);

protected:
	void AddClassesForEntities(const EntityDescriptorList* eDesList) const;
	void AddSuperClassesForEntities(const EntityDescriptorList* eDesList) const;
	void AddSubClassesFromSupertypeStatement(const EntityDescriptorList* eDesList);

	void AddSubClassesFromONEOForANDorANDOR(string str, Class*& superclass);
	void AddSubClassesFromANDorANDOR(string str, Class*& superclass);

	void AddClassesForTypes(const TypeDescriptorList* tDesList) const;
	void AddClassesForBaseTypes() const;

	void AddObjPropertiesForExplicitAttributes(Class* eClass, const AttrDescriptorList* aDesList) const;
	void AddObjPropertiesForInverseAttributes(Class* eClass, const Inverse_attributeList* invAttrList) const;
	void AddObjPropertyForAttribute(Class* eClass, const AttrDescriptor* aDes) const;

	void AddClassesObjPropertiesForSetBag(const TypeDescriptor* aggrTypeDes, string& aggrName) const;
	void AddClassesObjPropertiesForArrayList(const TypeDescriptor* aggrTypeDes, string& aggrName) const;

	void AddDataTypes() const;
	void AddDataProperties(OWL2Profile owl2Profile) const;
	void AddAnnotationProperties() const;
	void AddAnnotation(Object* obj, string annoPropName, string annoStr) const;

	string GetAbstractClassName() { return "abst_" + to_string(GetAbstractClassCount()); }
	int GetAbstractClassCount() { return m_abstractClassCount++; }

protected:
	void AddDataPropertiesForXYZ() const;

private:
	Ontology* m_ontology;
	const Schema* m_schema;

private:
	int m_abstractClassCount;
};