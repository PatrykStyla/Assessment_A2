#include "Admin.h"
#include "main.h"

using json = nlohmann::json;

Admin::Admin(Location& locations)
{
    // Load the location class
    m_locations = &locations;
    // Try to load admin credentials if user has logged in at least once
    LoadFromJSON();
}
float Admin::m_fVAT;
float Admin::m_fDiscount;

void Admin::SaveToJSON()
{
    json j;

    j["Admin"]["sUserName"] = m_sUsername;
    j["Admin"]["sPassword"] = m_sPassword;

    j["Admin"]["VAT"] = m_fVAT;
    j["Admin"]["Discount"] = m_fDiscount;

    std::ofstream AdminFile;
    AdminFile.open("admin.json");
    AdminFile << j.dump();
    AdminFile.close();
}

void Admin::LoadFromJSON()
{
    // Try to read the json
    std::ifstream i("admin.json");
    // File exists. Load the credentials
    if (i)
    {
        json j;
        i >> j;

        m_sUsername = j["Admin"]["sUserName"].get<std::string>();
        m_sPassword = j["Admin"]["sPassword"].get<std::string>();

        m_fVAT = j["Admin"]["VAT"].get<float>();
        m_fDiscount = j["Admin"]["Discount"].get<float>();
    }
    i.close();
}

bool Admin::Login(std::string sUsername, std::string sPassword)
{
    if (iWrongCredentialCounter > 4)
    {
        std::cout << "You have tried the incorrect details too many times. You will be locked out.\n";
        return false;
    }
    // Handle default credentials
    if (sUsername == m_sDefaultUsername || sPassword == m_sDefaultPassword)
    {
        std::cout << "You are using default credentials, you will be prompted to change them.\n\n";
        ChangeCredentials();

        m_bIsLoggedIn = true;

        return m_bIsLoggedIn;
    }
    // Normal login
    if ((sUsername == m_sUsername && sPassword == m_sPassword) || m_bIsLoggedIn == true)
    {
        m_bIsLoggedIn = true;

        return m_bIsLoggedIn;
    }
    else
    {
        std::cout << "Wrong credentials. You will be returned to the menu.\n";
        iWrongCredentialCounter++;
        return m_bIsLoggedIn;
    }
}

void Admin::SetVat(float fVAT)
{
    m_fVAT = fVAT;
}

void Admin::SetDiscount(float fDiscount)
{
    m_fDiscount = fDiscount;
}

bool Admin::IsLoggedIn()
{
    return m_bIsLoggedIn;
}
// We give the admin user some default credentials. Prompt admin to change them when they first log in. 
void Admin::FirstTimeLogIn()
{
    std::cout << "Since you are logging in using the default credentials you have to change them in order to continue.\n";

    std::string sNewUserName;
    std::string sNewPassword;
    std::string sNewPasswordConfirm;

    bool bNext = false;

    do // Default username check
    {
        std::cout << "Please enter the username\n";
        GetStringInput(sNewUserName);

        if (sNewUserName == m_sDefaultUsername)
        {
            std::cout << "Please choose a different username";
            bNext = true;
        }
        else { bNext = false; }
    } while (bNext);

    do  // Password match loop
    {
        do  // Default password check
        {
            std::cout << "Please enter the password\n";
            GetStringInput(sNewPassword);

            if (sNewPassword == m_sDefaultPassword)
            {
                std::cout << "Please choose a different password";
                bNext = false;
            }
            else { bNext = false; }
        } while (bNext);

        std::cout << "Confirm the password\n";
        GetStringInput(sNewPasswordConfirm);

        if (sNewPassword == sNewPasswordConfirm)
        {
            m_sUsername = sNewUserName;
            m_sPassword = sNewPassword;
            bNext = false;
        }
        else
        {
            std::cout << "The password don't match. Please try again.";
            bNext = true;
        }
    } while (bNext);

    // Finally save the updated info to a file (simulated database)
    SaveToJSON();
}
// Change admin credentials
void Admin::ChangeCredentials()
{
    std::string sNewUserName;
    std::string sNewPassword;
    std::string sNewPasswordConfirm;
    bool bNext = true;

    std::cout << "Please enter the username\n";
    GetStringInput(sNewUserName);

    std::cout << "Please enter the password\n";
    GetStringInput(sNewPassword);

    std::cout << "Confirm the password\n";
    GetStringInput(sNewPasswordConfirm);
    do
    {
        if (sNewPassword == sNewPasswordConfirm)
        {
            m_sUsername = sNewUserName;
            m_sPassword = sNewPassword;
            bNext = false;
        }
    } while (bNext);

    SaveToJSON();
}
// Edit the JSONs for the location. 
// The file is loaded on startup 
void Admin::EditLocations()
{
    int iChoice;
    bool bExit = true;
    do
    {
        std::cout << "1: Print current list of locations.\n";
        std::cout << "2: Edit an existing location.\n";
        std::cout << "3: Add a new location.\n";
        std::cout << "4: Remove a location\n";
        std::cout << "0: Exit\n";

        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0:
            bExit = false;
            break;
        case 1:
            PrintLocationsDetailed(*m_locations);
            break;
        case 2:
            EditLocation();
            break;
        case 3:
            AddLocation();
            break;
        case 4:
            RemoveLocation();
            break;
        default:
            std::cout << "Please select the appropriate option above.\n";
            break;
        }
    } while (bExit);
}

