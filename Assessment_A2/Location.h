#pragma once

#include <vector>
#include <string>

class Location
{
public:
	// The locations the company has to offer
	std::vector<std::string> m_sLocation;
	std::vector<float> m_fPrice;
	std::vector<float> m_fMinimumTravellers;

	std::vector<std::string> m_sActivity;
	std::vector<float> m_fCostPerPerson;
	std::vector<std::vector<std::string>> m_sAvailableLocations;

	Location();
};

