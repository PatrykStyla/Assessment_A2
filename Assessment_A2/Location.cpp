#include "Location.h"

Location::Location()
{
	m_sLocation = { "Florida", "Paris", "Milan", "Tokyo" };
	// Initialize the values
	m_sLocation =			{ "Florida", "Paris", "Milan", "Tokyo" };
	m_fPrice =				{ 1300, 600, 950, 1800 };
	m_fMinimumTravellers =	{ 4, 2, 2, 4 };

	m_sActivity =			{"Theme Park", "Guided Tours", "Sky Diving"};
	m_fCostPerPerson =		{ 80, 35, 110 };
	m_sAvailableLocations = { {"Florida", "Paris" ,"Tokyo"}, {"Paris", "Milan" ,"Tokyo"}, {"Florida"} };
}
