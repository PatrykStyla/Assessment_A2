#include "pch.h"
#include "CppUnitTest.h"

#include "../Assessment_A2/User.h"
#include "../Assessment_A2/User.cpp"
#include "../Assessment_A2/Admin.h"
#include "../Assessment_A2/Admin.cpp"
#include "../Assessment_A2/Location.h"
#include "../Assessment_A2/Location.cpp"
#include "../Assessment_A2/Helper.h"
#include "../Assessment_A2/Helper.cpp"
#include "../Assessment_A2/Family.h"
#include "../Assessment_A2/Family.cpp"
#include "../Assessment_A2/main.h"
#include "../Assessment_A2/main.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTesting
{
	TEST_CLASS(UnitTesting)
	{
	public:
		// This will be used throughout the testing as it doesn't really change
		Location location;
		// location class should ALWAYS load from a file
		// in case it doesn't exist it SHOULD default to some original values
		TEST_METHOD(TesLocationLoading)
		{
			// Remove the file that location is loading from
			std::remove("Locations.json");
			// try to access any data.
			Location location2;
			location2.GetLocations();
		}
		TEST_METHOD(TestAdmin)
		{
            // Remove the file that location is loading from
            std::remove("admin.json");

			Admin admin(location);

			admin.GetVat();
		}
		TEST_METHOD(InitUser)
		{
			std::string sName = "Test";
			int iAge = 15;

			User objUser(sName, iAge);
			if (sName != objUser.GetUserName() || iAge != objUser.GetAge())
			{
				Assert::Fail();
			}
		}
		TEST_METHOD(InitFamily)
		{
			std::string sName = "Test";
			Family family(sName);

			if (sName != family.GetFamilyName())
			{
				Assert::Fail();
			}
		}
		TEST_METHOD(AddUser)
		{
            std::string sNameFamily = "Test";
            Family family(sNameFamily);

            std::string sNameUser = "Test";
            int iAge = 15;

			std::shared_ptr<User> objUser = std::make_shared<User>(sNameUser, iAge);
			family.SetFamilyUser(objUser);

			if (family.GetUsers().at(0)->GetUserName() != sNameUser || family.GetUsers().at(0)->GetAge() != iAge)
			{
				Assert::Fail();
			}
		}
		// Validate whether age affects the family counter
		TEST_METHOD(AgeCounter)
		{
            std::string sNameFamily = "Test";
            Family family(sNameFamily);

            std::string Child;
            int iChild = 5;

            std::string Undefined;
            int iUndefined = 14;

            std::string Adult;
            int iAdult = 18;


            // The default values are 
			// > 18 adult
			// < 9 child
			// everything else is undefined
			family.ValidateAge(iChild);
			family.ValidateAge(iUndefined);
			family.ValidateAge(iAdult);

			if (family.GetAdultCounter() != 1)
			{
				Assert::Fail(L"No adults");
			}
			if (family.GetChildCounter() != 1)
			{
				Assert::Fail(L"No children");
			}
			if (family.GetUndefinedCounter() != 1)
			{
				Assert::Fail(L"No undefined");
			}

			// Change an age
			// Adult -> child
			iAdult = 5;
			// Normally you should pass the old age of the user but for demonstration sake we hardcore it
			family.ValidateAge(iAdult, 19);
			// No adults should be present and +1 should be present
            if (family.GetAdultCounter() != 0)
            {
                Assert::Fail(L"No adults removed");
            }
            if (family.GetChildCounter() != 2)
            {
                Assert::Fail(L"No children added");
            }
		}
		TEST_METHOD(AddLocation)
		{
			Location location;

			std::string sLocatioName = "test";
			float fPrice = 0;
			int iMinimumTravelers = 1;

			location.SetLocation(sLocatioName, fPrice, iMinimumTravelers);
			auto NewLocation = location.GetLocations().back();

			if (NewLocation.m_sLocation != sLocatioName || NewLocation.m_fPrice != fPrice || NewLocation.m_iMinimumTravellers != iMinimumTravelers)
			{
				Assert::Fail(L"Location not added properly");
			}
		}
		TEST_METHOD(AddActivityToLocation)
		{
			Location location;

			std::string sActivityName = location.GetActivities().at(1)->m_sActivity;
			float fPrice = location.GetActivities().at(1)->m_fCostPerPerson;

			// We need to specify the location index and the activity index we wish to add.
			// We get the location index from here subtract by 1
			PrintLocationsDetailed(location);
			// We get the activity index from here subtract by 1
			PrintAllAttractions(location);
			// By default 0 is Florida
			// and 1 is Guided Tours
			location.AddActivityToLocation(0, 1);

			auto activity = location.GetLocations().at(0).Activities.back();

			if (activity->m_sActivity != sActivityName || activity->m_fCostPerPerson != fPrice)
			{
				Assert::Fail(L"Activity not added properly");
			}
		}
		TEST_METHOD(AddSameActivityToLocation)
		{
			Location location;

			auto iActivitiesSize = location.GetLocations().at(0).Activities.size();

			// Florida already has Theme Park(0) added. It shouldn't be added twice
			location.AddActivityToLocation(0, 0);

			auto iActivitiesSizeNew = location.GetLocations().at(0).Activities.size();

			if (iActivitiesSize != iActivitiesSizeNew)
			{
				Assert::Fail(L"Activity at this location are different");
			}

			// Sanity check
			// Add Guided Tours
			location.AddActivityToLocation(0, 1);

			auto iActivitiesSizeNewNew = location.GetLocations().at(0).Activities.size();

			if (iActivitiesSize != iActivitiesSizeNewNew - 1)
			{
				Assert::Fail(L"Sanity check when adding location");
			}
		}
		TEST_METHOD(RemoveLocation)
		{
			Location location;
			// Get old size
			auto LocationSize = location.GetLocations().size();
			// 0 = Florida
			// Get old name at index
			std::string sName = location.GetLocations().at(0).m_sLocation;
			//remove
			location.RemoveLocation(0);

			auto locations = location.GetLocations();
			if (locations.at(0).m_sLocation == sName || LocationSize == locations.size() )
			{
				Assert::Fail(L"Location not removed properly");
			}
		}
		TEST_METHOD(RemoveActivity)
		{
            Location location;
            auto& activities = location.GetActivities();
            std::string sName = activities.at(0)->m_sActivity;
            auto iSize = activities.size();
			// Theme Park
            location.RemoveActivity(0);

            if (activities.at(0)->m_sActivity == sName || activities.size() == iSize)
            {
                Assert::Fail(L"Activity not removed properly");
            }

			// Activities should also be removed from the locations as well

			if (location.GetLocations().at(0).Activities.at(0)->m_sActivity == sName)
			{
				Assert::Fail(L"Activity not removed from location.\n");
			}
		}
	};
}