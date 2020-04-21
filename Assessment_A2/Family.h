#pragma once

#include <vector>
#include <string>
#include "User.h"
#include "Location.h"
#include "Helper.h"
#include <iostream>
#include <cstdio>

#include <memory>

class Family
{
private:
    /* Location class */
    std::string m_sLocation;
	/** We will add all the expenses of all users */
	float m_iTotalCostOfUsers = 0;
	std::string m_sFamilyName;
    /** A family will consists of a vector of users each with its own age, name, etc... */
    std::vector<std::shared_ptr<User>> m_clsUsers;

	int m_iAdultCounter = 0;
	int m_iChildCounter = 0;
	int m_iUndefinedCounter = 0;
protected:

public:
	Family(std::string);
	~Family();

	void SetFamilyLocation(std::string sLocation);
	std::string GetFamilyLocation();

	void SetFamilyTotalCost(float);
	float GetFamilyTotalCost();

	void SetFamilyName(std::string);
	std::string GetFamilyName();

	void SetFamilyUser(std::shared_ptr<User>);
	std::vector<std::shared_ptr<User>> GetFamilyUsers();
	void SaveToJSON(std::string sOldName = "");

	void AddMoreUsers();
	std::vector<std::shared_ptr<User>> GetUsers();

	void PrintListOfUsers();
    std::unique_ptr<Family> LoadFamily(std::string&);
    std::unique_ptr<Family> FamilyAlreadyExists(std::string&);
    bool CheckIfFamilyAlreadyExists(std::string&);

	// Validate party size and age
	void ValidateAge(int, int iOldAge = 0);

	void DeleteFamily();
	void EditUsers();
	void EditActivities(int);

	void RemoveFromJson(std::string&);
	static std::unique_ptr<Family> LoadFromJSON(std::string&);
	static std::unique_ptr<Family> CreateFamily(std::string&);
private:
};

