#pragma once

#include <vector>
#include <string>
#include "User.h"
#include "Location.h"

#include <memory>

class Family
{
private:
    /* Location class */
    std::string m_sLocation;
	/** We will add all the expenses of all users */
	int m_iTotalCostOfUsers = 0;
	std::string m_sFamilyName;
    /** A family will consists of a vector of users each with its own age, name, etc... */
    std::vector<std::shared_ptr<User>> m_clsUsers;
protected:

public:
	Family() {  };
	Family(std::string);
	~Family();

	void SetFamilyLocation(std::string sLocation);
	std::string GetFamilyLocation();

	void SetFamilyTotalCost(int);
	int GetFamilyTotalCost();

	void SetFamilyName(std::string);
	std::string GetFamilyName();

	void SetFamilyUser(std::shared_ptr<User>);
	std::vector<std::shared_ptr<User>> GetFamilyUsers();


private:

};

