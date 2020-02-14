#include "Family.h"

Family::Family(std::string FamilyName)
{
    m_sFamilyName = FamilyName;
}

Family::~Family()
{
	std::cout << "Family: " << m_sFamilyName << " destroyed\n";
}

void Family::SetFamilyLocation(std::string sLocation)
{
    m_sLocation = sLocation;
}

std::string Family::GetFamilyLocation()
{
    return m_sLocation;
}

void Family::SetFamilyTotalCost(int iTotalCost)
{

}

int Family::GetFamilyTotalCost()
{
    return m_iTotalCostOfUsers;
}

void Family::SetFamilyName(std::string sFamilyName)
{
}

std::string Family::GetFamilyName()
{
    return m_sFamilyName;
}

void Family::SetFamilyUser(std::shared_ptr<User> objuser)
{
    m_clsUsers.push_back(objuser);
}

std::vector<std::shared_ptr<User>> Family::GetFamilyUsers()
{
    return m_clsUsers;
}
