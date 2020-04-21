#pragma once

#include <vector>
#include <string>
#include <map>
#include "json.hpp"
#include <iostream>
#include <regex>
#include <memory>

#include "Parser.h"

class Location
{
public:
	Location();

    struct InvividualActivity;
    struct IndividualLocation;

    void InitFromJson();
    void LoadFromJson();
    void SaveToJson();
	// Edit a location at some index
	void EditLocation(int);
	// Add a new location
	void AddNewLocation(std::string&);
	// Remove a location at some index
	void RemoveLocation(int);

    void EditActivity(int);
    void AddNewActivity(std::string&);
    void RemoveActivity(int);
    // First index is for the location, second for the activity
    void RemoveActivityFromLocation(int, int);
    // First index is for the location, second for the activity
    void AddActivityToLocation(int, int);

    // There are 2 Sets. 1 for adding a new thing and the other to edit it at a specified index

    void SetLocation(std::string, float, int);
    std::vector<IndividualLocation> GetLocations();

    std::vector<std::unique_ptr<InvividualActivity>>& GetActivities();

    void SetAvailableLocations(std::string);
    void SetAvailableLocations(std::string, int);
    std::vector<std::vector<std::string>> GetAvailableLocations();

    void SetAvailableActivities(std::string);
    void SetAvailableActivities(std::string, int);
    std::vector<std::vector<std::string>> GetAvailableActivities();
private:
    std::vector<IndividualLocation> Locations;
    std::vector<std::unique_ptr<InvividualActivity>> Activities;


    /* This lists the available locations based on the activity */
    std::vector<std::vector<std::string>> m_sAvailableLocations;
    /* This lists the available activities based on the location */
    std::vector<std::vector<std::string>> m_sAvaliableActivities;
};

struct Location::InvividualActivity
{
    std::string m_sActivity;
    float m_fCostPerPerson;

    InvividualActivity(std::string m_sActivity, float m_fCostPerPerson)
        :m_sActivity(m_sActivity), m_fCostPerPerson(m_fCostPerPerson) {}
};

struct Location::IndividualLocation
{
    std::string m_sLocation;
    float m_fPrice;
    int m_iMinimumTravellers;

    std::vector<InvividualActivity*> Activities;
};