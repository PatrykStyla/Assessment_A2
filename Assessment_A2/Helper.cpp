#include <iostream>

#include "Location.h"
#include "Helper.h"
#include <filesystem>

void PrintLocations(Location& locations)
{
    for (size_t i = 0; i <  locations.GetLocations().size(); i++)
    {
        std::cout << i + 1 << ") Location: " << locations.GetLocations().at(i).m_sLocation
            << ", Price: " << char(156) << locations.GetLocations().at(i).m_fPrice << "\n";
    }
    std::cout << "\n";
}

void PrintLocationsDetailed(Location& locations)
{
    for (size_t i = 0; i < locations.GetLocations().size(); i++)
    {
        std::cout << i + 1 << ") Location: " << locations.GetLocations().at(i).m_sLocation
            << ", Price: " << char(156) << locations.GetLocations().at(i).m_fPrice
            << ", Minimum Travelers: " << locations.GetLocations().at(i).m_iMinimumTravellers << ".\n";
    }
    std::cout << "\n";
}

void PrintDetailsForLocation(Location& locations, int iChoiceDestination)
{
    std::cout  << "Location: " << locations.GetLocations().at(iChoiceDestination).m_sLocation
        << ", Price: " << char(156) << locations.GetLocations().at(iChoiceDestination).m_fPrice
        << ", Minimum Travelers: " << locations.GetLocations().at(iChoiceDestination).m_iMinimumTravellers << ".\n";

    std::cout << "\n";
}

void PrintAttractions(Location& locations, int iChoiceDestination)
{
    std::cout << "List of attractions at " << locations.GetLocations().at(iChoiceDestination).m_sLocation << "\n\n";

    for (size_t i = 0; i < locations.GetLocations().at(iChoiceDestination).Activities.size(); i++)
    {
        std::cout << i + 1 << ") Attraction: " << locations.GetLocations().at(iChoiceDestination).Activities.at(i)->m_sActivity
            << ", Price per user: " << char(156) << locations.GetLocations().at(iChoiceDestination).Activities.at(i)->m_fCostPerPerson << "\n";
    }
    std::cout << "\n";
}

void PrintAllAttractions(Location& locations)
{
    for (size_t i = 0; i < locations.GetActivities().size(); i++)
    {
        std::cout << i + 1 << ") Attraction: " << locations.GetActivities().at(i)->m_sActivity
            << ", Price per user: " << char(156) << locations.GetActivities().at(i)->m_fCostPerPerson << "\n";
    }
    std::cout << "\n";
}

void PrintListFamilies()
{
    std::cout << "List of all Families.\n\n";
    // https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
    std::string path = "jsons/";
    std::string sToRemove[3] = { "\"", "jsons/", ".json" };
    int i = 0;
    for (auto& entry : std::filesystem::directory_iterator(path))
    {
        // loop 3 times to remove "", jsons/ & .json
        auto a = entry.path().string();
        for (size_t i = 0; i < 3; i++)
        {
            ReplaceStringInPlace(a, sToRemove[i], "");
        }

        std::cout << i + 1 << ") " << a << "\n";
        i++;
    }
    std::cout << "\n";
}

bool RegexYesNo(bool& bMore, std::string& sChoice)
{
    if (std::regex_match(sChoice, std::regex("[y](?:es)?", std::regex_constants::icase)))
    {
        bMore = true;
    }
    else if (std::regex_match(sChoice, std::regex("[n](?:o)?", std::regex_constants::icase)))
    {
        bMore = false;
    }
    else
    {
        std::cout << "Please enter Yes(y) or No(n)\n";
        return true;
    }

    return false;
}

int ValidateNumberInput()
{
    int iNumberToValidate;

    std::cout << "\n";
    std::cin >> iNumberToValidate;
    std::cout << "\n";

    // if cin fails (not a number)
    while (!std::cin)
    {
        // https://stackoverflow.com/questions/5655142/how-to-check-if-input-is-numeric-in-c
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // end
        std::cout << "Please enter a number\n";
        std::cin >> iNumberToValidate;
        std::cout << "\n";
    }
    std::cin.ignore();
    return iNumberToValidate;
}

float ValidateFloatInput()
{
    float fNumberToValidate;

    std::cout << "\n";
    std::cin >> fNumberToValidate;
    std::cout << "\n";
    // if cin fails (not a number)
    while (!std::cin)
    {
        // https://stackoverflow.com/questions/5655142/how-to-check-if-input-is-numeric-in-c
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // end
        std::cout << "Please enter a number\n";
        std::cin >> fNumberToValidate;
        std::cout << "\n";
    }
    std::cin.ignore();
    return fNumberToValidate;
}

void AnotherLocation(bool& bExit)
{
    int iChoice;
    bool bChoice = true; 
    do
    {
        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0:
            bExit = false;
            bChoice = false;
            break;
        case 1:
            bExit = true;
            bChoice = false;
            break;
        default:
            bChoice = true;
            std::cout << "Please select a valid option.\n";
            break;
        }
    } while (bChoice);
}

void GetStringInput(std::string& sString)
{
    getline(std::cin, sString);
}

void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

void PrintActivitiesForUser(Family* family, int iIndex)
{
    const auto& activities = family->GetUsers().at(iIndex)->GetActivities();
    for (size_t i = 0; i < family->GetUsers().at(iIndex)->GetActivities().size(); i++)
    {
        std::cout << i + 1 << ") Activity: " << activities.at(i).m_sActivity
            << " Price: " << char(156) << activities.at(i).m_fCostPerPerson << ".\n";
    }
}
