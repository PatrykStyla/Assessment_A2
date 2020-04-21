#pragma once
#include "Location.h"
#include <regex>

// Prints all locations & price
void PrintLocations(Location&);
// Prints all locations & price & min travelers
void PrintLocationsDetailed(Location&);
// Print details for an individual location
void PrintDetailsForLocation(Location&, int);
// Print the attractions for a specific location & price
void PrintAttractions(Location&, int);
// Prints all attractions & price
void PrintAllAttractions(Location&);
// List the jsons file in the directory
void PrintListFamilies();
// validate yes/no. Will return true if the condition fails
bool RegexYesNo(bool&, std::string&);
// Will handle the input of a number, validate it and return the value when its correct
int ValidateNumberInput();
// Same but for floats
float ValidateFloatInput();
// used in admin for confirmation
void AnotherLocation(bool&);
// Get string with spaces
void GetStringInput(std::string&);
// https://stackoverflow.com/questions/5878775/how-to-find-and-replace-string
void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace);