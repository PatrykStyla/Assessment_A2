#include "Family.h"
#include "json.hpp"
#include <fstream>
#include <memory>
#include "Helper.h"

Family::Family(std::string FamilyName)
{
    m_sFamilyName = FamilyName;
}

Family::~Family()
{
#if DebugPrint
    std::cout << "Family: " << m_sFamilyName << " destroyed\n";
#endif
}

void Family::SetFamilyLocation(std::string sLocation)
{
    m_sLocation = sLocation;
}

std::string Family::GetFamilyLocation()
{
    return m_sLocation;
}

void Family::SetFamilyTotalCost(float iTotalCost)
{
    m_iTotalCostOfUsers += iTotalCost;
}

float Family::GetFamilyTotalCost()
{
    return m_iTotalCostOfUsers;
}

void Family::SetFamilyName(std::string sFamilyName)
{
    m_sFamilyName = sFamilyName;
}

std::string Family::GetFamilyName()
{
    return m_sFamilyName;
}

void Family::SetFamilyUser(std::shared_ptr<User> objuser)
{
    m_clsUsers.push_back(objuser);
}

std::vector<std::shared_ptr<User>> Family::GetFamilyUsers()
{
    return m_clsUsers;
}
using json = nlohmann::json;


void Family::RemoveFromJson(std::string& sFamilyName)
{
    std::string sToDelte = "jsons/" + sFamilyName + ".json";
    std::remove(sToDelte.c_str());
}

std::unique_ptr<Family> Family::LoadFromJSON(std::string& sFamilyName)
{
    // Try to read a json file with name of the family
    std::ifstream i("jsons/" + sFamilyName + ".json");

    // File with this name exists
    if (i)
    {
        int iIndex = 0;
        json j;
        // Load the json into the json class
        i >> j;
        // Create the family object we will put the json data here
        std::unique_ptr<Family> family = std::make_unique<Family>(sFamilyName);
        family->SetFamilyLocation(j["Family"]["location"]);
        family->SetFamilyTotalCost(j["Family"]["totalCost"]);
        // Load the data from the json
        for (const auto& users : j["Family"]["user"])
        {
            std::shared_ptr<User> user = std::make_shared<User>(users["name"], users["age"]);
            user->SetTotalUserCost(users["totalCost"].get<float>());
            family->SetFamilyUser(user);
            family->ValidateAge(user->GetAge());

            if (users.contains("activities"))
            {
                for (const auto& activities : users["activities"])
                {
                    Location::InvividualActivity activity = { activities["name"].get<std::string>(), activities["cost"].get<float>() };
                    family->GetUsers().at(iIndex)->SetActivity(activity);
                }
            }
            iIndex++;
        }

        i.close();
        return family;
    }
    else
    {
        // No such file was found
        std::cout << "A family with the name " << sFamilyName << " does not exist.\n";
        i.close();
        return nullptr;
    }
}

void Family::SaveToJSON(std::string sOldName)
{
    // Create the object and add the c++ obj properties
    json j;
    int i = 0;

    j["Family"]["name"] = GetFamilyName();
    j["Family"]["totalCost"] = GetFamilyTotalCost();
    j["Family"]["location"] = GetFamilyLocation();


    for (auto& user : GetFamilyUsers())
    {
        // Add the properties to the family object
        j["Family"]["user"].emplace_back() = { 
            {"name", user->GetUserName()},
            {"age", user->GetAge()},
            {"totalCost", user->GetTotalUserCost()}
        };
        for (auto& activity : user->GetActivities())
        {
            j["Family"]["user"][i]["activities"] += { {"name",activity.m_sActivity}, {"cost", activity.m_fCostPerPerson} };
        }
        i++;
    }

    // Optional parameter is present
    if (sOldName != "")
    {
        // Remove old file if it was renamed
        RemoveFromJson(sOldName);
    }

    std::ofstream myfile("jsons/" + GetFamilyName() + ".json");
    myfile << j.dump();
    myfile.close();
}