void Admin::EditLocation()
{
    int iChoice;
    std::string sChoice;
    bool bExit = true, bMore = true, bChoice = true, bOnce = true;
    if (bOnce)
    {
        PrintLocationsDetailed(*m_locations);
        bOnce = false;
    }
    do
    {
        std::cout << "Please which location you wish to edit.\n";

        iChoice = ValidateNumberInput();
        iChoice--;

        std::cout << "Are you sure you wish to edit " << m_locations->GetLocations().at(iChoice).m_sLocation << "?\n";
        do
        {
            GetStringInput(sChoice);

        } while (RegexYesNo(bMore, sChoice));

        if (bMore)
        {
            m_locations->EditLocation(iChoice);

            std::cout << "1: Edit another location.\n"
                << "0: Exit.\n\n";

            AnotherLocation(bExit);
        }
        else
        {
            std::cout << "1: Edit another location.\n"
                << "0: Exit.\n\n";

            AnotherLocation(bExit);
        }
    } while (bExit);
}

void Admin::AddLocation()
{
    std::string sNewLocationName, sChoice;
    bool bExit = true, bMore = true;
    do {
        std::cout << "\nPlease enter the name of the new location.\n";
        // Workaround to be able to use getline to get spaces
        GetStringInput(sNewLocationName);

        m_locations->AddNewLocation(sNewLocationName);

        std::cout << "Do you wish to add more locations?\n";
        GetStringInput(sChoice);
        RegexYesNo(bExit, sChoice);
    } while (bExit);
}

void Admin::RemoveLocation()
{
    int iChoice;
    std::string sChoice;
    bool bExit = true, bMore = true, bDelete = true;

    do
    {
        PrintLocations(*m_locations);

        std::cout << "Please which location you wish to delete.\n";
        iChoice = ValidateNumberInput();
        iChoice--;

        std::cout << "Are you sure you wish to delete " << m_locations->GetLocations().at(iChoice).m_sLocation << "?\n";
        do
        {
            GetStringInput(sChoice);
        } while (RegexYesNo(bMore, sChoice));

        if (bMore)
        {
            m_locations->RemoveLocation(iChoice);

            std::cout << "1: Edit another location.\n"
                << "0: Exit.\n\n";
            AnotherLocation(bExit);
        }
        else
        {
            std::cout << "1: Edit another location.\n"
                << "0: Exit.\n\n";
            AnotherLocation(bExit);
        }
    } while (bExit);
}

void Admin::EditActivities()
{
    int iChoice;
    bool bExit = true;
    do
    {
        std::cout << "1: Print current list of activities.\n";
        std::cout << "2: Edit an existing activity.\n";
        std::cout << "3: Add a new activity.\n";
        std::cout << "4: Remove an activity\n";
        std::cout << "0: Exit\n";

        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0:
            bExit = false;
            break;
        case 1:
            PrintAllAttractions(*m_locations);
            break;
        case 2:
            EditActivity();
            break;
        case 3:
            AddActivity();
            break;
        case 4:
            RemoveActivity();
            break;
        default:
            std::cout << "Please select the appropriate option above.\n";
            break;
        }
    } while (bExit);
}

void Admin::EditActivity()
{
    int iChoice;
    std::string sChoice;
    bool bExit = true, bMore;

    do
    {
        PrintAllAttractions(*m_locations);

        std::cout << "Please which activity you wish to edit.\n";
        iChoice = ValidateNumberInput();
        iChoice--;

        std::cout << "Are you sure you wish to edit " << m_locations->GetActivities().at(iChoice)->m_sActivity << "?\n";

        do
        {
            GetStringInput(sChoice);

        } while (RegexYesNo(bMore, sChoice));

        if (bMore)
        {
            m_locations->EditActivity(iChoice);

            std::cout << "1: Edit another location.\n"
                << "0: Exit.\n\n";

            AnotherLocation(bExit);
        }
        else
        {
            std::cout << "1: Edit another location.\n"
                << "0: Exit.\n\n";

            AnotherLocation(bExit);
        }
    } while (bExit);
}


