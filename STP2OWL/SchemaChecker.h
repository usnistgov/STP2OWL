#pragma once


typedef pair<string, string> StrPair;

class SchemaChecker
{
public:
	SchemaChecker();
	SchemaChecker(string stpFilePath);
	~SchemaChecker();

	// Initialize schema
	const Schema* InitializeSchema(Registry*& reg);
	
	// Set the short schema name.
	void SetShortName(string shortName) { m_targetSchemaShortName = shortName; }
	
	// Get the short schema name.
	string GetShortName() { return m_targetSchemaShortName; }
	
	// Get the long schema name.
	string GetLongName();

protected:
	void InitSchemaNames();
	void SetSchemaFromSTEPFile();
	void GetCurrentNameFromLine(string line);
	bool HasMatch(string input, string& output);

	//void SetShortName(string shortName) { m_targetSchemaShortName = shortName; }
	string GetCurrentName() { return m_currentSchemaName; }

private:
	string m_stpFilePath;
	string m_targetSchemaShortName;
	string m_targetSchemaLongName;
	string m_currentSchemaName;

	vector<StrPair> m_schemaMap;
};