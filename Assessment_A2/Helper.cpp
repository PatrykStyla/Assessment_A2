#include <iostream>

#include "Location.h"
#include "Helper.h"

void PrintLocations(Location& locations)
{
    for (size_t i = 0; i < locations.m_sLocation.size(); i++)
    {
        std::cout << i + 1 << ") Location: " << locations.m_sLocation[i]
                           << ", Price: " << locations.m_fPrice[i] << char(156) << "\n";
    }
}

void PrintAttractions(Location& locations, int iChoiceDestination)
{
    std::cout << "List of attractions at " << locations.m_sLocation[iChoiceDestination] << "\n";

    for (size_t i = 0; i < locations.m_sAvaliableActivities[iChoiceDestination].size(); i++)
    {
        std::cout << i + 1 << ") Attraction: " << locations.m_sAvaliableActivities[iChoiceDestination][i]
                           << ", Price per user: " << locations.map_ActivityPrice.at(locations.m_sAvaliableActivities[iChoiceDestination][i])
                           << char(156) << "\n";
    }
}
