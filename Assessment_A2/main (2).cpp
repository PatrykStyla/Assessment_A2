#include <iostream>
#include "Location.h"
#include "User.h"
#include "Family.h"
#include "json.hpp"

#include <fstream>
#include <memory>
#include <regex>

using json = nlohmann::json;

void SaveToJSON()
{

    json j;

    j["Family"]["name"] = "family name";
	j["Family"]["user"] = { // Those will be [] in the .json
		// Properties of the objects
		{ 
			{"name", "username"}, {"age", 1}, {"totalCost", 10000}
		}, 
		// Properties of the objects
		{ 
			{"name", "username2"}, {"age2", 2}, {"totalCost2", 20000}
		} 
	}; // Those will be [] in the .json

	// This will ad another object at the end
	j["Family"]["user"].emplace_back() = { { "name", "username3" }, { "age3", 3 }, { "totalCost3", 30000 } };

    std::ofstream myfile;
    myfile.open("json.json");
    myfile << j.dump();
    myfile.close();
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

			if (std::regex_match(sChoice, std::regex("[y](?:es)?", std::regex_constants::icase)))
			{
				bOtherLocation = false;
				bChoice = true;
			}
			else if (std::regex_match(sChoice, std::regex("[n](?:o)?", std::regex_constants::icase)))
			{
				bOtherLocation = false;
				bChoice = false;
			}
			else
			{
				std::cout << "Please enter Yes(y) or No(n)\n";
				bOtherLocation = true;
			}
		} while (bOtherLocation);

	} while (bChoice);
}

void AddMoreUsers(Family *family)
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
        if (std::regex_match(sChoice, std::regex("[y](?:es)?", std::regex_constants::icase)))
        {
            std::cout << "Enter the name \n";
            std::cin >> sName;

            std::cout << "Enter the age \n";
            iAge = ValidateNumberInput();

			User user(sName, iAge);

			user.some_string = &sName;

            // Add user to the family
            family->m_clsUsers.push_back(user);
			bInputError = false;
        }
        // Match any capitalization of no or n
        else if (std::regex_match(sChoice, std::regex("[n](?:o)?", std::regex_constants::icase)))
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
	SaveToJSON();

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

	User user(sName, iAge);
    // Add user to the family
    family.m_clsUsers.push_back(user);
	
	// Check if user wants to add more users to the family
	AddMoreUsers(&family);

	// Select the destination
	std::cout << "Please select the appropriate choice\n\n";
	bool bChoiceSelectError = false;
	// TODO Error 
	do
	{
		if (!bChoiceSelectError)
		{
			std::cout << "1: " << "Select your desired destination\n";
			std::cout << "2: " << "See what activities are available in each location\n";

			std::cout << "0: " << "Exit\n";

			iChoiceSelect = ValidateNumberInput();
		}

		switch (iChoiceSelect)
		{
		case 0:
			bChoiceSelect = false;
			break;
		case 1:
			SelectLocation(locations, family);
			break;
		case 2:
			ActivitiesInEachLocation(locations);
			break;
		default:
			std::cout << "Please enter the correct option\n";
			iChoiceSelect = ValidateNumberInput();
			bChoiceSelectError = true;
			break;
		}
	} while (bChoiceSelect);
 }