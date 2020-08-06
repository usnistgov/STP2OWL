#pragma once


class S2OException
{
public:
	S2OException();
	S2OException(string message);
	~S2OException();

	string GetMessage() const { return m_message; }

private:
	string m_message;
};