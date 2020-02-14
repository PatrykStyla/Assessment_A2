#include "User.h"

User::User(std::string sName, int iYearsOld)
{
	m_iIsconsideredChild = 13;

	m_sName = sName;
	m_iYearsOld = iYearsOld;
	// Check if person is a child
	m_iYearsOld < m_iIsconsideredChild ? true : false;

	// std::cout << "User Contrusted: " << m_sName << "\n";
}


User::~User()
{
	std::cout << "User: " << m_sName << " destroyed\n";
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

void User::SetTotalUserCost(int iTotalCost)
{
	m_iTotalCost = iTotalCost;
}

int User::GetTotalUserCost()
{
	return m_iTotalCost;
}

bool User::IsChild()
{
	return m_iIsconsideredChild;
}
