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
	void SetSimpleGeometry(const int& simpleGeometry);

	const string& Input(void) const { return m_input; }
	const string& Output(const string& type);
	const OutputMode& Mode(void) const { return m_mode; }
	const string& Schema(void) const { return m_schema; }
	const OWL2Profile& Profile(void) const { return m_owl2Profile; }
	bool IsKeepClass(const string& name) const;
	bool IsSimpleGeomtry(void) const { return m_simpleGeometry; }

	// Software version (as of June 2021)
	const string Version(void) const { return "1.0"; }

private:
	string m_input;
	string m_output;
	OutputMode m_mode;
	string m_schema;
	OWL2Profile m_owl2Profile;
	map<string, int> m_classList;
	bool m_simpleGeometry;
	bool m_isKeepClassOn;

// OWL2 Profiles
public:
	bool IsOWL2EL(const string& s) const;
	bool IsOWL2QL(const string& s) const;
	bool IsOWL2RL(const string& s) const;

protected:
	void InitializeOWL2Restrictions();

private:
	vector<string> m_restrictionEL, m_restrictionQL, m_restrictionRL;
};