#pragma once


#include "Location.h"
#include "Family.h"
#include "Admin.h"

void FamilyMenu(std::unique_ptr<Family>& family, Location& locations);
void SelectLocation(Location& locations, std::unique_ptr<Family>& family);
void ActivitiesInEachLocation(Location& locations);