std::unique_ptr<Family> Family::CreateFamily(std::string& sFamilyName)
{
    std::string sName, sChoice;
    int iAge, iChoice;
    bool bChoice = false, bExit = false;

    // Create family with the given name
    std::unique_ptr<Family> family = std::make_unique<Family>(sFamilyName);

    std::cout << "Please add a family member\n";

    // Create a user
    std::cout << "Enter the name \n";
    getline(std::cin, sName);

    std::cout << "Enter the age \n";
    iAge = ValidateNumberInput();
    family->ValidateAge(iAge);

    std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);
    // Add user to the family
    family->SetFamilyUser(user);

    // Check if user wants to add more users to the family
    family->AddMoreUsers();
    do
    {
        do
        {
            bExit = false;
            if (family->m_iAdultCounter == 0)
            {
                std::cout << "There must be at least 1 adult present(" << User::GetIsConsideredAdult() << "+) in order to book the trip.\n"
                    << "you can either.\n"
                    << "1) Add an adult user.\n"
                    << "2) Edit an existing user.\n";
                iChoice = ValidateNumberInput();

                switch (iChoice)
                {
                case 1:
                    family->AddMoreUsers();
                    break;
                case 2:
                    family->EditUsers();
                    break;
                default:
                    std::cout << "Please select a valid option.\n";
                    bExit = true;
                    break;
                }
            }
        } while (bExit);
    } while (family->m_iAdultCounter == 0);
    // Save the family with the family name provided
    family->SaveToJSON();

    return family;
}

int Family::GetAdultCounter()
{
    return m_iAdultCounter;
}

int Family::GetChildCounter()
{
    return m_iChildCounter;
}

int Family::GetUndefinedCounter()
{
    return m_iUndefinedCounter;
}

void Family::PrintListOfUsers()
{
    const auto& Users = GetFamilyUsers();
    for (size_t i = 0; i < Users.size(); i++)
    {
        std::cout << i + 1 << ") Name of user: " << Users.at(i)->GetUserName()
            << " Age: " << Users.at(i)->GetAge()
            << " Total Cost: " << char(156) << Users.at(i)->GetTotalUserCost() << ".\n";
        std::cout << "   Activities:\n";
        for (size_t ii = 0; ii < Users.at(i)->GetActivities().size(); ii++)
        {
            std::cout << "    " << ii + 1 << ") Activity: " << Users.at(i)->GetActivities().at(ii).m_sActivity << "\n";
        }
    }
    std::cout << "\n";
}

