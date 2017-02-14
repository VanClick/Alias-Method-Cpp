#pragma once

/*
 * learn from http://www.keithschwarz.com/darts-dice-coins/
 */

class AliasMethod
{
public:
	AliasMethod(const double weights[], const int size);
	~AliasMethod();
	int next();
private:
	int* m_alias;
	double* m_probability;
	const int m_size;
};