void Admin::AddActivity()
{
    std::string sNewActivityName, sChoice;
    bool bExit = true, bMore = true;

    do {
        std::cout << "\nPlease enter the name of the new activity.\n";

        GetStringInput(sNewActivityName);
        m_locations->AddNewActivity(sNewActivityName);

        std::cout << "Do you wish to add more activities?\n";
        GetStringInput(sChoice);
        RegexYesNo(bExit, sChoice);
    } while (bExit);
}

void Admin::RemoveActivity()
{
    int iChoice;
    std::string sChoice;
    bool bExit = true, bMore = true, bDelete = true;

    do
    {
        PrintAllAttractions(*m_locations);

        std::cout << "Please which activity you wish to delete.\n";
        iChoice = ValidateNumberInput();
        iChoice--;

        std::cout << "Are you sure you wish to delete "
            << m_locations->GetActivities().at(iChoice)->m_sActivity << "?\n\n"
            << "NOTE: Removing an activity will remove it from the corresponding locations. Do you want to see the locations that will be affected?\n";
        do
        {
            GetStringInput(sChoice);
        } while (RegexYesNo(bMore, sChoice));

        if (bMore)
        {
            m_locations->RemoveActivity(iChoice);

            std::cout << "1: Remove another activity.\n"
                << "0: Exit.\n";
            AnotherLocation(bExit);
        }
        else
        {
            std::cout << "1: Remove another activity.\n"
                << "0: Exit.\n\n";
            AnotherLocation(bExit);
        }
    } while (bExit);
}

void Admin::EditFamily()
{
    auto family = SelectFamily();
    int iChoice, iLocationIndex;
    bool bExit = true;
    std::string sNewFamilyName = "", sOldFamilyName = "";
    do
    {


        std::cout << "1) Edit family name.\n"
            << "2) Edit the users.\n"
            << "3) Edit Location.\n"
            << "5) Save all changes.\n"
            << "0) Exit\n";

        iChoice = ValidateNumberInput();
        Location location;

        if (family)
        {
            switch (iChoice)
            {
            case 0: // Exit 
                bExit = false;
                break;
            case 1: // Edit the name
                std::cout << "Please enter the new name.\n";
                do 
                {
                    GetStringInput(sNewFamilyName);
                    if (sNewFamilyName == "")
                    {
                        std::cout << "Please enter at least 1 character.\n";
                    }
                } while (sNewFamilyName == "");
                sOldFamilyName = family->GetFamilyName();
                family->SetFamilyName(sNewFamilyName);
                break;
            case 2: // Edit the users
                break;
            case 3: // Edit the location
                PrintLocationsDetailed(location);
                std::cout << "Please choose the location.\n";
                iLocationIndex = ValidateNumberInput();
                iLocationIndex--;

                family->SetFamilyLocation(location.GetLocations().at(iLocationIndex).m_sLocation);
                break;
            case 5:
                 family->SaveToJSON(sOldFamilyName);
                break;
            default:
                std::cout << "Please choose a valid option\n";
                break;
            }
        }
    } while (bExit);
}

void Admin::AddFamily()
{
    std::string sNewFamilyName;
    std::cout << "Please enter the new family name.\n";
    GetStringInput(sNewFamilyName);

    auto family = Family::CreateFamily(sNewFamilyName);
    Location location;
    if (family)
    {
        FamilyMenu(family, location);
    }
}

void Admin::RemoveFamily()
{
    auto family = SelectFamily();
    std::string sChoice;
    bool bRegex;
    std::cout << "Are you sure you want to delete " << family->GetFamilyName() << "?\n"
        << "This will remove all users in that family as well.\n";

    if (family)
    {
        do
        {
            GetStringInput(sChoice);
        } while (RegexYesNo(bRegex, sChoice));

        family->DeleteFamily();
    }
}

void Admin::SeeFamilyDetails()
{
    auto family = SelectFamily();
    int iChoice;
    std::string sChoice;
    bool bExit = true;

    family->PrintListOfUsers();

    do
    {
        std::cout << "1) List all users.\n"
            << "2) Edit user.\n"
            << "0) Exit.\n";

        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0:
            bExit = false;
            break;
        case 1:
            family->PrintListOfUsers();
            break;
        case 2:
            EditUsers();
            break;
        default:
            std::cout << "Please select a valid option.\n";
            break;
        }
    } while (bExit);


}

