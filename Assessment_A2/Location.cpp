#include "Location.h"
#include <fstream>

using json = nlohmann::json;

Location::Location()
{
    // We will try to read the data from a json file that can be edited by an admin
    // This json simulates a database
    InitFromJson();

	// Initialize the values
	m_sLocation =			{ "Florida", "Paris", "Milan", "Tokyo" };
	m_fPrice =				{ 1300.0f, 600.0f, 950.0f, 1800.0f };
	m_fMinimumTravellers =	{ 4, 2, 2, 4 };

	m_sActivity =			{"Theme Park", "Guided Tours", "Sky Diving"};
	m_fCostPerPerson =		{ 80.0f, 35.0f, 110.0f };
	m_sAvailableLocations =	
    { 
		{"Florida", "Paris" ,"Tokyo"},
		{"Paris", "Milan" ,"Tokyo"},
		{"Florida"}
	};

	m_sAvaliableActivities = 
	{ 
		{"Theme Park", "Sky Diving"},
		{"Theme Park", "Guided Tours"},
		{"Guided Tours"},
        {"Theme Park", "Guided Tours" }
	};

    InitLocationPriceMap();
    InitActivityPriceMap();
    InitActivityLocationMap();
    InitLocationActivityMap();
}

void Location::InitFromJson()
{
    json j;
    j["Locations"]["location"] = { "Florida", "Paris", "Milan", "Tokyo" };
    j["Locations"]["price"] = { 1300.0f, 600.0f, 950.0f, 1800.0f };
    j["Locations"]["minimumTravellers"] = { 4, 2, 2, 4 };

    j["Activities"]["activity"] = { "Theme Park", "Guided Tours", "Sky Diving" };
    j["Activities"]["price"] = { 80.0f, 35.0f, 110.0f };
    j["Activities"]["AvalailableLocations"] =
    {
        {"Florida", "Paris" ,"Tokyo"},
        {"Paris", "Milan" ,"Tokyo"},
        {"Florida"}
    };

    std::ofstream Locations;
    Locations.open("Locations.json");
    Locations << j.dump();
    Locations.close();
}

void Location::InitLocationPriceMap()
{
    for (size_t i = 0; i < m_sLocation.size(); i++)
    {
        map_LocationPrice.insert(std::pair<std::string, float>(m_sLocation[i], m_fPrice[i]));
    }
}

void Location::InitActivityPriceMap()
{
    for (size_t i = 0; i < m_sActivity.size(); i++)
    {
        map_ActivityPrice.insert(std::pair<std::string, float>(m_sActivity[i], m_fCostPerPerson[i]));
    }
}

void Location::InitActivityLocationMap()
{
    for (size_t i = 0; i < m_sActivity.size(); i++)
    {
        map_ActivityLocation.insert(std::pair<std::string, std::vector<std::string>>(m_sActivity[i], m_sAvailableLocations[i]));
    }
}

void Location::InitLocationActivityMap()
{
    for (size_t i = 0; i < m_sLocation.size(); i++)
    {
        map_LocationActivity.insert(std::pair<std::string, std::vector<std::string>>(m_sLocation[i], m_sAvaliableActivities[i]));
    }
}