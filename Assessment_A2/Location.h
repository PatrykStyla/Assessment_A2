#pragma once

#include <vector>
#include <string>

enum class UserLocation
{
    Floria, Paris, Milan, Tokyo,
};

class Location
{
public:
	// The locations the company has to offer
	std::vector<std::string> m_sLocation;
	std::vector<float> m_fPrice;
	std::vector<float> m_fMinimumTravellers;

	std::vector<std::string> m_sActivity;
	std::vector<float> m_fCostPerPerson;
	/* This lists the available locations based on the activity */
	std::vector<std::vector<std::string>> m_sAvailableLocations;
	/* This lists the available activities based on the location */
	std::vector<std::vector<std::string>> m_sAvaliableActivities;

	Location();
};

