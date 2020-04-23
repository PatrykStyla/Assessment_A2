#include "User.h"
#include "Helper.h"

User::User(std::string sName, int iYearsOld)
{
	m_sName = sName;
	m_iYearsOld = iYearsOld;
	// Check if person is a child
	m_iYearsOld < m_iIsConsideredChild ? true : false;
}


User::~User()
{
#if DebugPrint
    std::cout << "User: " << m_sName << " destroyed\n";
#endif
}

int User::GetIsConsideredChild()
{
	return m_iIsConsideredChild;
}

int User::GetIsConsideredAdult()
{
	return m_iIsConsideredAdult;
}

void User::SetUserName(std::string sName)
{
	m_sName = sName;
}

std::string User::GetUserName()
{
	return m_sName;
}

void User::SetAge(int iAge)
{
	m_iYearsOld = iAge;
}

int User::GetAge()
{
	return m_iYearsOld;
}

void User::SetTotalUserCost(float iTotalCost)
{
	m_iTotalCost += iTotalCost;
}

float User::GetTotalUserCost()
{
	return m_iTotalCost;
}

void User::SetActivity(Location::InvividualActivity& sActivity)
{
	m_sListOfAttractions.push_back(sActivity);
}

std::vector<Location::InvividualActivity> User::GetActivities()
{
	return m_sListOfAttractions;
}

void User::RemoveActivity(int iIndex)
{

}
bool User::IsChild()
{
	return m_iIsConsideredChild;
}
