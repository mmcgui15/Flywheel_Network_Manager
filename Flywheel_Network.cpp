#include "Flywheel_Network.h"

bool show_fly_menu_flag = true;  //keeps flywheel network window open
bool add_flywheel_flag = false;  //adds a new flywheel when true
bool delete_flywheel_flag = false;
bool show_naming_menu_flag = false; // opens popup to name newly added flywheel when true

static int selected_fw = 0; //return value. Value becomes the subscript of the selected flywheel once the first one is added.

static int rotary_sensor_input = 0; //mock value (in RPM) of the input sensor for testing. controlled by slider

ImGuiWindowFlags Flywheel_network_flags = 0; //

void add_new_flywheel(std::vector<flywheel>& flybank, char* fly_name)
{
    flybank.push_back(flywheel(fly_name, flywheel::shape::solid_sphere));
}

void delete_flywheel(std::vector<flywheel>& flybank, int subscript) {
    if (flybank.size() > 0)
        flybank.erase(flybank.begin()+subscript);
    delete_flywheel_flag = false;
}

double total_stored_energy(std::vector<flywheel> fly_newtork) {
    double total_energy = 0;
    for (int i = 0; i < fly_newtork.size(); i++)
    {
        total_energy += fly_newtork[i].get_kin_energy();
    }
    return total_energy;
}


void name_flywheel(std::vector<flywheel>& fb1)    //Open window to name and add a flywheel. Name defaults to flywheel# if blank
{
    ImGui::Begin("Flywheel Entry");
    ImGui::Text("Enter name of flywheel");
    static char new_fly_name[64] = "";
    static bool valid_name = true;
    int default_fly_num = fb1.size() + 1;
    ImGui::InputText(" ", new_fly_name, IM_ARRAYSIZE(new_fly_name));

    if (valid_name == false)
        ImGui::Text("Error. Duplicate name entered. Enter New Name.");

        if (ImGui::Button("Add"))
        {
            if(!new_fly_name[0]) //default name if nothing is entered
                sprintf(new_fly_name, "flywheel #%d", default_fly_num);

            valid_name = true;

            for (int i = 0; i < fb1.size(); i++)
            {
                if (strcmp(fb1[i].get_name(), new_fly_name) == 0) {
                    valid_name = false;
                }
            }
            if (valid_name == true) {
                add_new_flywheel(fb1, new_fly_name);
                std::cout <<  fb1[0];
                sprintf(new_fly_name, "");
                add_flywheel_flag = false;
            }
         
            ImGui::CloseCurrentPopup();
        }
        if (ImGui::Button("Close"))
        {
            add_flywheel_flag = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::End();
}

// Flywheel Window. Returns the subscript of the flywheel selected flywheel to pass to active flywheel window.
int Show_Flywheel_Network(std::vector <flywheel>& fb, flywheel& fw)
{
    if (show_fly_menu_flag) Flywheel_network_flags |= ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Flywheel Network", NULL, Flywheel_network_flags); //second parameter is a window flag that keeps window open when null

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("flywheel menu"))
        {
            ImGui::MenuItem("Add Flywheel", NULL, &add_flywheel_flag);
            ImGui::MenuItem("Delete Flywheel", NULL, &delete_flywheel_flag);
            ImGui::MenuItem("Settings");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (delete_flywheel_flag) {
        delete_flywheel(fb, selected_fw);
        ImGui::End();
        selected_fw = 0;  //resets selected flywheel to prevent vector subscripting errors (only if last flywheel is deleted)
        return selected_fw;
    }

    //update and display all flywheels
    if (add_flywheel_flag) {
        name_flywheel(fb);
    }

    //choose the active flywheel
    for (int n = 0; n < fb.size(); n++)
    {
        char buf[32];
        sprintf(buf, fb[n].get_name());
        if(ImGui::Selectable(buf, selected_fw == n))
          selected_fw = n;
    }

    //Show flywheel network information
   // ImGui::Text("Total Stored Power = %.5f MW", total_stored_energy(flywheel_bank));

    
   

    ImGui::End();

    return selected_fw;
}

//load .fly file containing network settings
void load_network(std::string network_file, std::vector<flywheel>& fb1) {
    std::ifstream ifs(network_file.c_str());
    

    if (!ifs) {
        std::cerr << "error, can not open " << network_file; //... handle error
        return;
    }
    while (!ifs.eof()) {
        flywheel f_buffer("", flywheel::shape::solid_cylinder);
        while (ifs >> f_buffer)
            fb1.push_back(f_buffer);
        std::cout << "flywheel input: " << f_buffer;
    }
}

void store_network(std::string network_name, std::vector<flywheel>& fb1) {
    std::ofstream ofs(network_name.c_str());
    for (int i = 0; i < fb1.size(); i++) {
        ofs << fb1[i];
        std::cout << i << ' ';
    }
}







