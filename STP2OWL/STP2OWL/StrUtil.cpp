#include "stdafx.h"
#include "StrUtil.h"

string StrUtil::ToLower(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	return str;
}

string StrUtil::ToUpper(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);

	return str;
}

string StrUtil::RemoveCharacter(string str, string target)
{
	int location = static_cast<int>(str.find(target));

	while (location != string::npos)
	{
		str.replace(str.find(target), target.length(), "");

		location = static_cast<int>(str.find(target));
	}

	return str;
}

string StrUtil::ReplaceCharacter(string str, string from, string to)
{
	int location = static_cast<int>(str.find(from));

	while (location != string::npos)
	{
		str.replace(location, from.length(), to);

		location = static_cast<int>(str.find(from, location + to.length()));
	}

	return str;
}

string StrUtil::GetStringBetween(string str, string front, string back)
{
	if (str.find(front) != string::npos
		&& str.find(back) != string::npos)
	{
		int frontLocation = static_cast<int>(str.find(front) + front.length());
		int backLocation = static_cast<int>(str.find_last_of(back));
		int lengthBetween = backLocation - frontLocation;

		str = str.substr(frontLocation, lengthBetween);
	}

	return str;
}
 
string StrUtil::GetStringBetweenParenthesis(string str, int leftParenLocation)
{	
	int flag = 0;
	int rightParenLocation = 0;

	string subStr;

	for (int i = leftParenLocation; i < str.length(); ++i)
	{
		string ch(1, str.at(i));

		if (Equal(ch, "("))
			flag++;
		else if (Equal(ch, ")"))
			flag--;

		if (flag == 0)
		{
			rightParenLocation = i;
			break;
		}
	}

	if (flag == 0)
	{
		int subStrLocation = leftParenLocation + 1;
		int subStrLength = rightParenLocation - subStrLocation;
		subStr = str.substr(subStrLocation, subStrLength);
	}

	return subStr;
}

void StrUtil::SplitStringBySeparator(string str, string sep, vector<string>& subStrs)
{
	int location = static_cast<int>(str.find(sep));

	while (location != string::npos)
	{
		subStrs.push_back(str.substr(0, location));
		str = str.substr(location + sep.length(), str.length());

		location = static_cast<int>(str.find(sep));
	}

	subStrs.push_back(str);
}

void StrUtil::SplitStringByCommaInParentheses(string str, vector<string>& subStrs)
{
	str = GetStringBetweenParenthesis(str, 0);

	string sep = ","; // comma
	string leftParen = "(";
	string rightParen = ")";

	str = RemoveCharacter(str, " ");

	// list of list OR array of array (e.g. (0,0,0),(0,0,0) )
	if (Exist(str, leftParen)
		&& Exist(str, rightParen)
		&& Exist(str, sep)
		&& StartWith(str, leftParen)
		&& EndWith(str, rightParen))
	{
		string token;

		int location = 0;

		while (location != string::npos)
		{
			token = GetStringBetweenParenthesis(str, 0);
			token = leftParen + token + rightParen;

			if (Equal(token, str))
				break;

			subStrs.push_back(token);
			str = str.substr(location + token.length() + 1, str.length());

			location = static_cast<int>(str.find(leftParen));
		}

		if (Exist(str, leftParen)
			&& Exist(str, rightParen))
		{
			subStrs.push_back(str);	// do not save null string.
		}
	}
	else // list OR array (e.g. 0,0,0 )
	{
		SplitStringBySeparator(str, sep, subStrs);
	}
}

bool StrUtil::Exist(string str, string target)
{
	if (str.find(target) == -1)
		return false;

	return true;
}

bool StrUtil::Equal(string a, string b)
{
	if (a == b)
		return true;
	
	return false;
}

bool StrUtil::StartWith(string str, string target)
{
	if (str.find(target) == 0)
		return true;

	return false;
}

bool StrUtil::EndWith(string str, string target)
{
	if (str.find_last_of(target) == str.length() - 1)
		return true;
	//if (str.find(target) + target.length() == str.length())
	//	return true;
	
	return false;
}
