#pragma once

class STP2ONT;
class Individual;

class OntWriter 
{
public:
	void WriteOWLFunctionalSyntax(STP2ONT& s2o, S2O_Option& opt);

protected:
	void ApplyOWL2Profile(stringstream& ss_owl, S2O_Option& opt);
	bool IsKeepClass(Individual* indvl, S2O_Option& opt);
};