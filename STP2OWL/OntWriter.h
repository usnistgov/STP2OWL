#pragma once

class Ontology;
class step2ont;

class OntWriter 
{
public:
	static void WriteOWLFunctionalSyntax(string filePath, step2ont* s2o, int mode);
};