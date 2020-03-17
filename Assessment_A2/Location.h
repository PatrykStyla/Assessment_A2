#pragma once

#include <vector>
#include <string>
#include <map>
#include "json.hpp"

class Location
{
public:
	// The locations the company has to offer
	std::vector<std::string> m_sLocation;
	std::vector<float> m_fPrice;
	std::vector<int> m_fMinimumTravellers;

	std::vector<std::string> m_sActivity;
	std::vector<float> m_fCostPerPerson;
	/* This lists the available locations based on the activity */
	std::vector<std::vector<std::string>> m_sAvailableLocations;
	/* This lists the available activities based on the location */
    std::vector<std::vector<std::string>> m_sAvaliableActivities;

    // We use maps to match the string to its corresponsing value
    // maps are dynamically initialized based on the initial values
    // Not all maps may be used for this program.

	/* location => price */
	std::map<std::string, float> map_LocationPrice;
    /* location => available activities*/
    std::map<std::string, std::vector<std::string>> map_ActivityLocation;
	/* activity => price*/
	std::map<std::string, float> map_ActivityPrice;
	/* activity => available locations*/
	std::map<std::string, std::vector<std::string>> map_LocationActivity;

	Location();
	void InitLocationPriceMap();
	void InitActivityPriceMap();
	void InitActivityLocationMap();
	void InitLocationActivityMap();

    void InitFromJson();
    void LoadFromJson();
};