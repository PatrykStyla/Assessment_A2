#include <iostream>
#include "Location.h"
#include "User.h"
#include "Family.h"

int main()
{
	Location locations;
	Family family;
	std::string sName;
	int iAge;

	std::cout << "Welcome to the holiday program \n";
	std::cout << "Please add add all the individuals that will be participating in the trip \n";



	std::cout << "Enter the name \n";
	std::cin >> sName;

	std::cout << "Enter the age \n";
	std::cin >> iAge;

	// Create a user
	User user(sName, iAge);
	// Add user to the family
	family.m_clsUsers.push_back(user);
	// Select the destination
	std::cout << "Please select your desired destination from the list below \n";
	
	for (size_t i = 0; i < locations.m_sAvailableLocations.size(); i++)
	{
		std::cout << i << ": " << locations.m_sLocation[i] << "\n";
	}
}