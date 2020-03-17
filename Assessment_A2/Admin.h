#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"

class Admin
{
private:
    std::string m_sUsername = "admin";
    std::string m_sPassword = "admin";
    bool m_bIsLoggedIn = false;
public:
    void HandleAdmin();
    void Login(std::string, std::string);
    bool IsLoggedIn();
};