#include <stdint.h>
#include "lander.h"
#include <math.h>
#include <iostream>
#include <unistd.h>

double total_mass;
double ship_weight = 534.0;
double force_one;
double max_velocity = 110.0;
double burn_rate;
double v_nozz = 2000.0;
double final_velocity = 0.0;

/* Declare extern method for the assembly language function*/

int main() {
    ship_state_type *rocket = new ship_state_type;
    rocket->fuel_weight = 1000.0;
    rocket->acceleration = 0.0; /* Initial acceleration = 0 */
    rocket->velocity = 0.0; /* Initial velocity = 0 */
    rocket->altitude = 0.0; /* Initial altitude = 0 */
    rocket->prior_burn_rate = 0.0; /* Initial prior burn rate  = 0 */

    rocket->time = 0.0; /* Initial time in air  = 0*/
    rocket->hover_count = 0.0; /* Initial hover count = 0 */
    rocket->hover_accomplished = 0; /* Initial hover accomplished = 0 */



    /* Call setBurnRate every 100 milliseconds */

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
            rocket->acceleration = (force_one / total_mass) - 10.0; /* in m/s^2*/
            rocket->altitude = (rocket->velocity * 0.1) + (0.5 * rocket->acceleration * (0.1 * 0.1));
            /* Do final velocity */
            final_velocity = rocket->velocity + (rocket->acceleration * 0.1);
            /* Do the vi */
            rocket->velocity = final_velocity;

     /*       std::cout << "Total mass: " << total_mass << std::endl;
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
            std::cout << "\n";*/
            // loop shit
            /*setBurnRate(*rocket);*/
            /*sleep((unsigned int)0.1);*/
            rocket->time += 1;
        }
      /*  std::cout << "Out of burn phase: \n";*/
        setBurnRate(*rocket);
        sleep((unsigned int)0.1);
        rocket->time += 1;
    }

    return 0;
}


double setBurnRate(ship_state_type ss) {

    return 0; // Obviously, not the correct answer ;-)
}
