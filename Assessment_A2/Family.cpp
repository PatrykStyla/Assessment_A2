#include "Family.h"

Family::Family(std::string FamilyName)
{
    m_sFamilyName = FamilyName;
}

Family::~Family()
{
	std::cout << "Family: " << m_sFamilyName << " destroyed\n";
}

void Family::SetUserLocation(std::string sLocation)
{
    m_sLocation = sLocation;
}