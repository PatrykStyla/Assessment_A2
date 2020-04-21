#include "Location.h"
#include "Helper.h"
#include <fstream>

using json = nlohmann::json;

Location::Location()
{
    // We will try to read the data from a json file that can be edited by an admin
    // This json simulates a database
    InitFromJson();
}

void Location::InitFromJson()
{
    // Try to read a json file
    std::ifstream i("Locations.json");
    
    if (i)
    {
        // file exists. Load the data from the file
        LoadFromJson();
    }
    else 
    {
        // The file was not present for some reason. 
        // Recreate it using the default values
        json j;
        j["Locations"]["location"]                = { "Florida", "Paris", "Milan", "Tokyo" };
        j["Locations"]["price"]                   = { 1300.0f, 600.0f, 950.0f, 1800.0f };
        j["Locations"]["minimumTravellers"]       = { 4, 2, 2, 4 };

        j["Activities"]["activity"]               = { "Theme Park", "Guided Tours", "Sky Diving" };
        j["Activities"]["price"]                  = { 80.0f, 35.0f, 110.0f };
        j["Activities"]["AvalailableLocations"]   =
        {
            {"Florida", "Paris" ,"Tokyo"},
            {"Paris", "Milan" ,"Tokyo"},
            {"Florida"}
        };
        j["Activities"]["m_sAvaliableActivities"] =
        {
            {"Theme Park", "Sky Diving"},
            {"Theme Park", "Guided Tours"},
            {"Guided Tours"},
            {"Theme Park", "Guided Tours" }
        };

        std::ofstream Locations;
        Locations.open("Locations.json");
        Locations << j.dump();
        Locations.close();
        // Load the data into memory
        LoadFromJson();
    }
}
// There is a guarantee that the file is present
void Location::LoadFromJson()
{
    json j;
    // The locations the company has to offer
    std::vector<std::string> sLocation;
    std::vector<float> fPrice;
    std::vector<int> iMinimumTravellers;

    std::vector<std::string> sActivity;
    std::vector<float> fCostPerPerson;

    std::ifstream JSON("Locations.json");
    // Load the json into the json class
    JSON >> j;


    // Load the data to memory 

    m_sAvailableLocations = j["Activities"]["AvalailableLocations"].get<std::vector<std::vector<std::string>>>();
    m_sAvaliableActivities = j["Activities"]["m_sAvaliableActivities"].get<std::vector<std::vector<std::string>>>();

    sActivity = j["Activities"]["activity"].get<std::vector<std::string>>();
    fCostPerPerson = j["Activities"]["price"].get<std::vector<float>>();

    for (size_t i = 0; i < sActivity.size(); i++)
    {
        std::unique_ptr<InvividualActivity> ptr = std::make_unique<InvividualActivity>( sActivity.at(i), fCostPerPerson.at(i) );
        Activities.push_back(std::move(ptr));
    }
    sLocation = j["Locations"]["location"].get<std::vector<std::string>>();
    fPrice = j["Locations"]["price"].get<std::vector<float>>();
    iMinimumTravellers = j["Locations"]["minimumTravellers"].get<std::vector<int>>();

    for (size_t i = 0; i < sLocation.size(); i++)
    {
        // Add location, price, minimum travelers to the object
        Locations.push_back({ sLocation.at(i), fPrice.at(i), iMinimumTravellers.at(i) });

        for (size_t ii = 0; ii < m_sAvaliableActivities.at(i).size(); ii++)
        {
            for (size_t iii = 0; iii < Activities.size(); iii++)
            {
                if (m_sAvaliableActivities.at(i).at(ii) == Activities.at(iii)->m_sActivity)
                {
                    Locations.at(i).Activities.push_back(Activities.at(iii).get());
                    break;
                }
            }

        }        
    }

    JSON.close();
}

