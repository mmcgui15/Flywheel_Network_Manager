#pragma once

#include "imgui.h"
#include "flywheel.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <string>



//flywheel network window settings
static flywheel f1(flywheel::shape::solid_sphere);
static std::vector <flywheel> flywheel_bank;

//draws flywheel network to window
void add_new_flywheel(std::vector<flywheel>& flybank, char* fly_name);
void delete_flywheel(std::vector<flywheel>& flybank, int subscript);
void name_flywheel(std::vector<flywheel>& fb1);
int Show_Flywheel_Network(std::vector <flywheel>& fb, flywheel& fw);


double total_stored_energy(std::vector<flywheel> fly_newtork);

//load .fly file containing network settings
void load_network(std::string network_file, std::vector<flywheel>& fb1);
void store_network(std::string network_name, std::vector<flywheel>& fb1);


