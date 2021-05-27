#pragma once


typedef pair<string, string> StrPair;

class SchemaChecker
{
public:
	SchemaChecker();
	~SchemaChecker();

	// Set STEP file path
	void SetSTEPFilePath(const string& filePath) { m_stpFilePath = filePath; }

	// Initialize schema
	bool InitializeSchema(Registry*& reg, S2O_Option& opt);
	
	// Set the short schema name.
	void SetShortName(const string& shortName) { m_targetSchemaShortName = shortName; }
	
	// Get the short schema name.
	const string& GetShortName() const { return m_targetSchemaShortName; }
	
	// Get the long schema name.
	const string& GetLongName();

protected:
	void InitSchemaNames();
	void SetSchemaFromSTEPFile();
	void GetCurrentNameFromLine(string line);
	bool HasMatch(string input, string& output) const;

	//void SetShortName(string shortName) { m_targetSchemaShortName = shortName; }
	const string& GetCurrentName() const { return m_currentSchemaName; }

private:
	string m_stpFilePath;
	string m_targetSchemaShortName;
	string m_targetSchemaLongName;
	string m_currentSchemaName;

	vector<StrPair> m_schemaMap;
};