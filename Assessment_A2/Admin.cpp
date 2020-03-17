#include "Admin.h"

void Admin::HandleAdmin()
{
}

void Admin::Login(std::string sUsername, std::string sPassword)
{
    if (sUsername == m_sUsername && sPassword == m_sPassword)
    {
        m_bIsLoggedIn = true;
    }
}

bool Admin::IsLoggedIn()
{
    return m_bIsLoggedIn;
}
