#pragma once

#include <string>
#include <iostream>	
#include <vector>

class User
{
private:
	/* The age under we will consider a person to be a child; */
	int m_iIsconsideredChild;
	
    std::string m_sName;
    int m_iYearsOld;
    bool m_bIsChild;
    int m_iTotalCost;

	std::vector<std::string> m_sListOfAttractions;
protected:

public:
	void SetUserName(std::string);
	std::string GetUserName();

	void SetAge(int);
	int GetAge();

	void SetTotalUserCost(int);
	int GetTotalUserCost();

	void SetActivity(std::string);
	std::vector<std::string> GetActivities();

	bool IsChild();

	User(std::string sName, int iYearsOld);
	~User();
};

