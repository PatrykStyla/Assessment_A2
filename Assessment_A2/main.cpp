#include <iostream>
#include "Location.h"
#include "User.h"
#include "Family.h"
#include "json.hpp"
#include "Helper.h"

#include <fstream>
#include <memory>
#include <regex>

#pragma region Save/Load
using json = nlohmann::json;
void SaveToJSON(Family& family)
{

    json j;

    // Create the object and add the c++ obj properties


    j["Family"]["name"] = family.GetFamilyName();
    j["Family"]["totalCost"] = family.GetFamilyTotalCost();
    j["Family"]["location"] = family.GetFamilyLocation();

    for (auto user : family.GetFamilyUsers())
    {
        // Add the properties to the family object
        j["Family"]["user"].emplace_back() = { {"name", user->GetUserName()}, {"age", user->GetAge()}, {"totalCost", 10000} };
    }
    // This will ad another object at the end

    std::ofstream myfile;
    myfile.open(family.GetFamilyName() + ".json");
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
        std::unique_ptr<Family> family = std::make_unique<Family>(sfamilyName);
        family->SetFamilyLocation(j["Family"]["location"]);
        // Load the data from the json
        for (auto users : j["Family"]["user"])
        {
            std::shared_ptr<User> user = std::make_shared<User>(users["name"], users["age"]);
            user->SetTotalUserCost(users["totalCost"]);
            family->SetFamilyUser(user);
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

#pragma endregion Save/Load
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
    int iChoiceDestination, iHowManyAttractions, iAttraction;
    bool bChoiceDestination = false, bStopAddingActivities = false, bAttraction = false;

    PrintLocations(locations);
    do
    {
        iChoiceDestination = ValidateNumberInput();

        // remove the offset
        iChoiceDestination = iChoiceDestination - 1;

        if (iChoiceDestination >= 0 && iChoiceDestination < locations.map_LocationPrice.size())
        {
            family->SetFamilyLocation(locations.m_sLocation[iChoiceDestination]);
            auto users = family->GetFamilyUsers();

            std::cout << "\nChoose an attraction for each user  " << locations.m_sLocation[iChoiceDestination] << "\n\n";
            PrintAttractions(locations, iChoiceDestination);

            std::cout << "Please add attractions for every user from the available list\n\n";
            for (auto user : users)
            {
                std::cout << "How many attractions do you wish to add for the user: " << user->GetUserName() << "?\n";
                iHowManyAttractions = ValidateNumberInput();
                std::cout << "You can stop at any time by pressing 0(zero)\n";
                for (size_t i = 0; i < iHowManyAttractions; i++)
                {
                    do
                    {
                        bAttraction = false;
                        std::cout << "Please enter the attraction " << i + 1 << "\n";
                        iAttraction = ValidateNumberInput();
                        iAttraction -= 1;
                        if (iAttraction >= 0 && iAttraction < locations.map_LocationActivity.at(locations.m_sLocation[iChoiceDestination]).size())
                        {
                            user->SetActivity(locations.map_LocationActivity.at(locations.m_sLocation[iChoiceDestination]).at(iAttraction));
                        }
                        else
                        {
                            // do while bool
                            bAttraction = true;
                        }
                    } while (bAttraction);
                }
            }
        }
        else { bChoiceDestination = true; }
    } while (iChoiceDestination >= 0 || bChoiceDestination);
}

void ActivitiesInEachLocation(Location& locations)
{
    int iChoice;
    bool bChoice = false, bLocation = false, bOtherLocation = false;
    std::string sChoice;

    do
    {
        if (!bOtherLocation)
        {
            std::cout << "Select the desired activity to see the available locations\n";
        }
        do
        {
            for (int i = 0; i < locations.m_sActivity.size(); i++)
            {
                std::cout << i + 1 << ") Activity " << locations.m_sActivity[i] << ", Price per person: " << locations.m_fCostPerPerson[i] << char(156) << "\n";
            }
            std::cout << "\n";
            do
            {
                iChoice = ValidateNumberInput();
                // Remove offset
                iChoice = iChoice - 1;
                if (iChoice >= 0 && iChoice < locations.m_sActivity.size())
                {
                    for (int i = 0; i < locations.m_sAvailableLocations[iChoice].size(); i++)
                    {
                        std::cout << "Location: " << locations.m_sAvailableLocations[iChoice][i]
                            << ", Price: " << locations.map_LocationPrice[locations.m_sAvailableLocations[iChoice][i]] << char(156) << "\n";
                    }
                }
                else
                {
                    std::cout << "Please choose one of the available locations\n";
                    bLocation = true;
                }
            } while (bLocation);


            std::cout << "\nDo you want to check other locations?\n";

            std::cin >> sChoice;
            std::cout << "\n";

            if (std::regex_match(sChoice, std::regex("[y](?:es)?", std::regex_constants::icase)))
            {
                bChoice = true;
                bOtherLocation = true;
            }
            else if (std::regex_match(sChoice, std::regex("[n](?:o)?", std::regex_constants::icase)))
            {
                // exit the loops
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
        bInputError = false;
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
            family.SetFamilyUser(user);

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

bool CheckIfFamilyAlreadyExists(std::string sFamilyName)
{
    bool bChoice = true;
    // Try to read a json file with name of the family
    std::ifstream i(sFamilyName + ".json");

    // File with this name exists
    if (i) { return true; }
    else { return false; }
}

std::unique_ptr<Family> CreateFamily(std::string sFamilyName)
{
    std::string sName, sChoice;
    int iAge;
    bool bChoice = false;

    // Create family with the given name
    std::unique_ptr<Family> family = std::make_unique<Family>(sFamilyName);

    std::cout << "Please add a family member\n";

    // Create a user
    std::cout << "Enter the name \n";
    std::cin >> sName;

    std::cout << "Enter the age \n";
    iAge = ValidateNumberInput();

    std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);
    // Add user to the family
    family->SetFamilyUser(user);

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
    bool bMenu = true, bExists;

    // if we have the family return
    if (family)
    {
        // TODO print out the family and user for a final confirmation before proceeding to choose the location
        std::cout << "Do you want to load this family with the following details?\n";
        std::cout << "Location: " << family->GetFamilyLocation() << "\n";
        for (auto user : family->GetFamilyUsers())
        {
            std::cout << "Name: " << user->GetUserName()
                << " Age: " << user->GetAge()
                << " Cost: " << user->GetTotalUserCost() << "\n";
        }

        do
        {
            std::cout << "1) Choose that family\n";
            std::cout << "2) Return to menu\n";

            iChoice = ValidateNumberInput();

            if (iChoice > 0 && iChoice < 3) { bMenu = false; }
        } while (bMenu);

        switch (iChoice)
        {
        case 1:
            // continue
            return family;
            break;
        case 2:
            return nullptr;
            break;
        default:
            // TODO delete
            std::cout << "Startup menu out of bounds\n";
            break;
        }
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
            // Load locally stored family
            std::cout << "Please enter the name of the family you want to load\n";
            std::cin >> sFamilyName;
            family = LoadFamily(sFamilyName);
            break;
        case 2:
            // Create a new family
            std::cout << "Please enter the name of the family you want to create\n";
            std::cin >> sFamilyName;
            bExists = CheckIfFamilyAlreadyExists(sFamilyName);
            if (bExists)
            {
                std::cout << "A family with that name already exists";
            }
            else
            {
                family = CreateFamily(sFamilyName);
            }
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

std::unique_ptr<Family> FamilyAlreadyExists(std::string sFamilyName)
{
    int iChoice;
    bool bChoice = true, bExists;
    std::unique_ptr<Family> family;
    // Try to read a json file with name of the family
    std::ifstream i(sFamilyName + ".json");

    // File with this name exists
    if (i)
    {
        do
        {
            std::cout << "A family with the name " << sFamilyName << " Already exists.\n";
            std::cout << "Do you want to \n";
            std::cout << "1) Use a different name?\n";
            std::cout << "2) Override the old family?\n";
            std::cout << "3) Load the family?\n";

            iChoice = ValidateNumberInput();
            if (iChoice > 0 && iChoice < 4) { bChoice = false; }
        } while (bChoice);

        switch (iChoice)
        {
        case 1:
            std::cout << "Please enter the new name\n";
            std::cin >> sFamilyName;
            bExists = CheckIfFamilyAlreadyExists(sFamilyName);
            if (bExists)
            {
                // name exists
                family = FamilyAlreadyExists(sFamilyName);
            }
            else
            {
                family = CreateFamily(sFamilyName);
            }
            break;
        case 2:
            // continue the program
            family = CreateFamily(sFamilyName);
            break;
        case 3:
            family = LoadFamily(sFamilyName);
            break;
        default:
            // TODO delete
            std::cout << "Startup menu out of bounds\n";
            break;
        }
    }
    else
    {
        // continue the program
    }

    return family;
}

void Login()
{
    // TODO admin login
}

std::unique_ptr<Family> StartupMenu(bool bExit)
{
    bool bMenu = true, bExists;
    int iChoice = 0;
    std::string sFamilyName;
    std::unique_ptr<Family> family;

    bExit = true;

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
        // Load locally stored family
        std::cout << "Please enter the name of the family you want to load\n";
        std::cin >> sFamilyName;
        family = LoadFamily(sFamilyName);
        break;
    case 2:
        // Create a new family
        std::cout << "Please enter the name of the family you want to create\n";
        std::cin >> sFamilyName;
        bExists = CheckIfFamilyAlreadyExists(sFamilyName);
        if (bExists)
        {
            // family already exists
            family = FamilyAlreadyExists(sFamilyName);
        }
        else
        {
            family = CreateFamily(sFamilyName);
        }
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
    std::cout << "Welcome to the holiday program \n";

    // A user will either load an existing family or create a new one
    // OR an admin can log in to the system
    bool* bExit = new bool(false);
    auto family = StartupMenu(bExit);

    do
    {
        // Check if family exists
        // This wont happen when an admin tries to log in or the user decides to exit
        if (family)
        {
            Location locations;
            int iChoiceSelect;
            bool bChoiceSelect = true;

            std::cout << "select\n\n";
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
            // User decides to go to startup menu again
            auto family = StartupMenu(bExit);
        }
    } while (!family || *bExit == true);
}