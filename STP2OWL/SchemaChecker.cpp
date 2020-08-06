#include "stdafx.h"
#include "SchemaChecker.h"


SchemaChecker::SchemaChecker()
{
	InitSchemaNames();
}

SchemaChecker::SchemaChecker(string stpFilePath)
	:m_stpFilePath(stpFilePath)
{
	InitSchemaNames();
	SetSchemaFromSTEPFile();
}

SchemaChecker::~SchemaChecker()
{
	m_schemaMap.clear();
}

const Schema* SchemaChecker::InitializeSchema(Registry*& reg)
{
	string schemaName = GetShortName();

	if (schemaName == "203")
		reg = new Registry(SchemaInitAP203);
	else if (schemaName == "203e2")
		reg = new Registry(SchemaInitAP203E2);
	else if (schemaName == "209")
		reg = new Registry(SchemaInitAP209);
	else if (schemaName == "210e2")
		reg = new Registry(SchemaInitAP210E2);
	else if (schemaName == "210e3")
		reg = new Registry(SchemaInitAP210E3);
	else if (schemaName == "214e3")
		reg = new Registry(SchemaInitAP214E3);
	else if (schemaName == "219")
		reg = new Registry(SchemaInitAP219);
	else if (schemaName == "227")
		reg = new Registry(SchemaInitAP227);
	else if (schemaName == "235")
		reg = new Registry(SchemaInitAP235);
	else if (schemaName == "238")
		reg = new Registry(SchemaInitAP238);
	else if (schemaName == "239")
		reg = new Registry(SchemaInitAP239);
	else if (schemaName == "240")
		reg = new Registry(SchemaInitAP240);
	else if (schemaName == "242")
		reg = new Registry(SchemaInitAP242);
	else if (schemaName == "242e2")
		reg = new Registry(SchemaInitAP242E2);
	else if (schemaName == "ifc2x3")
		reg = new Registry(SchemaInitIFC2X3);
	else if (schemaName == "ifc4")
		reg = new Registry(SchemaInitIFC4);
	else if (schemaName == "iso15926")
		reg = new Registry(SchemaInitISO15926);
	else if (schemaName == "pdm")
		reg = new Registry(SchemaInitPDM);
	else
	{
		cout << "Invalid schema! (" << GetCurrentName() << ")" << endl;
		cout << "Available schemas:203,203e2,210e2,210e3,214e3,219,227,235,238,239,240,242,242e2,IFC2X3,IFC4,ISO15926,PDM" << endl;
		cout << "Schema is set to STEP AP242" << endl;

		SetShortName("242");
		reg = new Registry(SchemaInitAP242);
	}

	const Schema* sc = reg->FindSchema(GetLongName().c_str());

	return sc;
}

bool SchemaChecker::HasMatch(string input, string& output)
{
	for (int i = 0; i < (int)m_schemaMap.size(); ++i)
	{
		string schemaLower = StrUtil::ToLower(m_schemaMap[i].second);
		string schemaUpper = StrUtil::ToUpper(m_schemaMap[i].second);

		if (StrUtil::IsExisting(input, schemaLower)
			|| StrUtil::IsExisting(input, schemaUpper))
		{
			output = m_schemaMap[i].first;
			return true;
		}
	}
	
	return false;
}

string SchemaChecker::GetLongName()
{
	for (int i = 0; i < (int)m_schemaMap.size(); ++i)
	{
		string schemaLower = StrUtil::ToLower(m_schemaMap[i].first);
		string schemaUpper = StrUtil::ToUpper(m_schemaMap[i].first);
		
		if (m_targetSchemaShortName == schemaLower
			|| m_targetSchemaShortName == schemaUpper)
		{
			return m_schemaMap[i].second;
		}
	}
	
	return string("");
}

void SchemaChecker::InitSchemaNames()
{
	m_schemaMap.push_back({ "203", "config_control_design" });
	m_schemaMap.push_back({ "203e2", "ap203_configuration_controlled_3d_design_of_mechanical_parts_and_assemblies_mim_lf" });
	m_schemaMap.push_back({ "209", "ap209_multidisciplinary_analysis_and_design_mim_lf" });
	m_schemaMap.push_back({ "210e2", "ap210_electronic_assembly_interconnect_and_packaging_design_mim_lf" });
	m_schemaMap.push_back({ "210e3", "ap210_electronic_assembly_interconnect_and_packaging_design_mim_lf" });
	m_schemaMap.push_back({ "214e3", "automotive_design" });
	m_schemaMap.push_back({ "219", "dimensional_inspection_schema" });
	m_schemaMap.push_back({ "227", "plant_spatial_configuration" });
	m_schemaMap.push_back({ "235", "engineering_properties_schema" });
	m_schemaMap.push_back({ "238", "integrated_cnc_schema" });
	m_schemaMap.push_back({ "239", "ap239_product_life_cycle_support_arm_lf" });
	m_schemaMap.push_back({ "240", "process_planning_schema" });
	m_schemaMap.push_back({ "242", "ap242_managed_model_based_3d_engineering_mim_lf" });
	m_schemaMap.push_back({ "242e2", "ap242_managed_model_based_3d_engineering_mim_lf" });
	m_schemaMap.push_back({ "ifc2x3", "IFC2X3" });
	m_schemaMap.push_back({ "ifc4", "IFC4" });
	m_schemaMap.push_back({ "iso15926", "lifecycle_integration_schema" });
	m_schemaMap.push_back({ "pdm", "pdm_schema" });
}

void SchemaChecker::SetSchemaFromSTEPFile()
{
	ifstream myfile(m_stpFilePath);

	if (myfile.is_open())
	{
		string line;

		while (getline(myfile, line))
		{
			if (StrUtil::IsExisting(line, "FILE_SCHEMA"))
			{
				// check the same line
				string tmp = StrUtil::GetStringBetween(line, "'", "'");
				
				if (StrUtil::IsExisting(line, "'"))
					GetCurrentNameFromLine(tmp);

				if (HasMatch(tmp, m_targetSchemaShortName))
					break;

				// check the next line
				getline(myfile, line);

				tmp = StrUtil::GetStringBetween(line, "'", "'");

				if (StrUtil::IsExisting(line, "'"))
					GetCurrentNameFromLine(tmp);

				if (HasMatch(tmp, m_targetSchemaShortName))
					break;

				break;
			}
		}

		myfile.close();
	}
}

void SchemaChecker::GetCurrentNameFromLine(string line)
{
	if (StrUtil::IsExisting(line, " {"))
		m_currentSchemaName = line.substr(0, line.find(" {"));
	else if (StrUtil::IsExisting(line, "{"))
		m_currentSchemaName = line.substr(0, line.find("{"));
	else
		m_currentSchemaName = line;
}