void Location::SaveToJson()
{
    std::ifstream i("Locations.json");

    if (i)
    {
        json j;

        for (auto& a : Locations)
        {
            j["Locations"]["location"] += a.m_sLocation;
            j["Locations"]["price"] += a.m_fPrice;
            j["Locations"]["minimumTravellers"] += a.m_iMinimumTravellers;
        }

        for (auto& a : Activities)
        {
            j["Activities"]["activity"] += a->m_sActivity;
            j["Activities"]["price"] += a->m_fCostPerPerson;
        }

        j["Activities"]["AvalailableLocations"] = m_sAvailableLocations;
        j["Activities"]["m_sAvaliableActivities"] = m_sAvaliableActivities;

        std::ofstream Locations;
        Locations.open("Locations.json");
        Locations << j.dump();
        Locations.close();

        std::cout << "Changes saved successfully.\n\n";
    }
    else
    {
        std::cout << "The file could not be found.\n"
            << "You can either restore the file if you have accidentally deleted it, or relaunch the program with the default location settings.\n";
    }
}

void Location::EditLocation(int iIndex)
{
    int iChoice, iIntInput, iIndexActivity, iNewMinimumTravelers;
    bool bExit = true, bActivity = true;
    float fNewPrice;
    std::string sStringInput, NewActivity;

    do 
    {
        std::cout << "1) Edit the location name.\n"
            << "2) Edit the available activities.\n"
            << "3) Edit the price.\n"
            << "4) Edit minimum travelers.\n"
            << "5) Print details for the location.\n"
            << "0) Exit.\n";

        iChoice = ValidateNumberInput();
        switch (iChoice)
        {
        case 0:
            bExit = false;
            break;
        case 1: // Edit location name
            std::cout << "Please enter the new location name.\n";

            getline(std::cin, sStringInput);
            // Change all occurrences
            for (auto& activities : m_sAvailableLocations)
            {
                for (auto& activity : activities)
                {
                    if (activity == Locations.at(iIndex).m_sLocation)
                    {
                        activity = sStringInput;
                    }
                }
            }

            Locations.at(iIndex).m_sLocation = sStringInput;
            break;
        case 2: // Edit attractions
            std::cout << "\nIf you wish to edit the individual activities you have to go to the respective menu.\n\n";
            do
            {
                PrintAttractions(*this, iIndex);

                std::cout << "1) Add a new activity.\n"
                    << "2) Remove an activity.\n"
                    << "3) Print activities for current location.\n"
                    << "0) Exit.\n";
                    
                
                iIntInput = ValidateNumberInput();

                switch (iIntInput)
                {
                case 0: // Exit
                    bActivity = false;
                    break;
                case 1: // Add attraction
                    PrintAllAttractions(*this);

                    std::cout << "\nPlease select which attraction you wish to add in " << Locations.at(iIndex).m_sLocation << ".\n";
                    iIndexActivity = ValidateNumberInput();
                    iIndexActivity--;

                    AddActivityToLocation(iIndex, iIndexActivity);
                    break;
                case 2: // Remove attraction
                    std::cout << "\nPlease select which attraction you wish to remove in " << Locations.at(iIndex).m_sLocation << ".\n";
                    iIndexActivity = ValidateNumberInput();
                    iIndexActivity--;

                    RemoveActivityFromLocation(iIndex, iIndexActivity);
                    break;
                case 3: 
                    PrintAttractions(*this, iIndex);

                    break;
                default:
                    std::cout << "Please select a valid option.\n";
                    break;
                }
            } while (bActivity);
            break;
        case 3: // Price
            std::cout << "Please enter the new price";

            do 
            {
                fNewPrice = ValidateFloatInput();
                if (fNewPrice < 0)
                {
                    std::cout << "Please enter a positive value.\n";
                }
            } while (fNewPrice < 0);


            Locations.at(iIndex).m_fPrice = fNewPrice;
            break;
        case 4: // Minimum travelers
            std::cout << "Please enter the new amount";

            do
            {
                iNewMinimumTravelers = ValidateNumberInput();
                if (iNewMinimumTravelers < 1)
                {
                    std::cout << "Value must be greater than 0.\n";
                }
            } while (iNewMinimumTravelers < 1);

            Locations.at(iIndex).m_iMinimumTravellers = iNewMinimumTravelers;
            break;
        case 5:
            PrintDetailsForLocation(*this, iIndex);
            break;
        default:
            std::cout << "Please select a valid option.\n";
            break;
        }
    } while (bExit);
}

