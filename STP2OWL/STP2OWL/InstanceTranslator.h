#pragma once

class Ontology;
class ObjectProperty;
class DataProperty;
class Individual;

class InstanceTranslator
{
public:
	InstanceTranslator(InstMgr*& instList, Ontology*& ontology);
	~InstanceTranslator();

	void Translate(S2O_Option& opt);

protected:
	void AddIndividualsForInstances() const;

	void AddPropertyAssertions(Individual* indvl, SDAI_Application_instance_ptr inst) const;
	void AddPropertyAssertion(Individual* indvl, ObjectProperty* objProp, STEPattribute* attr) const;

	void AddObjectPropertyAssertion(Individual* domainIndvl, ObjectProperty* objProp, string rangeIndvlName) const;
	void AddDataPropertyAssertion(Individual* domainIndvl, string domainTypeName, string datPropName, string rangeValue) const;

	void AddPropertyAssertionsForSetBag(string aggrIndvlName, string str, const TypeDescriptor* aggrTypeDes, string& aggrName) const;
	void AddPropertyAssertionsForArrayList(string aggrIndvlName, string str, const TypeDescriptor* aggrTypeDes, string& aggrName) const;

	void AddPropertyAssertionForSelectFromString(Individual* indvl, ObjectProperty* objProp, string attrIndvlName, string str) const;

protected:
	void AddDataPropertyAssertionsForCartesianPointAndDirection(Individual* indvl, string str) const;
	void AddPropertyAssertionsForCoordinatesList(Individual* indvl, string aggrIndvlName, string str) const;
	
	void InitializeSimpleGeomtryMap(void);
	bool IsSimpleGeometryTarget(string clsName) const { return m_simpleGeometryMap.find(clsName) != m_simpleGeometryMap.end(); }

private:
	Ontology* m_ontology;
	InstMgr* m_instList;
	bool m_simpleGeometry;
	map<string, int> m_simpleGeometryMap;
};