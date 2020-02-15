#include <iostream>

#include "Location.h"
#include "Helper.h"

void PrintLocations(Location& locations)
{
    int i = 0;
    for (auto location : locations.map_LocationPrice)
    {
        std::cout << i + 1 << ") Location: " << location.first 
                  << ", Price: " << location.second << char(156) << "\n";
        i++;
    }

    //for (int i = 0; i < locations.m_sLocation.size(); i++)
    //{
    //    std::cout << i + 1 << ": Location: " << locations.m_sLocation[i] << ", Price per person: " << locations.m_fPrice[i] << char(156) << "\n";
    //}
}

void PrintAttractions(Location& locations, int iChoiceDestination)
{
    std::cout << "List of attractions at " << locations.m_sLocation[iChoiceDestination] << "\n";
    int i = 0;
    for (auto attraction : locations.map_LocationActivity.at(locations.m_sLocation[iChoiceDestination]))
    {
        std::cout << i + 1 << ")    Attraction: " << attraction
                  << ", Price per user: " << locations.map_ActivityPrice.at(attraction)
                  << char(156) << "\n";
        i++;
    }
}
