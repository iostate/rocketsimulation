#include <stdint.h>
#include "lander.h"
#include <math.h>
#include <iostream>
#include <unistd.h>


double total_mass;
double ship_weight = 1000.0;
double force_one;
double max_velocity = 110.0;
double burn_rate;
double v_nozz = 2000.0;

int main() {
    std::cout << "Hello world\n";
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
            total_mass = ship_weight + rocket->fuel_weight;
            force_one = total_mass * 110.0;
            burn_rate = force_one / v_nozz;
            rocket->fuel_weight = burn_rate * delta_t;
            std::cout << "Burn phase: \n";
            std::cout << rocket->fuel_weight;
            std::cout << "\n";
            // loop shit
            /*setBurnRate(*rocket);*/
            sleep((unsigned int)0.1);
            rocket->time += 1;
        }
        std::cout << "Out of burn phase: \n";
        setBurnRate(*rocket);
        sleep((unsigned int)0.1);
        rocket->time += 1;
    }

    return 0;
}


double setBurnRate(ship_state_type ss) {

    return 0; // Obviously, not the correct answer ;-)
}
