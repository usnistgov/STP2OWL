#pragma once

class S2O_Option
{
public:
	S2O_Option();
	~S2O_Option();

	void SetInput(const string& input) { m_input = input; }
	void SetMode(const int& mode);
	void SetSchema(const string& schema) { m_schema = schema; }
	void SetOWL2Profile(const string& profile);
	void SetKeepClassList(const string& path);
	void SetSimpleGeometry(int simpleGeometry);

	string Input(void) const { return m_input; }
	string Output(const string& type);
	OutputMode Mode(void) const { return m_mode; }
	string Schema(void) const { return m_schema; }
	OWL2Profile Profile(void) const { return m_owl2Profile; }
	bool IsKeepClass(const string& name) const;
	bool IsSimpleGeomtry(void) const { return m_simpleGeometry; }

	// Software version (as of June 2021)
	string Version(void) const { return "1.0"; }

private:
	string m_input;
	OutputMode m_mode;
	string m_schema;
	OWL2Profile m_owl2Profile;
	map<string, int> m_classList;
	bool m_simpleGeometry;
	bool m_isKeepClassOn;

// OWL2 Profiles
public:
	bool IsOWL2EL(const string& s);
	bool IsOWL2QL(const string& s);
	bool IsOWL2RL(const string& s);

protected:
	void InitializeOWL2Restrictions();

private:
	vector<string> m_restrictionEL, m_restrictionQL, m_restrictionRL;
};