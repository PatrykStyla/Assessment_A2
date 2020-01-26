#include "Family.h"

Family::Family(std::string FamilyName)
{
    m_sFamilyName = FamilyName;
}

Family::~Family()
{
#ifdef _DEBUG
    std::cout << "Family: " << m_sFamilyName << " destroyed\n";
#endif // DEBUG
}

void Family::SetUserLocation(std::string sLocation)
{
    m_sLocation = sLocation;
}