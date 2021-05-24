//
#include "stdafx.h"
#include "STP2ONT.h"
#include "OntWriter.h"
#include "SchemaChecker.h"
#include "StopWatch.h"

// Print out the usage
void PrintUsage(string exe, S2O_Option* opt)
{
	cout << endl;
	cout << "//////////////////////////////////////////////////////////////" << endl;
	cout << "//  OntoSTEP 2.0: NIST STEP to OWL Translator(STP2OWL) " << opt->Version() << "  //" << endl;
	cout << "//////////////////////////////////////////////////////////////" << endl;
	cout << endl;
	cout << "[Usage]" << endl;
	cout << " " << exe << " option1 value1 option2 value2.." << endl;
	cout << endl;
	cout << "[Options]" << endl;
	cout << " --input      STEP file path" << endl;
	cout << " --schema     STEP Schema (Available input:203,203e2,210e2,210e3,214e3,219,227,235,238,239,240,242,242e2,IFC2X3,IFC4,ISO15926,PDM)" << endl;
	cout << " --profile    OWL 2 Profile (Available input:DL,EL,QL,RL) default=DL" << endl;
	cout << " --mode       Output Mode (0:schema,instance, 1:instance, 2:schema, 3:integrated) default=" << (int)opt->Mode() << endl;
	cout << " --simgeom    Simple Geometry (1:yes, 0:no) default=" << opt->IsSimpleGeomtry() << endl;
	cout << " --filter     CSV file path for STEP entity names to be kept in the OWL instances" << endl;
	cout << endl;
	cout << "[Examples]" << endl;
	cout << " " << exe << " --input Model.stp --profile RL --simgeom 1" << endl;
	cout << " " << exe << " --mode 1 --input Model.step --profile EL --filter target.csv" << endl;
	cout << " " << exe << " --schema 210e2 --profile QL" << endl;
	cout << endl;
	cout << "[Disclaimers]" << endl;
	cout << " This software was developed at the National Institute of Standards and Technology by" << endl;
	cout << " employees of the Federal Government in the course of their official duties. Pursuant" << endl;
	cout << " to Title 17 Section 105 of the United States Code this software is not subject to" << endl;
	cout << " copyright protection and is in the public domain. This software is an experimental" << endl;
	cout << " system. NIST assumes no responsibility whatsoever for its use by other parties, and" << endl;
	cout << " makes no guarantees, expressed or implied, about its quality, reliability, or any" << endl;
	cout << " other characteristic. NIST Disclaimer : https://www.nist.gov/disclaimer" << endl;
	cout << endl;
	cout << " This software is provided by NIST as a public service. You may use, copy and" << endl;
	cout << " distribute copies of the software in any medium, provided that you keep intact this" << endl;
	cout << " entire notice. You may improve, modify and create derivative works of the software" << endl;
	cout << " or any portion of the software, and you may copy and distribute such modifications" << endl;
	cout << " or works. Modified works should carry a notice stating that you changed the software" << endl;
	cout << " and should note the date and nature of any such change. Please explicitly" << endl;
	cout << " acknowledge NIST as the source of the software." << endl;
	cout << endl;
	cout << "[Credits]" << endl;
	cout << " -The translator is based on the STEPcode (See https://github.com/stepcode/stepcode)" << endl;
	cout << " -Developed by Soonjo Kwon at NIST in 2020" << endl;
}

// Set option values
bool SetOption(int argc, char* argv[], S2O_Option* opt)
{
	// Print out usage
	if (argc < 2)
	{
		string a = argv[0];
		PrintUsage(a, opt);
		return false;
	}

	bool inputFlag = false;
	bool batchFlag = false;

	// Set options
	for (int i = 1; i < argc; ++i)
	{
		string token(argv[i]);

		if (token != "--input"
			&& token != "--schema"
			&& token != "--mode"
			&& token != "--simgeom"
			&& token != "--profile"
			&& token != "--filter")
		{
			cout << "No such option: " << token << endl;
			return false;
		}
		else
		{
			if (argc == i + 1)
			{
				cout << "No value for the option: " << token << endl;
				return false;
			}
			else
			{
				string token1(argv[i + 1]);

				if (token == "--input")
				{
					inputFlag = true;
					opt->SetInput(token1);
				}
				else if (token == "--schema")
				{
					opt->SetSchema(token1);
				}
				else if (token == "--mode")
				{
					int mode = stoi(token1);
					opt->SetMode(mode);

					if (mode != 0
						&& mode != 1
						&& mode != 2
						&& mode != 3)
					{
						cout << "mode must be one of 0, 1, 2, 3." << endl;
						return false;
					}
				}
				else if (token == "--simgeom")
				{
					int simgeom = stoi(token1);
					opt->SetSimpleGeometry(simgeom);

					if (simgeom != 0
						&& simgeom != 1)
					{
						cout << "simgeom must be either 0 or 1." << endl;
						return false;
					}
				}
				else if (token == "--profile")
				{
					opt->SetOWL2Profile(token1);

					if (token1 != "DL"
						&& token1 != "EL"
						&& token1 != "QL"
						&& token1 != "RL")
					{
						cout << "profile must be one of DL, EL, QL, RL." << endl;
						return false;
					}
				}
				else if (token == "--filter")
				{
					opt->SetKeepClassList(token1);
				}

				++i;
			}
		}
	}

	// Check input path
	if (opt->Input().empty()
		&& opt->Schema().empty())
	{
		cout << "Please input a STEP file or a schema." << endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	S2O_Option opt; // Option for STEP to OWL translator

	int status = -1; // Translation status

#if _DEBUG
	opt.SetInput("C:\\Users\\User\\Desktop\\TestCases\\test.step");
	opt.SetMode(0);
	opt.SetOWL2Profile("QL");
	opt.SetSimpleGeometry(1);
	opt.SetSchema("242");
#else
	if (!SetOption(argc, argv, &opt))
		return status;
#endif

	SchemaChecker* schemaChecker = new SchemaChecker();

	if (opt.Input().empty())
	{
		schemaChecker->SetShortName(opt.Schema());
		opt.SetMode(2);
		opt.SetInput(opt.Schema());
	}
	else
	{
		schemaChecker->SetSTEPFilePath(opt.Input());
	}
	
	Registry* reg = nullptr;
	const Schema* sc = schemaChecker->InitializeSchema(reg);

	cout << endl;
	cout << "[Schema: " << schemaChecker->GetLongName();
	cout << " (" << schemaChecker->GetShortName() << ")" << "]" << endl << endl;

	InstMgr* instList = new InstMgr();
	STP2ONT* s2o = new STP2ONT(schemaChecker->GetLongName());

	try
	{
		StopWatch sw;
		sw.Start();

		sw.Lap();

		s2o->TranslateSchema(sc, &opt);

		cout << "STEP Schema to Ontology translation completed!" << endl;

		sw.Lap();

		if (opt.Mode() != OutputMode::Schema_Only)
		{
			STEPfile sfile(*reg, *instList, "", false);
			sfile.ReadExchangeFile(opt.Input());

			// if there's an error, exit (e.g. wrong file path)
			if (sfile.Error().severity() <= Severity::SEVERITY_USERMSG)
				cout << sfile.Error().UserMsg();

			S2OThrowExceptionIf<S2OException>(sfile.Error().severity() < Severity::SEVERITY_WARNING, "Fatal error! Translaton aborted.");

			s2o->TranslateInstances(instList, &opt);

			cout << "STEP Instances to Ontology translation completed!" << endl;

			sw.Lap();
		}

		// writer
		OntWriter ow;
		ow.WriteOWLFunctionalSyntax(s2o, opt);

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

	return status;
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