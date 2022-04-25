#include "Active_Flywheel.h"

bool active_flywheel_settings = false;
bool tach_active = false;
int artificial_rpm;          //when tachometer sensor is not hooked up, use slider to artificially adjust the angular velocity of flywheels.
static int frame_counter = 0;




void Show_Active_Flywheel(flywheel& active_flywheel)
{

    ImGui::Begin("Active Flywheel");
    if (ImGui::Button("Settings"))
        active_flywheel_settings = true;
    ImGui::Text(active_flywheel.get_name());
    ImGui::Text("Flywheel inertial constant = %.3f", active_flywheel.get_inertial_const());
    ImGui::Text("Angular velocity = %f", active_flywheel.get_ang_vel());
    ImGui::Text("Kinetic energy = %f", active_flywheel.get_kin_energy());
    ImGui::Text("Flywheel Radius = %.5f", active_flywheel.get_radius());
    ImGui::Text("Rotor Material = %s", active_flywheel.get_material_type());
    ImGui::Text("Density = %.5f", active_flywheel.get_material_density());
    ImGui::Text("Flywheel Mass = %.5f", active_flywheel.get_mass());
    if (active_flywheel.get_rotor_type() == flywheel::shape::solid_sphere)
        ImGui::Text("Flywheel Length = n/a for spherical rotor");
    else ImGui::Text("Flywheel Length = %.5f", active_flywheel.get_length());

    ImGui::Checkbox("Toggle Tachometer vs Slider", &tach_active);

    if (tach_active) {
        if (frame_counter == 60) {
            Read_Velocity_Sensors(active_flywheel);
            frame_counter = 0;
        }
        else frame_counter++;
    }
    else {
        ImGui::SliderInt("Manual Set RPM", &artificial_rpm, 0, 30000);
        active_flywheel.set_ang_vel(artificial_rpm);
    }

    
    ImGui::End();

    if (active_flywheel_settings)
        {
        static double new_radius = active_flywheel.get_radius();
        static double new_length = active_flywheel.get_length();
        static int rot_type_current_index = active_flywheel.get_rotor_type();
        static flywheel::shape new_rot_type = active_flywheel.get_rotor_type();
        static rotor_material new_rot_mat = active_flywheel.get_material();


        
        static flywheel* last_open_flywheel = &active_flywheel;
        if (&active_flywheel != last_open_flywheel) {
            new_radius = active_flywheel.get_radius();
            new_length = active_flywheel.get_length();
            rot_type_current_index = active_flywheel.get_rotor_type();

            //update the rotor material 
            last_open_flywheel = &active_flywheel;
        }

            ImGui::Begin("Flywheel Settings");
            ImGui::Text("%s settings", active_flywheel.get_name());

            //flywheel settings dropdown("Combo") menus
            static ImGuiComboFlags flags = 0;

            const char* rotor_types[] = { "Solid Sphere", "Solid Cylinder"};
            const char* rot_type_preview_value = rotor_types[rot_type_current_index];  
            if (ImGui::BeginCombo("Flywheel Type", rot_type_preview_value, flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(rotor_types); n++)
                {
                    const bool is_selected = (rot_type_current_index == n);
                    if (ImGui::Selectable(rotor_types[n], is_selected))
                    {
                        rot_type_current_index = n;
                        switch (n) {
                        case 0:new_rot_type = flywheel::shape::solid_sphere;
                            //box for diameter
                            break;
                        case 1:
                            new_rot_type = flywheel::shape::solid_cylinder;
                            //boxes for diameter and length
                            break;
                        }
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            
            ImGui::InputDouble("Rotor Radius", &new_radius);
            
            if(rot_type_current_index !=0) {         //only radius is needed for spherical rotor
                ImGui::InputDouble("Rotor Length", &new_length);
            }
            

            const char* rotor_material[] = { "Carbon Fiber", "Steel", "Iron"};
            static int rot_mat_current_index = 0; 
            const char* rot_mat_preview_value = rotor_material[rot_mat_current_index];
            if (ImGui::BeginCombo("Flywheel Material", rot_mat_preview_value, flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(rotor_material); n++)
                {
                    const bool is_selected = (rot_mat_current_index == n);
                    if (ImGui::Selectable(rotor_material[n], is_selected))
                    {
                        rot_mat_current_index = n;
                        switch (n) {
                        case 0: new_rot_mat = carbon_fiber;
                            break;
                        case 1: new_rot_mat = steel;
                            break;
                        case 2: new_rot_mat = iron;
                            break;
                        }
                        rot_mat_current_index = n;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Update Settings")) {
                active_flywheel.set_radius(new_radius);
                active_flywheel.set_length(new_length);
                active_flywheel.set_rotor_type(new_rot_type);
                active_flywheel.set_material(new_rot_mat);
                active_flywheel.calc_mass();
                active_flywheel_settings = 0;
            }

            ImGui::End();
        }
}

void Read_Velocity_Sensors(flywheel& fly) {
        fly.set_ang_vel(get_rpm());
        fly.get_kin_energy();
}
