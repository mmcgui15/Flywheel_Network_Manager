#pragma once

#include <string.h>
#include <iostream>



struct rotor_material {
    char name[32] = "";
    double density = 0;
    double shear_force = 0;

    rotor_material(char new_name[32], double new_den, double new_s_force);

    rotor_material& operator = (rotor_material& rm);
};

//add options for custom rotor materials??
//UPDATE WITH REAL MATERIAL DENSITIES AND SHEAR FORCES!
static rotor_material carbon_fiber("carbon fiber", 10, 0);
static rotor_material steel("steel", 11, 0);
static rotor_material iron("iron", 12, 0);
#define DEFAULT_RM carbon_fiber("carbon fiber", 10, 0)

class flywheel {

    public:
        enum shape {
            solid_sphere = 0, solid_cylinder = 1 //to add new rotor shape, must also update calc_mass() and flywheel settings menu in active_flywheel.cpp.
        }; //variable inertia flywheel??
     
        //flywheel member update and retreival fcns. short fcns defined in this header file
        void set_material(rotor_material& rm);
        rotor_material get_material() { return r_mat; }
        char* get_material_type() { return r_mat.name; }
        double get_material_density() { return r_mat.density; }
        double get_material_shear_force() { return r_mat.shear_force; }

        void set_rotor_type(shape s);
        shape get_rotor_type(){ return rotor_type; }

        double get_inertial_const() { return inertial_const; };
        double get_m_inertia() { return inertial_const*mass*radius*radius; }//i = kmr^2 in KGm^2

        double get_mass() { return mass; }
        void calc_mass();                                 //calculates mass from volume and density(rotor shape and material dependant)
        void set_mass(int new_mass) { mass = new_mass; }    //only for custom mass input

        void set_radius(double new_rad) { radius = new_rad; }
        double get_radius() { return radius; }

        void set_length(double new_len) { length = new_len; }
        double get_length() { return length; }

        void set_ang_vel(double new_vel) { ang_vel = new_vel; }    //sensor input required
        double get_ang_vel() { return ang_vel; }

        double get_kin_energy(){ return .5 * get_m_inertia() * ang_vel * ang_vel; } //total kinetic energy in moving flywheel

        char* get_name() { return flywheel_name; }
        /*char* get_name_c() {
            char fly_name_c[64];
            for (int i = 0; i < flywheel_name.size(); ++i) {
                fly_name_c[i] = flywheel_name[i];
            }
            return fly_name_c; }*/

        void rename(char new_name[]);

        void set_voltage();             //tbd... power input/output depends on motor/gennerator
        double get_voltage();

        flywheel();
        flywheel(shape rs);
        flywheel(char name[64], shape rs);

    private:

        void set_inertial_const(flywheel& f);
        char flywheel_name[64];      //make sure array does not overflow. do not update size without updating fcn!!
        shape rotor_type = shape::solid_cylinder;
        double radius = 0;           // m
        double length = 0;           //m. not used for all rotor shapes
        double mass = 0;             // kg
        double inertial_const = 0;
        double ang_vel = 0;          //currently in rpm
        rotor_material r_mat = carbon_fiber;
    };

std::istream& operator >>(std::istream& is, rotor_material& rm);
std::ostream& operator <<(std::ostream& os, rotor_material& rm);
std::istream& operator >>(std::istream& is, flywheel::shape& f_s);
std::ostream& operator <<(std::ostream& os, flywheel::shape& f_s);
std::istream& operator >>(std::istream& is, flywheel& fw);
std::ostream& operator <<(std::ostream& os, flywheel& fw);


