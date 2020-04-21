#include "main.h"

void BookTrip(Family* family)
{
    int iChoice;
    float fPrice = family->GetFamilyTotalCost(), fFormatedOutput;
    if (family->GetAdultCounter() > 2 && family->GetChildCounter() > 2)
    {
        // at least 2 adults and children. Apply discount
        fPrice *= Admin::GetVat() * Admin::GetDiscout();
        fFormatedOutput = Admin::GetVat() > 1 ? abs((Admin::GetDiscout() - 1) * 100) : abs((Admin::GetDiscout() + 1) * 100);
        std::cout << "Since there are at least 2 adults and children a " << Admin::GetDiscout() << " % has been applied.\n";
    }
    else
    {
        fPrice *= Admin::GetVat();
    }
    std::cout << "Your final price after taxes is: " << char(156) << fPrice << "\n";

    std::cout << "1)Pay with card.\n";
    iChoice = ValidateNumberInput();

    switch (iChoice)
    {
    case 1:
        std::cout << "Your purchase has been successful.\n";
    default:
        std::cout << "Please choose the appropriate option.\n";
        break;
    }
}

void Checkout(Family* family)
{
    int iChoice;
    bool bExit = true;
    std::cout << "Below are all the details you have selected.\n";
    std::cout << "Location: " << family->GetFamilyLocation() << " Total Cost: " << char(156) << family->GetFamilyTotalCost() << "\n";
    family->PrintListOfUsers();
    std::cout << "\nNOTE: The prices above exclude any VAT or other taxes.\n";
    do
    {
        std::cout << "1) If you want to change anything. You will redirected to the previous menu.\n"
            << "2) Book the trip.\n";
        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 1:
            bExit = false;
            break;
        case 2:
            BookTrip(family);
            // EXIT
            bExit = false;
            break;
        default:
            std::cout << "Please enter a valid option.\n";
            break;
        }
    } while (bExit);
}

// Slightly modified version of the Admin::EditFamily()
void EditFamily(Family* family)
{
    int iChoice, iLocationIndex;
    bool bExit = true;
    std::string sNewFamilyName = "", sOldFamilyName = "";
    do
    {
        std::cout << "1) Edit family name.\n"
            << "2) Edit the users.\n"
            << "3) Edit Location.\n"
            << "4) Save all changes.\n"
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
            case 4:
                family->SaveToJSON(sOldFamilyName);
                break;
            default:
                std::cout << "Please choose a valid option\n";
                break;
            }
        }
    } while (bExit);
}

void AfterSelectionMenu(Family* family)
{
    int iChoice;
    bool bExit = true;
    std::cout << "Thank you for choosing the activities. Your family has been saved so you can come back at any time. You can now:\n";
   
    do
    {
        std::cout << "1) Edit your family.\n"
            << "2) Go to checkout.\n"
            << "0) Exit to main menu\n";
        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0:
            bExit = false;
            break;
        case 1:
            EditFamily(family);
            break;
        case 2:
            Checkout(family);
            bExit = false;
            break;
        default:
            std::cout << "Please enter a valid option.\n";
            break;
        }
    } while (bExit);
}

