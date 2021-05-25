#include "stdafx.h"
#include "S2O_Option.h"

S2O_Option::S2O_Option()
	:m_mode(OutputMode::Separated),
    m_owl2Profile(OWL2Profile::OWL2_DL),
    m_simpleGeometry(false),
    m_isKeepClassOn(false)
{
    InitializeOWL2Restrictions();
}

S2O_Option::~S2O_Option()
{
}

void S2O_Option::SetMode(const int& mode)
{
    if (mode == 0)
        m_mode = OutputMode::Separated;
    else if (mode == 1)
        m_mode = OutputMode::Instance_Only;
    else if (mode == 2)
        m_mode = OutputMode::Schema_Only;
    else if (mode == 3)
        m_mode = OutputMode::Integrated;
}

void S2O_Option::SetOWL2Profile(const string& profile)
{
	if (profile == "RL")
		m_owl2Profile = OWL2Profile::OWL2_RL;
	else if (profile == "QL")
		m_owl2Profile = OWL2Profile::OWL2_QL;
	else if (profile == "EL")
		m_owl2Profile = OWL2Profile::OWL2_EL;
}

void S2O_Option::SetKeepClassList(const string& path)
{
    // Create an input filestream
    ifstream myFile(path);

    // Make sure the file is open
    if (!myFile.is_open()) 
        throw runtime_error("Could not open file");

    // Helper vars
    string line, token;
    char delimiter = ',';

    // Read the column names
    if (myFile.good())
    {
        // Read data, line by line
        while (getline(myFile, line))
        {
            // Create a stringstream of the current line
            stringstream ss(line);

            while (getline(ss, token, delimiter))
            {
                transform(token.begin(), token.end(), token.begin(), ::tolower);
                m_classList.insert({ token , 1 });
            }
        }

        // Close file
        myFile.close();

        m_isKeepClassOn = true;
    }
}

void S2O_Option::SetSimpleGeometry(int simpleGeometry)
{
    if (simpleGeometry == 1)
        m_simpleGeometry = true;
}

string S2O_Option::Output(const string& type)
{
    string output = m_input + "_" + type;

    if (m_owl2Profile == OWL2Profile::OWL2_DL)
    {
        output += "_DL.owl";
    }
    else if (m_owl2Profile == OWL2Profile::OWL2_EL)
    {
        output += "_EL.owl";
    }
    else if (m_owl2Profile == OWL2Profile::OWL2_RL)
    {
        output += "_RL.owl";
    }
    else if (m_owl2Profile == OWL2Profile::OWL2_QL)
    {
        output += "_QL.owl";
    }

    return output;
}

bool S2O_Option::IsKeepClass(const string& name) const
{
    if (!m_isKeepClassOn
        || m_classList.find(name) != m_classList.end())
        return true;

    return false;
}

bool S2O_Option::IsOWL2EL(const string& s)
{
    for (int i = 0; i < (int)m_restrictionEL.size(); ++i)
    {
        if (StrUtil::Exist(s, m_restrictionEL[i]))
        {
            return false;
        }
    }
        
    return true;
}

bool S2O_Option::IsOWL2QL(const string& s)
{
    for (int i = 0; i < (int)m_restrictionQL.size(); ++i)
    {
        if (StrUtil::Exist(s, m_restrictionQL[i]))
        {
            return false;
        }
    }

    return true;
}

bool S2O_Option::IsOWL2RL(const string& s)
{
    for (int i = 0; i < (int)m_restrictionRL.size(); ++i)
    {
        if (StrUtil::Exist(s, m_restrictionRL[i]))
        {
            return false;
        }
    }

    return true;
}

void S2O_Option::InitializeOWL2Restrictions()
{
    // EL
    m_restrictionEL.push_back("ObjectAllValuesFrom");
    m_restrictionEL.push_back("DataAllValuesFrom");
    m_restrictionEL.push_back("ObjectMaxCardinality");
    m_restrictionEL.push_back("ObjectMinCardinality");
    m_restrictionEL.push_back("ObjectExactCardinality");
    m_restrictionEL.push_back("DataMaxCardinality");
    m_restrictionEL.push_back("DataMinCardinality");
    m_restrictionEL.push_back("DataExactCardinality");
    m_restrictionEL.push_back("ObjectUnionOf");
    m_restrictionEL.push_back("DisjointUnion");
    m_restrictionEL.push_back("DataUnionOf");
    m_restrictionEL.push_back("ObjectComplementOf");
    m_restrictionEL.push_back("ObjectOneOf");
    m_restrictionEL.push_back("DataOneOf");
    m_restrictionEL.push_back("DisjointObjectProperties");
    m_restrictionEL.push_back("DisjointDataProperties");
    m_restrictionEL.push_back("IrreflexiveObjectProperty");
    m_restrictionEL.push_back("InverseObjectProperties");
    m_restrictionEL.push_back("FunctionalObjectProperty");
    m_restrictionEL.push_back("InverseFunctionalObjectProperty");
    m_restrictionEL.push_back("SymmetricObjectProperty");
    m_restrictionEL.push_back("AsymmetricObjectProperty");
    
    // QL
    m_restrictionQL.push_back("ObjectSomeValuesFrom");
    m_restrictionQL.push_back("DataSomeValuesFrom");
    m_restrictionQL.push_back("ObjectHasSelf");
    m_restrictionQL.push_back("ObjectHasValue");
    m_restrictionQL.push_back("DataHasValue");
    m_restrictionQL.push_back("ObjectOneOf");
    m_restrictionQL.push_back("ObjectIntersectionOf");
    m_restrictionQL.push_back("DataOneOf");
    m_restrictionQL.push_back("ObjectAllValuesFrom");
    m_restrictionQL.push_back("DataAllValuesFrom");
    m_restrictionQL.push_back("ObjectMaxCardinality");
    m_restrictionQL.push_back("ObjectMinCardinality");
    m_restrictionQL.push_back("ObjectExactCardinality");
    m_restrictionQL.push_back("DataMaxCardinality");
    m_restrictionQL.push_back("DataMinCardinality");
    m_restrictionQL.push_back("DataExactCardinality");
    m_restrictionQL.push_back("ObjectUnionOf");
    m_restrictionQL.push_back("DisjointUnion");
    m_restrictionQL.push_back("DataUnionOf");
    m_restrictionQL.push_back("SubObjectPropertyOf");
    m_restrictionQL.push_back("FunctionalObjectProperty");
    m_restrictionQL.push_back("InverseFunctionalObjectProperty");
    m_restrictionQL.push_back("FunctionalDataProperty");
    m_restrictionQL.push_back("TransitiveObjectProperty");
    m_restrictionQL.push_back("HasKey"); 

    // RL
    m_restrictionRL.push_back("DisjointUnion");
    m_restrictionRL.push_back("ReflexiveObjectProperty");
    m_restrictionRL.push_back("ObjectMaxCardinality");
    m_restrictionRL.push_back("ObjectMinCardinality");
    m_restrictionRL.push_back("ObjectExactCardinality");
    m_restrictionRL.push_back("DataMaxCardinality");
    m_restrictionRL.push_back("DataMinCardinality");
    m_restrictionRL.push_back("DataExactCardinality");
    m_restrictionRL.push_back("ObjectAllValuesFrom");
    m_restrictionRL.push_back("ObjectOneOf");
    m_restrictionRL.push_back("ObjectUnionOf");
}
