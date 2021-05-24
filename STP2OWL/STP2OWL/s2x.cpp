// s2x.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include "step2ont.h"
#include "OntWriter.h"
#include "SchemaChecker.h"
#include "StopWatch.h"

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "[Usage] " << argv[0] << " file.stp" << endl;
		return 0;
	}

	//string stepfilePath = "C:\\Users\\snk29\\Desktop\\stp2ont\\x64\\Release\\ctc\\nist_ctc_01_asme1_rd.stp";
	//string stepfilePath = "C:\\Users\\snk29\\Desktop\\testcases\\ANC101.stp";
	string mode = string(argv[1]);
	//string mode = "-i";

	if (mode == "-s")
	{
		SchemaChecker* schemaChecker = new SchemaChecker();
		string schemaNum(argv[2]);
		//string schemaNum = "242e2";

		schemaChecker->SetShortName(schemaNum);

		Registry* reg = nullptr;
		const Schema* sc = schemaChecker->InitializeSchema(reg);

		cout << endl;
		cout << "[Schema: " << schemaChecker->GetLongName();
		cout << " (" << schemaChecker->GetShortName() << ")" << "]" << endl << endl;

		step2ont* s2o = new step2ont(schemaChecker->GetLongName());

		StopWatch sw;
		sw.Start();

		s2o->TranslateSchema(sc);
		
		// writer
		string owlSchemaFilePath = schemaNum + "_Schema.owl";

		OntWriter::WriteOWLFunctionalSyntax(owlSchemaFilePath, s2o, (int)0);

		cout << "OWL file generation completed!" << endl;

		sw.End();

		delete s2o;
		delete reg;
		delete schemaChecker;
	}
	else if (mode == "-i")
	{
		string stepfilePath(argv[2]);
		//string stepfilePath = "C:\\Users\\snk29\\Desktop\\stp2ont\\x64\\Release\\ctc\\nist_ctc_01_asme1_rd.stp";

		SchemaChecker* schemaChecker = new SchemaChecker(stepfilePath);

		Registry* reg = nullptr;
		const Schema* sc = schemaChecker->InitializeSchema(reg);

		cout << endl;
		cout << "[Schema: " << schemaChecker->GetLongName();
		cout << " (" << schemaChecker->GetShortName() << ")" << "]" << endl << endl;

		InstMgr* instList = new InstMgr();
		step2ont* s2o = new step2ont(schemaChecker->GetLongName());

		try
		{
			StopWatch sw;
			sw.Start();

			STEPfile sfile(*reg, *instList, "", false);
			sfile.ReadExchangeFile(stepfilePath);

			// if there's an error, exit (e.g. wrong file path)
			if (sfile.Error().severity() <= Severity::SEVERITY_USERMSG)
				cout << sfile.Error().UserMsg();

			S2OThrowExceptionIf<S2OException>(sfile.Error().severity() < Severity::SEVERITY_WARNING, "Fatal error! Translaton aborted.");

			sw.Lap();

			s2o->TranslateSchema(sc);

			cout << "STEP Schema to Ontology translation completed!" << endl;

			sw.Lap();

			s2o->TranslateInstances(instList);

			cout << "STEP Instances to Ontology translation completed!" << endl;

			sw.Lap();

			// writer
			//string owlSchemaFilePath = stepfilePath + "_Schema.owl";
			string owlInstanceFilePath = stepfilePath + "_Instances.owl";

			//OntWriter::WriteOWLFunctionalSyntax(owlSchemaFilePath, s2o, (int)0);
			OntWriter::WriteOWLFunctionalSyntax(owlInstanceFilePath, s2o, (int)1);

			cout << "OWL file generation completed!" << endl;

			sw.End();

			sw.ReportTotal();
		}
		catch (S2OException& e)
		{
			cout << e.GetMessage() << endl;
		}

		delete s2o;
		delete instList;
		delete reg;
		delete schemaChecker;
	}

	return 0;
}

