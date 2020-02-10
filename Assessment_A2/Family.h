#pragma once

#include <vector>
#include <string>
#include "User.h"
#include "Location.h"

#include <memory>

class Family
{
private:
    /* Location */
    std::string m_sLocation;

protected:

public:
	Family() {};
	Family(std::string);
	~Family();
	/** The name of the family */
	std::string m_sFamilyName;
	/** A family will consists of a vector of users each with its own age, name, etc... */
	std::vector<std::shared_ptr<User>> m_clsUsers;
	/** We will add all the expenses of all users */
	int m_iTotalCostOfUsers;

	void SetUserLocation(std::string sLocation);
private:

};