void Location::AddNewLocation(std::string& sNewLocationName)
{
    float fPricePerPerson;
    int iMinumTravellers, iIndexActivity;
    bool bAddActivities = true, bExit = true, bMoreActivities = true;
    std::string sChoice;

    std::cout << "Please enter the price per person.\n";

    do 
    {
        fPricePerPerson = ValidateFloatInput();
        if (fPricePerPerson < 0)
        {
            std::cout << "Please enter a positive number.\n";
        }
    } while (fPricePerPerson < 0);

    std::cout << "Please enter the minimum amount of travelers.\n";

    do 
    {
        iMinumTravellers = ValidateNumberInput();
        if (iMinumTravellers < 1)
        {
            std::cout << "There must be at least 1 minimum traveler.\n";
        }
    } while (iMinumTravellers < 1);

    SetLocation(sNewLocationName, fPricePerPerson, iMinumTravellers);

    std::cout << "Do you wish to add any activities? You can always add them later.\n";
    GetStringInput(sChoice);

    do 
    {
        RegexYesNo(bAddActivities, sChoice);

        if (bAddActivities)
        {
            do 
            {
                // add activities

                PrintAllAttractions(*this);

                std::cout << "\nPlease select which attraction you wish to add in " << Locations.back().m_sLocation << ".\n";
                iIndexActivity = ValidateNumberInput();
                iIndexActivity--;

                AddActivityToLocation((int)Locations.size() - 1, iIndexActivity);

                std::cout << "Do you want to add more activities?\n";
                GetStringInput(sChoice);
                RegexYesNo(bMoreActivities, sChoice);

            } while (bMoreActivities);
        }
        else
        {
            // Do nothing
            bExit = false;
        }
    } while (bExit);
}

void Location::RemoveLocation(int iIndex)
{
    Locations.erase(Locations.begin() + iIndex);
}

void Location::EditActivity(int iIndex)
{
    int iChoice;
    bool bExit = true;
    std::string sNewActivityName;
    float fNewActivityprice;
    do 
    {
        std::cout << "1) Edit the activity name.\n"
            << "2) Edit the activities available at each location.\n"
            << "3) Edit the price.\n"
            << "4) Print activities for the location.\n"
            << "0) Exit.\n";

        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0: // Exit
            bExit = false;
            break;
        case 1: // Edit activity name
            std::cout << "Please enter the new name.\n";
            GetStringInput(sNewActivityName);
            // Change all occurrences
            for (auto& activities : m_sAvaliableActivities)
            {
                for (auto& activity : activities)
                {
                    if (activity == Activities.at(iIndex)->m_sActivity)
                    {
                        activity = sNewActivityName;
                    }
                }
            }

            Activities.at(iIndex)->m_sActivity = sNewActivityName;
            break;
        case 2: // 
            break;
        case 3:// Edit activity price
            std::cout << "Please enter the new price.\n";
            do 
            {
                fNewActivityprice = ValidateFloatInput();
                if (fNewActivityprice < 0)
                {
                    std::cout << "Please enter a positive value.\n";
                }
            } while (fNewActivityprice < 0);

            Activities.at(iIndex)->m_fCostPerPerson = fNewActivityprice;
            break;
        case 4: // Print activities for the location
            PrintAttractions(*this, iIndex);
            break;
        default:
            std::cout << "Please choose a valid option.\n";
            break;
        }

    } while (bExit);

}

