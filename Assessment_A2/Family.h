#pragma once

#include <vector>
#include <string>
#include "User.h"
#include "Location.h"

class Family
{
private:
	// A family will consts of a vector of users each with its own age, name, etc...
	std::vector<User> m_clsUsers;
	Location Locations;

protected:

public:

private:
	Family();
};

