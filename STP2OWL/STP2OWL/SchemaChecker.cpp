#include "stdafx.h"
#include "SchemaChecker.h"


SchemaChecker::SchemaChecker()
{
	InitSchemaNames();
}

SchemaChecker::~SchemaChecker()
{
	m_schemaMap.clear();
}

bool SchemaChecker::InitializeSchema(Registry*& reg, S2O_Option& opt)
{
	if (!opt.Input().empty())
	{
		SetSTEPFilePath(opt.Input());
		SetSchemaFromSTEPFile();
	}
	else
	{
		SetShortName(opt.Schema());
		opt.SetMode(2);
		opt.SetInput(opt.Schema()); // For output file name
	}
	
	string schemaName = GetShortName();
	schemaName = StrUtil::ToLower(schemaName);

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
	}
	
	if (reg)
	{
		cout << "\n[Schema: " << GetLongName() << " (" << GetShortName() << ")" << "]" << endl << endl;
		return true;
	}
	
	return false;
}

bool SchemaChecker::HasMatch(string input, string& output) const
{
	for (int i = 0; i < (int)m_schemaMap.size(); ++i)
	{
		string schemaLower = StrUtil::ToLower(m_schemaMap[i].second);
		string schemaUpper = StrUtil::ToUpper(m_schemaMap[i].second);

		if (StrUtil::Exist(input, schemaLower)
			|| StrUtil::Exist(input, schemaUpper))
		{
			output = m_schemaMap[i].first;
			return true;
		}
	}
	
	return false;
}

const string& SchemaChecker::GetLongName()
{
	for (int i = 0; i < (int)m_schemaMap.size(); ++i)
	{
		string schemaLower = StrUtil::ToLower(m_schemaMap[i].first);
		string schemaUpper = StrUtil::ToUpper(m_schemaMap[i].first);
		
		if (m_targetSchemaShortName == schemaLower
			|| m_targetSchemaShortName == schemaUpper)
		{
			m_targetSchemaLongName = m_schemaMap[i].second;
			break;
		}
	}
	
	return m_targetSchemaLongName;
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
			if (StrUtil::Exist(line, "FILE_SCHEMA"))
			{
				// check the same line
				string tmp = StrUtil::GetStringBetween(line, "'", "'");
				
				if (StrUtil::Exist(line, "'"))
					GetCurrentNameFromLine(tmp);

				if (HasMatch(tmp, m_targetSchemaShortName))
					break;

				// check the next line
				getline(myfile, line);

				tmp = StrUtil::GetStringBetween(line, "'", "'");

				if (StrUtil::Exist(line, "'"))
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
	if (StrUtil::Exist(line, " {"))
		m_currentSchemaName = line.substr(0, line.find(" {"));
	else if (StrUtil::Exist(line, "{"))
		m_currentSchemaName = line.substr(0, line.find("{"));
	else
		m_currentSchemaName = line;
}
