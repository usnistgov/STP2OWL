#include "stdafx.h"
#include "S2OException.h"

S2OException::S2OException()
{
}

S2OException::S2OException(string message)
	: m_message(message)
{
}

S2OException::~S2OException()
{
}