/*
///  JSON translate  ////

ofstream fout;

void printAttribute(STEPattribute* attr)
{
	BASE_TYPE attrBaseType = attr->BaseType();	// attribute base type

	if (attrBaseType == BASE_TYPE::sdaiINSTANCE)
	{
		SDAI_Application_instance_ptr val = attr->Entity();

		if (val->StepFileId() == 0)
			fout << "null";
		else
			fout << "{\"$ref\":\"" << val->StepFileId() << "\"}";
	}
	else if (attrBaseType == BASE_TYPE::sdaiSTRING)
	{
		SDAI_String* val = attr->String();

		string str(val->c_str());

		if (str.length() >= 2)
			str = str.substr(1, str.length() - 2);

		// remove "\n"
		while (str.find("\n") != -1)
		{
			str.replace(str.find("\n"), 1, "");
		}

		str = "\"" + str + "\"";

		fout << str;
	}
	else if (attrBaseType == BASE_TYPE::sdaiINTEGER)
	{
		SDAI_Integer* val = attr->Integer();
		fout << *val;
	}
	else if (attrBaseType == BASE_TYPE::sdaiREAL)
	{
		SDAI_Real* val = attr->Real();
		fout << *val;
	}
	else if (attrBaseType == BASE_TYPE::sdaiBOOLEAN)
	{
		SDAI_BOOLEAN* val = attr->Boolean();

		string str;
		val->asStr(str);

		if (str == "T")
			fout << "true";
		else
			fout << "false";
	}
	else if (attrBaseType == BASE_TYPE::sdaiLOGICAL)
	{
		SDAI_LOGICAL* val = attr->Logical();

		string str;
		val->asStr(str);

		str = "\"" + str + "\"";

		fout << str;
	}
	else if (attrBaseType == BASE_TYPE::sdaiENUMERATION)
	{
		SDAI_Enum* val = attr->Enum();

		string str;
		val->asStr(str);

		str = "\"" + str + "\"";

		fout << str;
	}
	else if (attrBaseType == BASE_TYPE::sdaiAGGR)
	{
		STEPaggregate_ptr aggr = attr->Aggregate();

		string str;
		aggr->asStr(str);

		fout << str;

		std::cout << "sdaiAGGR type was found!" << endl;
	}
	else if (attrBaseType == BASE_TYPE::sdaiSELECT)
	{
		string str = attr->asStr();

		if (str.find("#") != -1)
		{
			// remove the null(\0) at the end
			if (str[str.length() - 1] == '\0')
				str = str.substr(0, str.length() - 1);

			str = str.substr(1, str.length() - 1);

			str = "{\"$ref\":\"" + str + "\"}";
		}
		else
		{
			str = str.substr(0, str.length() - 1);

			// replace "1.E" -> "1.0E"
			if (str.find(".E") != -1)
			{
				str.replace(str.find(".E"), 2, ".0E");
			}

			str = "\"" + str + "\"";
		}

		fout << str;
	}
}

void printAttributes(SDAI_Application_instance_ptr inst)
{
	STEPattributeList attrList = inst->attributes;	// attribute list
	int attrCnt = attrList.EntryCount();			// attribute list count

	if (attrCnt == 0)
	{
		fout << endl;
	}
	else
	{
		for (int i = 0; i < attrCnt; ++i)
		{
			STEPattribute* redefAttr = attrList.operator[](i).RedefiningAttr();	// redefined attribute

			if (redefAttr)
				continue;

			STEPattribute* attr = &attrList.operator[](i);

			string attrName = attr->getADesc()->Name();	// attribute name

			fout << ",\n\"" << attrName << "\":";

			BASE_TYPE attrType = attr->Type();	// attribute type

			if (attrType == BASE_TYPE::LIST_TYPE
				|| attrType == BASE_TYPE::SET_TYPE
				|| attrType == BASE_TYPE::ARRAY_TYPE
				|| attrType == BASE_TYPE::BAG_TYPE)
			{
				STEPaggregate_ptr aggr = attr->Aggregate();
				int aggrCnt = aggr->EntryCount();

				fout << "[";

				EntityNode* entNode = (EntityNode*)aggr->GetHead();

				for (int k = 0; k < aggrCnt; ++k)
				{
					string str;
					entNode->asStr(str);

					// remove the null(\0) at the end
					if (str[str.length() - 1] == '\0')
						str = str.substr(0, str.length() - 1);

					// remove "\n"
					while (str.find("\n") != -1)
						str.replace(str.find("\n"), 1, "");

					// remove " "(blank)
					while (str.find(" ") != -1)
						str.replace(str.find(" "), 1, "");

					if (str.find("#") != -1) // instance reference
					{
						if (str.find("(") != -1
							&& str.find(")") != -1)	// multiple instances reference
						{
							str = str.substr(1, str.find(")") - 1);

							string temp, token;
							temp = temp + "[";

							while (str.find(",") != -1)
							{
								token = str.substr(0, str.find(",") - 1);

								temp = temp + "{\"$ref\":\"" + token + "\"},";

								str.replace(str.find("#"), str.find(",") + 1, "");
							}

							temp = temp + "{\"$ref\":\"" + str + "\"}";

							temp = temp + "]";

							str = temp;
						}
						else // single instance reference
						{
							str = str.substr(1, str.length() - 1);
							str = "{\"$ref\":\"" + str + "\"}";
						}
					}
					else if (str.find("(") != -1
						&& str.find(")") != -1) // coordinates or orders
					{
						// replace "0.," -> "0.0,"
						while (str.find(".,") != -1)
						{
							str.replace(str.find(".,"), 2, ".0,");
						}

						// replace "0.)" -> "0.0)"
						while (str.find(".)") != -1)
						{
							str.replace(str.find(".)"), 2, ".0)");
						}

						if (str.find("(") == 0)
						{
							str = str.substr(1, str.find(")") - 1);

							str = "[" + str + "]";
						}
						else
						{
							str = "\"" + str + "\"";
						}
					}

					// replace "0." -> "0.0"
					if (str[str.length() - 1] == '.')
					{
						str.replace(str.find("."), 1, ".0");
					}

					// replace "1.E" -> "1.0E"
					if (str.find(".E") != -1)
					{
						str.replace(str.find(".E"), 2, ".0E");
					}

					if (k == aggrCnt - 1)
						fout << str;
					else
						fout << str << ",";

					entNode = (EntityNode*)entNode->NextNode();
				}

				fout << "]";
			}
			else if (attrType == BASE_TYPE::GENERIC_TYPE)
			{
				string str = attr->asStr();
				fout << str;

				std::cout << "GENERIC_TYPE was found!" << endl;
			}
			else if (attrType == BASE_TYPE::UNKNOWN_TYPE)
			{
				string str = attr->asStr();
				fout << str;

				std::cout << "UNKNOWN_TYPE was found!" << endl;
			}
			else if (attrType == BASE_TYPE::REFERENCE_TYPE)
			{
				printAttribute(attr);
			}
			else
			{
				printAttribute(attr);
			}

			if (i == attrCnt - 1)
				fout << endl;
		}
	}
}


void printReferences(instanceRefs_t* refs, instanceID id)
{
	instanceRefs_t::cvector* v = refs->find(id);

	fout << "\t";

	if (!v)
	{
		fout << "*No references" << endl;
	}
	else
	{
		instanceRefs_t::cvector::const_iterator it(v->begin()), end(v->end());

		fout << "*Instance #" << id << " is referred to by " << v->size() << " other instances: ";

		for (; it != end; it++)
		{
			fout << "#" << *it << " ";
		}

		fout << endl;
	}
}


int EntityCount(string name, InstMgr& instList)
{
	int instCnt = instList.InstanceCount();

	int compEntityCnt = 0;

	for (int i = 0; i < instCnt; ++i)
	{
		SDAI_Application_instance_ptr inst = instList.GetSTEPentity(i);	// i-th instance
		string entName = inst->EntityName();

		if (inst->IsComplex())
		{
			STEPcomplex* comp = (STEPcomplex*)inst;

			while (!comp == NULL)
			{
				SDAI_Application_instance_ptr compInst = (SDAI_Application_instance_ptr)comp;
				entName = compInst->EntityName();

				if (name == entName)
					compEntityCnt++;

				comp = comp->sc;
			}
		}
		else
		{
			if (name == entName)
				compEntityCnt++;
		}
	}

	return compEntityCnt;
}

void WriteJSON(InstMgr& instList, string filePath)
{
	// open json file
	fout.open(filePath.c_str());

	// instance count
	int instCnt = instList.InstanceCount();

	//lazyInstMgr* mgr = new lazyInstMgr;
	//mgr->openFile(sfilePath);

	vector<SDAI_Application_instance_ptr> orderedInstList;
	vector<string> uniqueEntNames;

	// save instances and their names to separate vectors
	for (int i = 0; i < instCnt; ++i)
	{
		SDAI_Application_instance_ptr inst = instList.GetSTEPentity(i);	// i-th instance
		string entName = inst->EntityName();	// entity name

		if (inst->IsComplex())
		{
			STEPcomplex* comp = (STEPcomplex*)inst;

			while (comp != NULL)
			{
				SDAI_Application_instance_ptr compInst = (SDAI_Application_instance_ptr)comp;
				entName = compInst->EntityName();

				orderedInstList.push_back(compInst);
				uniqueEntNames.push_back(entName);

				comp = comp->sc;
			}
		}
		else
		{
			orderedInstList.push_back(inst);
			uniqueEntNames.push_back(entName);
		}
	}

	// sort instances by name
	sort(begin(orderedInstList), end(orderedInstList), [](SDAI_Application_instance_ptr a, SDAI_Application_instance_ptr b)
		{ return (string)(a->EntityName()) < (string)(b->EntityName()); });

	// get unique entity names
	sort(uniqueEntNames.begin(), uniqueEntNames.end());
	auto last = unique(uniqueEntNames.begin(), uniqueEntNames.end());
	uniqueEntNames.erase(last, uniqueEntNames.end());

	fout << "{" << endl;

	int instIndex = 0;

	for (int i = 0; i < (int)uniqueEntNames.size(); ++i)
	{
		int	instCnt = EntityCount(uniqueEntNames[i], instList);

		// transform to lower case
		transform(uniqueEntNames[i].begin(), uniqueEntNames[i].end(), uniqueEntNames[i].begin(), ::tolower);
		fout << "\"" << uniqueEntNames[i] << "\":";

		if (instCnt > 1)
			fout << "[" << endl;

		for (int j = 0; j < instCnt; ++j)
		{
			fout << "{" << endl;

			SDAI_Application_instance_ptr inst = orderedInstList[instIndex];

			// entity ID
			int entID = inst->StepFileId();
			fout << "\"$id\":\"" << entID << "\"";

			// print attributes
			printAttributes(inst);

			fout << "}";

			if (j < instCnt - 1)
				fout << "," << endl;
			else
			{
				if (instCnt != 1)
					fout << endl;
			}

			instIndex++;
		}

		if (instCnt > 1)
			fout << "]";

		if (i < (int)uniqueEntNames.size() - 1)
			fout << "," << endl;
		else
			fout << endl;
	}

	fout << "}" << endl;

	// close json file
	fout.close();

	orderedInstList.clear();
	uniqueEntNames.clear();

	cout << "JSON file generation has been completed!" << endl;

	//delete mgr;
}
*/