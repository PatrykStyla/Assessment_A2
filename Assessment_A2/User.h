#pragma once

#include <string>
#include <iostream>	
#include <vector>
#include "Location.h"

class User
{
private:
	/* The age under we will consider a person to be a child; */
	static const int m_iIsConsideredChild = 9;
	static const int m_iIsConsideredAdult = 18;
	
    std::string m_sName;
    int m_iYearsOld;
    bool m_bIsChild;
    float m_iTotalCost = 0;

	std::vector<Location::InvividualActivity> m_sListOfAttractions;
public:
	static int GetIsConsideredChild();
	static int GetIsConsideredAdult();


	void SetUserName(std::string);
	std::string GetUserName();

	void SetAge(int);
	int GetAge();

	void SetTotalUserCost(float);
	float GetTotalUserCost();

	void SetActivity(Location::InvividualActivity&);
	std::vector<Location::InvividualActivity> GetActivities();
	void RemoveActivity(int);

	bool IsChild();

	User(std::string sName, int iYearsOld);
	~User();
};

