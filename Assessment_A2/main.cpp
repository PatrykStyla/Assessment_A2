#include <iostream>
#include "Location.h"
#include "User.h"
#include "Family.h"
#include "json.hpp"

#include <fstream>
#include <memory>
#include <regex>

using json = nlohmann::json;

void SaveToJSON(Family &family)
{

    json j;

	// Create the object and add the c++ obj properties


    j["Family"]["name"] = family.m_sFamilyName;
	j["Family"]["totalCost"] = family.m_iTotalCostOfUsers;


	for (auto user : family.m_clsUsers)
	{
		// Add the properties to the family object
		j["Family"]["user"].emplace_back() = { {"name", user->m_sName}, {"age", user->m_iYearsOld}, {"totalCost", 10000} };
	}
	// This will ad another object at the end



    std::ofstream myfile;
    myfile.open("json.json");
    myfile << j.dump();
    myfile.close();
}

void LoadFromJSON(std::string sfamilyName)
{
	// Read a json file with name of the family
	std::ifstream i(sfamilyName);

	if (i)
	{
        json j;
        // Load the json into the json class
        i >> j;
		// Create the family object we will put the json data here
		Family family(sfamilyName);

		for (auto users : j["Family"]["user"])
		{
			std::cout << users;
			std::shared_ptr<User> user = std::make_shared<User>(users["name"], users["age"]);
			user->m_iTotalCost = users["totalCost"];
			family.m_clsUsers.push_back(user);
		}
	}
	else
	{
		// No such file was found
	}



	// Load the data to the c++ obj
}

/* Will handle the input of a number, validate it and return the value when its correct*/
int ValidateNumberInput()
{
	int iNumberToValidate;

	std::cin >> iNumberToValidate;

	// if cin fails (not a number)
	while (!std::cin)
	{
		// https://stackoverflow.com/questions/5655142/how-to-check-if-input-is-numeric-in-c
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		// end
		std::cout << "Please enter a number\n";
		std::cin >> iNumberToValidate;
	}

	return iNumberToValidate;
}


void SelectLocation(Location& locations, Family& family)
{
    int iChoiceDestination;

    for (int i = 0; i < locations.m_sLocation.size(); i++)
    {
        std::cout << i + 1 << ": " << locations.m_sLocation[i] << "\n";
    }
    do
    {
		iChoiceDestination = ValidateNumberInput();
		
        // remove the offset
        iChoiceDestination = iChoiceDestination - 1;

		family.SetUserLocation(locations.m_sLocation[iChoiceDestination]);

        if (iChoiceDestination >= 0)
        {
            std::cout << "\nattractions available at " << locations.m_sLocation[iChoiceDestination] << "\n\n";
            for (int i = 0; i < locations.m_sAvaliableActivities[iChoiceDestination].size(); i++)
            {
                std::cout << i + 1 << ": " << locations.m_sAvaliableActivities[iChoiceDestination][i] << "\n";
            }
        }
    } while (iChoiceDestination >= 0);
}

void ActivitiesInEachLocation(Location& locations)
{
	int iChoice;
	bool bChoice = true, bOtherLocation = true;
	std::string sChoice;

    do
    {
		if (bOtherLocation)
		{
			std::cout << "Select the desired activity to see the available locations\n";
		}

        for (int i = 0; i < locations.m_sActivity.size(); i++)
        {
            std::cout << i + 1 << ": " << locations.m_sActivity[i] << "\n";
        }

		iChoice = ValidateNumberInput();
		std::cout << "\n";
		// Remove offset
		iChoice = iChoice - 1;

		for (int i = 0; i < locations.m_sAvailableLocations[iChoice].size(); i++)
		{
			std::cout << i + 1 << ": " << locations.m_sAvailableLocations[iChoice][i] << "\n";
		}

		std::cout << "\nDo you want to check other locations?\n";

		do
		{
			std::cin >> sChoice;
			std::cout << "\n";

			if (std::regex_match(sChoice, std::regex("[1y](?:es)?", std::regex_constants::icase)))
			{
				bOtherLocation = false;
				bChoice = true;
			}
			else if (std::regex_match(sChoice, std::regex("[1n](?:o)?", std::regex_constants::icase)))
			{
				bChoice = false;
				bOtherLocation = false;
			}
			else
			{
				std::cout << "Please enter Yes(y) or No(n)\n";
				bOtherLocation = true;
			}
		} while (bOtherLocation);

	} while (bChoice);
}

void AddMoreUsers(Family &family)
{
	bool bAdditionalMembers = true, bInputError = false;
	std::string sName, sFamilyName, sChoice;
	int iAge;
    do
    {
        if (!bInputError)
        {
            // Will only show if we have not made an error when inputting wrong value
            std::cout << "Do you want to add additional members?\n";
        }

        std::cin >> sChoice;

        // Match any capitalization of yes or y
        if (std::regex_match(sChoice, std::regex("[1y](?:es)?", std::regex_constants::icase)))
        {
            std::cout << "Enter the name \n";
            std::cin >> sName;

            std::cout << "Enter the age \n";
            iAge = ValidateNumberInput();

            std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);

            // Add user to the family
            family.m_clsUsers.push_back(user);

        }
        // Match any capitalization of no or n
        else if (std::regex_match(sChoice, std::regex("[1n](?:o)?", std::regex_constants::icase)))
        {
            // Exit loop
            bAdditionalMembers = false;
        }
        else
        {
            std::cout << "Please enter Yes(y) or No(n)\n";
            bInputError = true;
        }

    } while (bAdditionalMembers);
}

int main()
{
	LoadFromJSON("json.json");

	Location locations;
	std::string sName, sFamilyName, sChoice;
	int iAge, iChoiceSelect;
	bool bChoiceSelect = true;


	std::cout << "Welcome to the holiday program \n";
	std::cout << "Please add add all the individuals that will be participating in the trip \n";

	// Create a family
	std::cout << "Please enter the name for the family\n";
	std::cin >> sFamilyName;

	Family family(sFamilyName);

	std::cout << "Please add a family member\n";

	// Create a user
	std::cout << "Enter the name \n";
	std::cin >> sName;

	std::cout << "Enter the age \n";
	iAge = ValidateNumberInput();

	std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);
    // Add user to the family
    family.m_clsUsers.push_back(user);
	
	// Check if user wants to add more users to the family
	AddMoreUsers(family);

	SaveToJSON(family);
	// Select the destination

	std::cout << "select\n\n";
	// TODO Error 
	do
	{
        std::cout << "1: " << "Select your desired destination\n";
        std::cout << "2: " << "See what activities are available in each location\n";

		iChoiceSelect = ValidateNumberInput();

		switch (iChoiceSelect)
		{
		case 1:
			SelectLocation(locations, family);
			break;
		case 2:
			ActivitiesInEachLocation(locations);
			break;
		default:
			std::cout << "Please enter the correct option\n";
			iChoiceSelect = ValidateNumberInput();

			bChoiceSelect = false;
			break;
		}
	} while (bChoiceSelect);
}