std::unique_ptr<Family> Family::LoadFamily(std::string& sFamilyName)
{
    auto family = Family::LoadFromJSON(sFamilyName);
    int iChoice;
    bool bMenu = true, bExists;

    // if we have the family return
    if (family)
    {
        // TODO print out the family and user for a final confirmation before proceeding to choose the location
        std::cout << "Do you want to load this family with the following details?\n";
        std::cout << "Location: " << family->GetFamilyLocation() << " Total Cost: " << char(156) << family->GetFamilyTotalCost() <<  "\n";
        family->PrintListOfUsers();

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
            GetStringInput(sFamilyName);
            family = LoadFamily(sFamilyName);
            break;
        case 2:
            // Create a new family
            std::cout << "Please enter the name of the family you want to create\n";
            GetStringInput(sFamilyName);
            bExists = CheckIfFamilyAlreadyExists(sFamilyName);
            if (bExists)
            {
                std::cout << "A family with that name already exists.\n";
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

std::unique_ptr<Family> Family::FamilyAlreadyExists(std::string& sFamilyName)
{
    int iChoice;
    bool bChoice = true, bExists;
    std::unique_ptr<Family> family;
    // Try to read a json file with name of the family
    std::ifstream i("jsons/" + sFamilyName + ".json");

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
            GetStringInput(sFamilyName);
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

bool Family::CheckIfFamilyAlreadyExists(std::string& sFamilyName)
{
    bool bChoice = true;
    // Try to read a json file with name of the family
    std::ifstream i("jsons/" + sFamilyName + ".json");

    // File with this name exists
    if (i) { return true; }
    else { return false; }
}

void Family::ValidateAge(int iAge, int iOldAge)
{
    if (iOldAge > 0)
    { // When updating the age get the old age and delete the corresponding counter
        if (iOldAge >= User::GetIsConsideredAdult())
        {
            m_iAdultCounter--;
        }
        else if (iOldAge <= User::GetIsConsideredChild())
        {
            m_iChildCounter--;
        }
        else
        {
            m_iUndefinedCounter--;
        }
    }
    if (iAge >= User::GetIsConsideredAdult())
    {
        m_iAdultCounter++;
    }
    else if (iAge <= User::GetIsConsideredChild())
    {
        m_iChildCounter++;
    }
    else
    {
        m_iUndefinedCounter++;
    }
}

void Family::DeleteFamily()
{
    RemoveFromJson(m_sFamilyName);
}

void Family::EditUsers()
{
    int iChoice, iNewAge, iEdit;
    bool bExit = true, bEdit = true;


    do
    { // bExit
        PrintListOfUsers();
        std::cout << "Choose an user to edit.\n";
        iChoice = ValidateNumberInput();
        iChoice--;
        do
        { // bEdit

            std::cout << "1) Change the name\n"
                << "2) Change the age.\n"
                << "3) Edit activities.\n"
                << "4) Print list of users.\n"
                << "5) Choose a different user.\n"
                << "0) Exit.\n";
            bEdit = true;
            iEdit = ValidateNumberInput();
            switch (iEdit)
            {
            case 0: // Exit
                bExit = false;
                bEdit = false;
                break;
            case 1: // Change age
                std::cout << "Choose the new name for the user " << GetUsers().at(iChoice)->GetUserName() << ".\n";
                iNewAge = ValidateNumberInput();
                ValidateAge(iNewAge, GetUsers().at(iChoice)->GetAge());

                GetUsers().at(iChoice)->SetAge(iNewAge);
                break;
            case 2: // Change name
                std::cout << "Choose the new age for the user " << GetUsers().at(iChoice)->GetUserName() << ".\n";
                iNewAge = ValidateNumberInput();
                ValidateAge(iNewAge, GetUsers().at(iChoice)->GetAge());

                GetUsers().at(iChoice)->SetAge(iNewAge);
                break;
            case 3: // Edit activities
                EditActivities(iChoice);
                break;
            case 4:
                PrintListOfUsers();
                break;
            case 5:
                bEdit = false;
                break;
            default:
                std::cout << "PLease select a valid option.\n";
                break;
            }
        } while (bEdit);
    } while (bExit);        
}

void Family::EditActivities(int iIndex)
{
    int iChoice, iIndexActivity;
    Location location;
    auto activities = GetUsers().at(iIndex)->GetActivities();
    for (size_t i = 0; i < activities.size(); i++)
    {
        std::cout << "Activities for user " << GetUsers().at(iIndex)->GetUserName();

        std::cout << i + 1 << ") " << activities.at(i).m_sActivity << ".\n";
    }
    std::cout << "\n";

    std::cout << "1) Add an activity.\n"
        << "2) Remove an activity.\n";

    iChoice = ValidateNumberInput();

    switch (iChoice)
    {
    case 1:
        PrintAllAttractions(location);
        iIndexActivity = ValidateNumberInput();
        iIndexActivity--;

        GetUsers().at(iIndex)->SetActivity(*location.GetActivities().at(iIndexActivity));
        break;
    case 2:
        PrintListOfUsers();
        // Index User
        iIndexActivity = ValidateNumberInput();
        iIndexActivity--;

        GetUsers().at(iIndex)->RemoveActivity(iIndexActivity);
        break;
    default:
        std::cout << "Please choose a valid option.\n";
        break;
    }
}

void Family::AddMoreUsers()
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
        GetStringInput(sChoice);

        // Match any capitalization of yes or y
        if (std::regex_match(sChoice, std::regex("[y](?:es)?", std::regex_constants::icase)))
        {
            std::cout << "Enter the name \n";
            GetStringInput(sName);

            std::cout << "Enter the age \n";
            iAge = ValidateNumberInput();

            std::shared_ptr<User> user = std::make_shared<User>(sName, iAge);
            ValidateAge(iAge);
            // Add user to the family
            SetFamilyUser(user);

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

std::vector<std::shared_ptr<User>> Family::GetUsers()
{
    return m_clsUsers;
}
