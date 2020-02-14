#pragma once

#include <string>
#include <iostream>	

class User
{
private:
	/* The age under we will consider a person to be a child; */
	int m_iIsconsideredChild;
	
    std::string m_sName;
    int m_iYearsOld;
    bool m_bIsChild;
    int m_iTotalCost;

protected:

public:
	void SetUserName(std::string);
	std::string GetUserName();

	void SetAge(int);
	int GetAge();

	void SetTotalUserCost(int);
	int GetTotalUserCost();

	bool IsChild();

	User(std::string sName, int iYearsOld);
	~User();
};

