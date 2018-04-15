#include "lander.h"
#include <stdint.h>
#include<iostream>
#include<math.h>
/*TODO:
 * 1. Can declare anything within this file needed */


/* Declare extern method for the assembly language function*/
/*
int main() {
    ship_state_type *rocket = new ship_state_type;
    rocket->fuel_weight = 1000.0;
    rocket->acceleration = 0.0; *//* Initial acceleration = 0 *//*
    rocket->velocity = 0.0; *//* Initial velocity = 0 *//*
    rocket->altitude = 0.0; *//* Initial altitude = 0 *//*
    rocket->prior_burn_rate = 0.0; *//* Initial prior burn rate  = 0 *//*

    rocket->time = 0.0; *//* Initial time in air  = 0*//*
    rocket->hover_count = 0.0; *//* Initial hover count = 0 *//*
    rocket->hover_accomplished = 0; *//* Initial hover accomplished = 0 *//*



    *//* Call setBurnRate every 100 milliseconds *//*

    while (rocket->time < 500) {

        while (rocket->time < 200) {
            std::cout << "Current timer: " << rocket->time << std::endl;
            total_mass = ship_weight + rocket->fuel_weight;
            std::cout << "Total_Mass: \t" << total_mass << std::endl;
            force_one = total_mass * 109.0;
            burn_rate = force_one / v_nozz;
            rocket->fuel_weight = (rocket->fuel_weight)  - (burn_rate * delta_t);
            std::cout << "Fuel weight: \t" << rocket->fuel_weight << std::endl;
            total_mass = ship_weight + rocket->fuel_weight;
            std::cout << "New total mass: " << total_mass << std::endl << "\n";
            rocket->acceleration = (force_one / total_mass) - 10.0; *//* in m/s^2*//*
            rocket->altitude = (rocket->velocity * 0.1) + (0.5 * rocket->acceleration * (0.1 * 0.1));
            *//* Do final velocity *//*
            final_velocity = rocket->velocity + (rocket->acceleration * 0.1);
            *//* Do the vi *//*
            rocket->velocity = final_velocity;

     *//*       std::cout << "Total mass: " << total_mass << std::endl;
            std::cout << "Burn Rate: " << burn_rate << std::endl;
            std::cout << "BURNED THIS AMOUNT OF FUEL: " << burn_rate * delta_t << std::endl;
            std::cout << "Rocket acceleration: " << rocket->acceleration << std::endl;
            std::cout << "Rocket altitude: " << rocket->altitude << std::endl;
            std::cout << "Rocket Final Velocity: " << final_velocity << std::endl;
            std::cout << "Rocket Initial Velocity: " << rocket->velocity << std::endl;
            std::cout << "Rocket fuel weight: \n";
            std::cout << rocket->fuel_weight;
            std::cout << "\nTICK: ";
            std::cout << rocket->time;
            std::cout << "\n";*//*
            // loop shit
            *//*setBurnRate(*rocket);*//*
            *//*sleep((unsigned int)0.1);*//*
            rocket->time += 1;
        }
      *//*  std::cout << "Out of burn phase: \n";*//*
        setBurnRate(*rocket);
        sleep((unsigned int)0.1);
        rocket->time += 1;
    }

    return 0;
}*/

/*Set the shipweight explicitly since we can't acces the .ino file*/
double explicit_ship_weight = 534.0;
double mass_rocket = explicit_ship_weight; /* Measured in kgs */
double mass_fuel = 1000.0; /* Measured in kgs */
/* double mass_total = mass_rocket + mass_fuel;  Measured in kgs */
double mass_total = mass_rocket + mass_fuel;
double burn_rate = 0.0; /* Measured in ? */
double final_height = 0.0;/* Measured in meters */
/* double target_height = 0.0;  The target height for the ship to reach */
double altitude = 0.0; /* Keeps track of the current height */
double change_in_altitude_since_last_time_slice = 0.0; /* */
double force_1 = 0.0; /* measured in Newtons */
double force_2 = 0.0; /* measured in Newtons */
/* nozzle velocity is declared in lander.h. and is 2000 m/s */
double acceleration = 0.0; /* measured in meters/seconds^(2) */
double final_velocity = 0.0; /* Measured in meters/second */
double initial_velocity = 0.0;
double change_in_velocity_since_last_time_slice = 0.0;
double range = 0.0; /* measured in meters */
double top_of_range = final_height + range;
double bottom_of_range = final_height - range;
/* Originally initialized as bottom_of_range + hover_range but that is not defined*/
double target_height = bottom_of_range + range;
double T = 0.0;
double height_cutoff = target_height - 200;
double velocity_max = sqrt(2000);

double take_off() {
    double change_in_altitude = 0.0;
    force_1 = mass_total * 109;
    burn_rate = force_1 / nozzle_velocity;

    /* Update the following:
     * 1. Total mass
     * 2. Acceleration
     * 3. Altitude
     * 4. Final velocity
     * 5. Initial Velocity
     * 6. Change in altitude
     * */
    mass_total = mass_total - (burn_rate * 0.1);
    acceleration = (force_1 / mass_total) - 10;
    altitude = altitude + (initial_velocity * 0.1) + (0.5 * acceleration * 0.1 * 0.1);
    final_velocity = initial_velocity + (acceleration * 0.1);
    initial_velocity = final_velocity;
    change_in_altitude = height_cutoff - altitude;

    /* Increase the time slice*/
    T += 0.1;

    return burn_rate;
}

double take_us_to_goal() {


}

/*
 * Use a stack initialized within this stack that pushes the initial altitude onto it, and
 * then pops it into the parameter of this function along with the final_height in order to return the change
 * in altitude
 * double calculate_change_in_altitude(double initial_altitude_at_beginning_of_time_slice, double final_height) {
 *
 *  return final_height - initial_altitude_at_beginning_of_time_slice;
 * }
 * */



double calculate_change_in_velocity(double the_initial_velocity_at_beginning_of_time_slice, double the_final_velocity) {
    return the_final_velocity - the_initial_velocity_at_beginning_of_time_slice;
}



double setBurnRate(ship_state_type ss) {
   double ze_burn_rate = 0.0;
    if (T < 0.5){
        ze_burn_rate = take_off();
    } else if ((altitude < height_cutoff) && (T>= 0.5)) {
        ze_burn_rate = take_us_to_goal();
    }

    return ze_burn_rate;
}

int main() {

    return 0;
}
