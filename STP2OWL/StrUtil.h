#pragma once

class StrUtil
{
public:
	// Convert to lower cases.
	static string ToLower(string str);

	// Convert to upper cases.
	static string ToUpper(string str);

	// Remove characters same as the target from the string.
	static string RemoveCharacter(string str, string target);

	// Replace the string 'from' to the string 'to'.
	static string ReplaceCharacter(string str, string from, string to);

	// Get the string between the first of 'front' and the last of 'back'.
	static string GetStringBetween(string str, string front, string back);

	// Get the string between left and right parentheses.
	static string GetStringBetweenParenthesis(string str, int leftParenLocation);

	// Split the string by the given separator.
	static void SplitStringBySeparator(string str, string sep, vector<string>& subStrs);

	// Split the string by comma in parentheses.
	static void SplitStringByCommaInParentheses(string str, vector<string>& subStrs);

	// Check if the target string is existing.
	static bool IsExisting(string str, string target);

	// Check if two strings are equal.
	static bool Equals(string a, string b);

	// Check if the string starts with the target string.
	static bool StartsWith(string str, string target);

	// Check if the string ends with the target string.
	static bool EndsWith(string str, string target);
};