#include "Location.h"
#include <fstream>

using json = nlohmann::json;

Location::Location()
{
    // We will try to read the data from a json file that can be edited by an admin
    // This json simulates a database
    InitFromJson();

    InitLocationPriceMap();
    InitActivityPriceMap();
    InitActivityLocationMap();
    InitLocationActivityMap();
}

void Location::InitFromJson()
{
    // Try to read a json file
    std::ifstream i("Locations.json");
    
    if (i)
    {
        // file exists. Load the data from the file
        LoadFromJson();
    }
    else 
    {
        // The file was not pressent for some reason. 
        // Recreate it using the default values
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
        j["Activities"]["m_sAvaliableActivities"] =
        {
            {"Theme Park", "Sky Diving"},
            {"Theme Park", "Guided Tours"},
            {"Guided Tours"},
            {"Theme Park", "Guided Tours" }
        };

        std::ofstream Locations;
        Locations.open("Locations.json");
        Locations << j.dump();
        Locations.close();
        // Load the data into memory
        LoadFromJson();
    }
}

void Location::LoadFromJson()
{
    json j;

    std::ifstream Locations("Locations.json");
    // Load the json into the json class
    Locations >> j;

    // Load the data to memory
    m_sLocation = j["Locations"]["location"].get<std::vector<std::string>>();
    m_fPrice = j["Locations"]["price"].get<std::vector<float>>();
    m_fMinimumTravellers = j["Locations"]["minimumTravellers"].get<std::vector<int>>();

    m_sActivity = j["Activities"]["activity"].get<std::vector<std::string>>();
    m_fCostPerPerson = j["Activities"]["price"].get<std::vector<float>>();
    m_sAvailableLocations = j["Activities"]["AvalailableLocations"].get<std::vector<std::vector<std::string>>>();
    m_sAvaliableActivities = j["Activities"]["m_sAvaliableActivities"].get<std::vector<std::vector<std::string>>>();

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