void SelectLocation(Location& locations, std::unique_ptr<Family>& family)
{
    int iChoiceDestination, iHowManyAttractions, iAttraction;
    bool bChoiceDestination = true, bAttraction = false;

    PrintLocationsDetailed(locations);
    do
    {
        iChoiceDestination = ValidateNumberInput();
        // remove the offset
        iChoiceDestination--;

        if (iChoiceDestination >= 0 && iChoiceDestination < locations.GetLocations().size())
        {
            family->SetFamilyLocation(locations.GetLocations().at(iChoiceDestination).m_sLocation);
            // Set LOCATION price for every user.
            for (size_t i = 0; i < family->GetUsers().size(); i++)
            {
                family->SetFamilyTotalCost(locations.GetLocations().at(iChoiceDestination).m_fPrice);
            }
            auto users = family->GetFamilyUsers();

            std::cout << "\nChoose an attraction for each user " << "\n\n";
            PrintAttractions(locations, iChoiceDestination);

            std::cout << "\nPlease add attractions for every user from the available list\n"
                << "If you don't want to add any attractions press 0(zero)\n\n";
            for (auto& user : users)
            {
                std::cout << "How many attractions do you wish to add for the user: " << user->GetUserName() << "?\n";
                do
                {
                    iHowManyAttractions = ValidateNumberInput();
                    // user wants to add x attractions for that user
                    if (iHowManyAttractions > 0)
                    {
                        for (size_t i = 0; i < iHowManyAttractions; i++)
                        {
                            do
                            {
                                bAttraction = false;
                                std::cout << "Please enter the attraction " << i + 1 << "\n";
                                iAttraction = ValidateNumberInput();
                                iAttraction--;

                                if (iAttraction >= 0 && iAttraction < locations.GetLocations().at(iChoiceDestination).Activities.size())
                                {
                                    user->SetActivity(*locations.GetActivities().at(i));
                                    // set ACTIVITY price for each activity for each user
                                    user->SetTotalUserCost(locations.GetActivities().at(i)->m_fCostPerPerson);
                                }
                                else
                                {
                                    std::cout << "Please select a valid option.\n";
                                    // do while bool
                                    bAttraction = true;
                                }
                            } while (bAttraction);
                        }
                        // Add the TOTAL user cost to the total family cost to tally it all up
                        family->SetFamilyTotalCost(user->GetTotalUserCost());
                    }
                    else if (iHowManyAttractions == 0)
                    {
                        // User does not want to add any attractions. Continue with the next user
                    }
                    else
                    {
                        std::cout << "Please enter a positive number\n";
                    }
                } while (iHowManyAttractions < 0);
            }
            // looped through the users. Exit
            bChoiceDestination = false;
            family->SaveToJSON();
        }
        else 
        { 
            std::cout << "Please enter a positive number\n";
        }
    } while (bChoiceDestination);
    // Once the user has added all the activities
    AfterSelectionMenu(family.get());
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
            PrintAllAttractions(locations);
            do
            {
                iChoice = ValidateNumberInput();
                // Remove offset
                iChoice = iChoice - 1;
                if (iChoice >= 0 && iChoice < locations.GetActivities().size())
                {
                    PrintLocationsDetailed(locations);
                }
                else
                {
                    std::cout << "Please choose one of the available locations\n";
                    bLocation = true;
                }
            } while (bLocation);


            std::cout << "\nDo you want to check other locations?\n";

            GetStringInput(sChoice);
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

void Login(Admin& admin)
{
    // admin is already logged in
    if (admin.IsLoggedIn())
    {
        admin.AdminMenu();
    }
    else
    {
        std::string sUserName;
        std::string sPassword;

        std::cout << "Please enter the username\n";
        GetStringInput(sUserName);

        std::cout << "Please enter the password\n";
        GetStringInput(sPassword);

        if (admin.Login(sUserName, sPassword))
        {
            // Logged in
            admin.AdminMenu();
        }
    }
}

void FamilyMenu(std::unique_ptr<Family>& family, Location& locations)
{
    bool bExit = true;
    do
    {
        if (family)
        {
            int iChoiceSelect;
            bool bChoiceSelect = false;

            std::cout << "select\n\n";
            do
            {
                bChoiceSelect = false;
                std::cout << "1) Select your desired destination\n"
                    << "2) See what activities are available in each location\n"
                    << "3) Go to checkout.\n"
                    << "0) Go to the selection menu.\n";

                iChoiceSelect = ValidateNumberInput();

                switch (iChoiceSelect)
                {
                case 0:
                    // Exit
                    bExit = false;
                    break;
                case 1:
                    SelectLocation(locations, family);
                    // Exit to main loop.
                    bChoiceSelect = false;
                    bExit = false;
                    break;
                case 2:
                    ActivitiesInEachLocation(locations);
                    break;
                case 3:
                    Checkout(family.get());
                    break;
                default:
                    std::cout << "Please enter the correct option\n\n";

                    bChoiceSelect = true;
                    break;
                }
            } while (bChoiceSelect);
        }
    } while (!family || bExit);
}

void StartupMenu(Location& locations, Admin& admin)
{
    bool bMenu = true, bOnlyOnce = true, bExit = true;
    int iChoice = 0;
    std::string sFamilyName;
    std::unique_ptr<Family> family;

    do // Startup menu loop (main loop of the program)
    {
        if (bOnlyOnce)
        {
            std::cout << "Welcome to the program. Select the appropriate option.\n";
            bOnlyOnce = false;
        }

        std::cout << "1) Load a family\n"
            << "2) Create a new family\n"
            << "3) Login\n"
            << "0) Exit.\n";

        iChoice = ValidateNumberInput();

        switch (iChoice)
        {
        case 0: // Exit and terminate the program
            std::cout << "Thank you for using the program.\n";
            bExit = false;
            break;
        case 1: // Load locally stored family
            std::cout << "Please enter the name of the family you want to load\n";

            getline(std::cin, sFamilyName);

            family = family->LoadFamily(sFamilyName);
            break;
        case 2: // Create a new family
            std::cout << "Please enter the name of the family you want to create\n";

            getline(std::cin, sFamilyName);
            if (family->CheckIfFamilyAlreadyExists(sFamilyName))
            {
                // family already exists
                family = family->FamilyAlreadyExists(sFamilyName);
            }
            else
            {
                family = family->CreateFamily(sFamilyName);
            }
            break;
        case 3:  // Admin login
            Login(admin);
            break;
        default:
            std::cout << "Please select the appropriate option above.\n";
            break;
        }

        // If the user decided to load/create a family
        if (family && bExit)
        {
            FamilyMenu(family, locations);
        }

    } while (bExit);
}

int main()
{
    std::cout << "Welcome to the holiday program \n";
    Location locations;
    Admin admin(locations);
    // A user will either load an existing family or create a new one
    // OR an admin can log in to the system
    StartupMenu(locations, admin);

    std::cout << "Thank you using the program.\n";
}