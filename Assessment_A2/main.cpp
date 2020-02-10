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
	myfile.open(family.m_sFamilyName + ".json");
    myfile << j.dump();
    myfile.close();
}

std::unique_ptr<Family> LoadFromJSON(std::string sfamilyName)
{
	// Try to read a json file with name of the family
	std::ifstream i(sfamilyName + ".json");

	// File with this name exists
	if (i)
	{
        json j;
        // Load the json into the json class
        i >> j;
		// Create the family object we will put the json data here
		// Family family(sfamilyName);
		std::unique_ptr<Family> family = std::make_unique<Family>(sfamilyName);

		for (auto users : j["Family"]["user"])
		{
			std::cout << users << "\n";
			std::shared_ptr<User> user = std::make_shared<User>(users["name"], users["age"]);
			user->m_iTotalCost = users["totalCost"];
			family->m_clsUsers.push_back(user);
		}

		return family;
	}
	else
	{
		// No such file was found
		std::cout << "A family with the name " << sfamilyName << " does not exist.\n";

		return nullptr;
	}
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


void SelectLocation(Location& locations, std::unique_ptr<Family>& family)
{
    int iChoiceDestination;
	bool bChoiceDestination = false;

    for (int i = 0; i < locations.m_sLocation.size(); i++)
    {
        std::cout << i + 1 << ": " << locations.m_sLocation[i] << "\n";
    }
    do
    {
		iChoiceDestination = ValidateNumberInput();
		
        // remove the offset
        iChoiceDestination = iChoiceDestination - 1;

        if (iChoiceDestination >= 0 && iChoiceDestination < locations.m_sLocation.size())
        {
			family->SetUserLocation(locations.m_sLocation[iChoiceDestination]);

            std::cout << "\nattractions available at " << locations.m_sLocation[iChoiceDestination] << "\n\n";
            for (int i = 0; i < locations.m_sAvaliableActivities[iChoiceDestination].size(); i++)
            {
                std::cout << i + 1 << ": " << locations.m_sAvaliableActivities[iChoiceDestination][i] << "\n";
            }
        }
		else { bChoiceDestination = true; }
    } while (iChoiceDestination >= 0 || bChoiceDestination);
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


void AddMoreUsers(Family& family)
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

			std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);

			// Add user to the family
			family.m_clsUsers.push_back(user);

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


std::unique_ptr<Family> CreateFamily()
{
	std::string sName, sFamilyName;
	int iAge;

	std::cout << "Welcome to the holiday program \n";
	std::cout << "Please add add all the individuals that will be participating in the trip \n";

	// Create a family
	std::cout << "Please enter the name for the family\n";
	std::cin >> sFamilyName;

	std::unique_ptr<Family> family = std::make_unique<Family>(sFamilyName);

	std::cout << "Please add a family member\n";

	// Create a user
	std::cout << "Enter the name \n";
	std::cin >> sName;

	std::cout << "Enter the age \n";
	iAge = ValidateNumberInput();

	std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);
	// Add user to the family
	family->m_clsUsers.push_back(user);

	// Check if user wants to add more users to the family
	AddMoreUsers(*family);
	// Save the family with the family name provided
	SaveToJSON(*family);


	return family;
}


std::unique_ptr<Family> LoadFamily(std::string sFamilyName)
{
	auto family = LoadFromJSON(sFamilyName);
	int iChoice;
	bool bMenu = true;

	// if we have the family return
	if (family)
	{
		// TODO print out the family and user for a final confirmation before proceeding to choose the location
	}
	// Ask the user if he wants to create a new family/ choose a new family to load or exit
	else
	{
		do
		{
			std::cout << "Please select the appropriate option.\n";

			std::cout << "1) Choose a new family to load.\n";
			std::cout << "2) Create a new family.\n";
			std::cout << "3) Exit the program.\n";

			std::cout << "\n";

			iChoice = ValidateNumberInput();
			if (iChoice > 0 && iChoice < 4) { bMenu = false; }
		} while (bMenu);
		
		switch (iChoice)
		{
		case 1:
			// Load localy stored family
			std::cout << "Please enter the name of the family you want to load\n";
			std::cin >> sFamilyName;
			family = LoadFamily(sFamilyName);
			break;
		case 2:
			// Create a new family
			family = CreateFamily();
			break;
		case 3:
			// Family will be a null pointer and the program will end
			break;
		default:
			// TODO delete
			std::cout << "Startup menu out of bounds\n";
			break;
		}
	}

	return family;
}

void Login()
{
	// TODO admin login
}

std::unique_ptr<Family> StartupMenu()
{
	bool bMenu = true;
	int iChoice = 0;
	std::string sFamilyName;
	std::unique_ptr<Family> family;

	do
	{
		std::cout << "Welcome to the program. Select the appropriate option.\n";

		std::cout << "1) Load a family\n";
		std::cout << "2) Create a new family\n";
		std::cout << "3) Login\n";

		// Validate and check if the number matches the options
		iChoice = ValidateNumberInput();
		if (iChoice > 0 && iChoice < 4) { bMenu = false; }

	} while (bMenu);

	switch (iChoice)
	{
	case 1:
		// Load localy stored family
		std::cout << "Please enter the name of the family you want to load\n";
		std::cin >> sFamilyName;
		family = LoadFamily(sFamilyName);
		break;
	case 2:
		// Create a new family
		family = CreateFamily();
		break;
	case 3:
		// Admin login
		Login();
		break;
	default:
		// TODO delete
		std::cout << "Startup menu out of bounds\n";
		break;
	}

	return family;
}

int main()
{
	// A user will either load an existsing family or create a new one
	// OR an admin can log in to the system
	auto family = StartupMenu();
	
	// Check if family exists
	// This wont happen when an admin tries to log in or the user decides to exit
	if (family)
	{
		Location locations;
		int iChoiceSelect;
		bool bChoiceSelect = true;

		std::cout << "select\n\n";
		// TODO Error 
		do
		{
			std::cout << "1: " << "Select your desired destination\n";
			std::cout << "2: " << "See what activities are available in each location\n";

			iChoiceSelect = ValidateNumberInput();

			switch (iChoiceSelect)
			{
			case 0:
				// Exit
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

				bChoiceSelect = false;
				break;
			}
		} while (bChoiceSelect);
	}
	else
	{
		// Programm ends here if admin login option was selected and finished.
		std::cout << "Null \n";
	}


}