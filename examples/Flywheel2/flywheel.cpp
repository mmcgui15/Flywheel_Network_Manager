#pragma once

#include "flywheel.h"

rotor_material::rotor_material(char new_name[32], double new_den, double new_s_force) {
    memcpy(name, new_name, 32);
    density = new_den;
    shear_force = new_s_force;
}

rotor_material& rotor_material::operator = (rotor_material& rm) {
    memcpy(name, rm.name, sizeof(rm.name));
    density = rm.density;
    shear_force = rm.shear_force;
    return *this;
}

    void flywheel::set_rotor_type(shape s) {    //inertia constant only updated by setting rotor type
        rotor_type = s;
        flywheel::set_inertial_const(*this);
    }

    void flywheel::set_material(rotor_material& rm) {
        r_mat = rm;
    }

    void flywheel::set_inertial_const(flywheel& f) {
        switch (f.rotor_type) {
        case shape::solid_sphere:   
            inertial_const = 0.4; //solid sphere. assumes evenly distributed mass
            f.set_length(0); //no length in spherical rotor, only radius
            break;
        case shape::solid_cylinder:   
            inertial_const = 0.5;  //solid cylinder. assumes evenly distributed mass
            break;
        default:    
            inertial_const = 0;
            break;
        }
    } 

    void flywheel::rename(char new_name[])
    {
        for (int i = 0; i < 64; i++)
            flywheel_name[i] = new_name[i];
    }

    void flywheel::calc_mass() {
        static const double pi = 3.14159;
        switch (rotor_type) {
        case shape::solid_sphere:
            mass = 4 / 3 * pi * radius * radius * radius * r_mat.density;
            break;
        case shape::solid_cylinder:
            mass = pi * radius * radius * length * r_mat.density;
            break;
        default:
            mass = 0;
            break;
        }
    }

    void flywheel::set_voltage() {}                     //TBD: input/output voltages?
    double flywheel::get_voltage() { return 0; }        //TBD: input/output voltages?
    
    flywheel::flywheel(shape rs){
        set_rotor_type(rs);
        rename("noname");
    }

    flywheel::flywheel() {
        set_rotor_type(shape::solid_cylinder);
        rename("noname");
    }

    flywheel::flywheel(char name[64], shape rs) {
        set_rotor_type(rs);
        rename(name);
    }

    //flywheel iostream operator overloads

    std::istream& operator >>(std::istream& is, rotor_material& rm) {
        return is;
    }
    std::ostream& operator <<(std::ostream& os, rotor_material& rm) {
        return os << rm.name << rm.density << rm.shear_force;
    }

    std::istream& operator >>(std::istream& is, flywheel::shape& f_s) {
        return is;
    }

    std::ostream& operator <<(std::ostream& os, flywheel::shape& f_s) {
        return os; //enum conversions??
    }
    
    std::istream& operator >>(std::istream& is, flywheel& fw) {
        char f_n[64];
        flywheel::shape r_t;
        double r, l, m, i_c, a_v;
        rotor_material r_ms(carbon_fiber);
        is >> f_n >> r_t >> r >> l >> m >> i_c >> a_v >> r_ms;
        return is;
    }

    std::ostream& operator<<(std::ostream& os, flywheel& fw) {
        return os << fw.get_name()
            << fw.get_rotor_type()
            << fw.get_radius()
            <<fw.get_length()
            <<fw.get_mass()
            <<fw.get_inertial_const()
            <<fw.get_ang_vel()
            <<fw.get_material();
    }
