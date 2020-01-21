#pragma once

#include <string>

class User
{
private:
	// The age under we will consider a person to be a child;
	int m_iIsconsideredChild;


protected:

public:
	std::string m_sName;
	int m_iYearsOld;
	bool m_bIsChild;

	User(std::string sName, int iYearsOld);
};

