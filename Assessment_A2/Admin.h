#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Location.h"
#include "Helper.h"
#include "Family.h"

class Admin
{
private:
    std::string m_sDefaultUsername = "admin";
    std::string m_sDefaultPassword = "admin";
    std::string m_sUsername;
    std::string m_sPassword;
    bool m_bIsLoggedIn = false;
    int iWrongCredentialCounter = 0;

    Location* m_locations;
private:
    void SaveToJSON();
    void LoadFromJSON();
    // locations
    void EditLocations();
    void EditLocation();
    void AddLocation();
    void RemoveLocation();
    // activities
    void EditActivities();
    void EditActivity();
    void AddActivity();
    void RemoveActivity();
    // Family/Users
    void EditUsers();
    void EditFamily();
    void AddFamily();
    void RemoveFamily();

    void SaveChanges();
    void SeeFamilyDetails();

    std::unique_ptr<Family> SelectFamily();
public:
    Admin(Location&);
    bool Login(std::string, std::string);
    bool IsLoggedIn();
    void FirstTimeLogIn();
    void ChangeCredentials();




    void AdminMenu();
};