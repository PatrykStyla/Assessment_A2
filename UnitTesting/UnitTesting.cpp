#include "pch.h"
#include "CppUnitTest.h"

#include "../Assessment_A2/User.h"
#include "../Assessment_A2/User.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(UnitTesting)
	{
	public:
		
		TEST_METHOD(InitializeUser)
		{
			std::string sName = "Test";
			int iAge = 15;

			User objUser(sName, iAge);
			if (sName != objUser.GetUserName() || iAge != objUser.GetAge())
			{
				Assert::Fail();
			}
		}
	};
}
 