void Admin::EditUsers()
{
    int iChoice;
    bool bExit = true;

    do
    {
        std::cout << "1) Print list of families.\n"
            << "2) Edit a family.\n"
            << "3) Delete a family.\n"
            << "4) Add a family.\n"
            << "5) See details of a family.\n"
            << "0) Exit\n";

        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0:
            bExit = false;
            break;
        case 1:
            PrintListFamilies(); 
            break;
        case 2:
            EditFamily();
            break; 
        case 3:
            RemoveFamily();
            break;
        case 4:
            AddFamily();
            break;
        case 5:
            SeeFamilyDetails();
            break;
        default:
            std::cout << "Please select the appropriate option above.\n";
            break;
        }
    } while (bExit);
}

void Admin::SaveChanges()
{
    m_locations->SaveToJson();
}


std::unique_ptr<Family> Admin::SelectFamily()
{
    PrintListFamilies();

    bool bExit = true;
    int iChoice;
    std::string sFamilyName;
    std::cout << "Please choose which family you wish to load. You have to use the exact as it's printed above.\n";

    do
    {
        GetStringInput(sFamilyName);

        std::ifstream i("jsons/" + sFamilyName + ".json");
        if (i)
        {
            auto family = Family::LoadFromJSON(sFamilyName);
            return family;
        }
        else
        {
            std::cout << "Family not found.\n";
            std::cout << "1) Try again.\n"
                << "0) Exit.\n";

            iChoice = ValidateNumberInput();
            switch (iChoice)
            {
            case 0:
                bExit = false;
                break;
            case 1:
                break;
            default:
                std::cout << "Please choose the correct option.\n";
                break;
            }
        }
    } while (bExit);

    return nullptr;
}

// Default Menu for admin
void Admin::AdminMenu()
{
    int iChoice, iIndex;
    bool bExit = true, bMisc = true;
    float fInput, fFormatedOutput;

    std::cout << "Please choose the appropriate option.\n";
    do
    {
        std::cout << "1) Edit locations.\n"
            << "2) Edit activities.\n"
            << "3) Edit users.\n"
            << "4) Misc settings\n"
            << "5) Edit credentials\n"
            << "6) Save all changes.\n"
            << "7) Logout.\n"
            << "8) Return to main menu.\n";

        iChoice = ValidateNumberInput();
        switch (iChoice)
        {
        case 1:
            EditLocations();
            break;
        case 2:
            EditActivities();
            break;
        case 3:
            EditUsers();
            break;
        case 4:
            do
            {
                std::cout << "You can change:\n"
                    << "1) VAT.\n"
                    << "2) Discount ammount.\n"
                    << "0) Exit.\n";
                iIndex = ValidateNumberInput();

                switch (iIndex)
                {
                case 0:
                    bMisc = false;
                    break;
                case 1:
                    // Formats the 1.2 to 20%
                    fFormatedOutput = GetVat() > 1 ? (GetVat() - 1) * 100 : (GetVat() + 1) * 100;
                    std::cout << "Please enter the new ammount in %." << "Currently it is at: " << fFormatedOutput << "%\n";
                    do
                    {
                        fInput = ValidateFloatInput();
                        if (fInput < 0)
                        {
                            std::cout << "Please enter a positive number.\n";
                        }
                    } while (fInput < 0);
                    // format 20(%) to 1.2
                    fInput = (fInput / 100) + 1;

                    SetVat(fInput);
                    break;
                case 2:
                    // Formats 0.9 to 10% 
                    fFormatedOutput = GetVat() > 1 ? abs((GetDiscout() - 1) * 100) : abs((GetDiscout() + 1) * 100);
                    std::cout << "Please enter the new ammount in %." << "Currently it is at: " << fFormatedOutput << "%\n";
                    do 
                    {
                        fInput = ValidateFloatInput();
                        if (fInput < 0)
                        {
                            std::cout << "Please enter a positive number.\n";
                        }
                    } while (fInput < 0);

                    // format 10(%) to 0.9
                    fInput = abs(fInput / 100 - 1);

                    SetDiscount(fInput);
                    break;
                default:
                    break;
                }
            } while (bMisc);
            break;
        case 5:
            ChangeCredentials();
            break;
        case 6:
            SaveChanges();
            break;
        case 7:
            bExit = false;
            m_bIsLoggedIn = false;
            break;
        case 8:
            bExit = false;
            break;
        default:
            std::cout << "Please select the appropriate option above.\n";
            break;
        }
    } while (bExit);
}