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