void Location::AddNewActivity(std::string& sNewActivityName)
{
    float fPrice;

    std::cout << "Please enter the price\n";
    do
    {
        fPrice = ValidateFloatInput();
        if (fPrice < 0)
        {
            std::cout << "Please enter a positive number.\n";
        }
    } while (fPrice < 0);

    Activities.push_back(std::make_unique<InvividualActivity>(sNewActivityName, fPrice));
    m_sAvailableLocations.push_back({});
}

void Location::RemoveActivity(int iIndex)
{
    // find the pointer to the activity that will be removed
    auto DeletedReference = Activities.at(iIndex).get();

    Activities.erase(Activities.begin() + iIndex);

    // loop over all locations and delete any reference with that pointer
    for (auto& loc : Locations)
    {
        for (size_t i = 0; i < loc.Activities.size(); i++)
        {
            if (DeletedReference == loc.Activities.at(i))
            { // If the deleted reference matches that on the object that means the array at this index can be erased
                loc.Activities.erase(loc.Activities.begin() + i);
            }
        }
    }

    std::cout << "Activity removed successfully.\n";
}

void Location::RemoveActivityFromLocation(int iIndexLocation, int iIndexActivity)
{
    // Deletes the nth element from the start of the vector
    // m_sAvaliableActivities.at(iIndexLocation).erase(m_sAvaliableActivities.at(iIndexLocation).begin() + iIndexActivity);
    if (iIndexActivity < Locations.at(iIndexLocation).Activities.size())
    {
        Locations.at(iIndexLocation).Activities.erase(Locations.at(iIndexLocation).Activities.begin() + iIndexActivity);
    }
    else
    {
        std::cout << "Please choose a valid option.\n";
    }
}

void Location::AddActivityToLocation(int iIndexLocation, int iIndexActivity)
{
    // Add an activity from the activities array at some index to the available activities for that location
    // m_sAvaliableActivities.at(iIndexLocation).push_back(m_sActivity.at(iIndexActivity));

    for (size_t i = 0; i < Locations.at(iIndexLocation).Activities.size(); i++)
    {
        if (Locations.at(iIndexLocation).Activities.at(i)->m_sActivity == Activities.at(iIndexActivity)->m_sActivity)
        {
            std::cout << "Activity is already present.\n";
            return;
        }
    }

    // Add the activity to the location if not a duplicate
    Locations.at(iIndexLocation).Activities.push_back(Activities.at(iIndexActivity).get());
    // Add the activity 
    if (iIndexLocation >= m_sAvaliableActivities.size())
    {
        m_sAvaliableActivities.push_back({});
    }

    m_sAvaliableActivities.at(iIndexLocation).push_back(Activities.at(iIndexActivity)->m_sActivity);
    m_sAvailableLocations.at(iIndexActivity).push_back(Locations.at(iIndexLocation).m_sLocation);
}

void Location::SetLocation(std::string sNewLocationName, float fPricePerPerson, int iMinumTravellers)
{
    Locations.push_back({ sNewLocationName, fPricePerPerson, iMinumTravellers });
}

std::vector<Location::IndividualLocation> Location::GetLocations()
{
    return Locations;
}

std::vector<std::unique_ptr<Location::InvividualActivity>>& Location::GetActivities()
{
    return Activities;
}

void Location::SetAvailableLocations(std::string sString)
{
}

void Location::SetAvailableLocations(std::string sString, int iIndex)
{
}

std::vector<std::vector<std::string>> Location::GetAvailableLocations()
{
    return m_sAvailableLocations;
}

void Location::SetAvailableActivities(std::string sString)
{
}

void Location::SetAvailableActivities(std::string sString, int iIndex)
{
}

std::vector<std::vector<std::string>> Location::GetAvailableActivities()
{
    return m_sAvaliableActivities;
}