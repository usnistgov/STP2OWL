#pragma once

template<class T>
void S2OThrowExceptionIf(bool condition)
{
	if (condition)
		throw T();
}

template<class T, class P1>
void S2OThrowExceptionIf(bool condition, P1 p1)
{
	if (condition)
		throw T(p1);
}

template<class T, class P1, class P2>
void S2OThrowExceptionIf(bool condition, P1 p1, P2 p2)
{
	if (condition)
		throw T(p1, p2);
}