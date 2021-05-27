#pragma once

class STP2ONT;
class Individual;
class Ontology;

class OntWriter 
{
public:
	void WriteOWLFunctionalSyntax(STP2ONT& s2o, S2O_Option& opt) const;

protected:
	void ApplyOWL2Profile(stringstream& ss_owl, S2O_Option& opt) const;
	bool IsKeepClass(Individual* indvl, S2O_Option& opt) const;
};