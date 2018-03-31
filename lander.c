#include "lander.h"
#include <math.h>
#include <stdio.h>


int main() {
/*M = theoretical exhaust velocity = 5200 meters/second
real world exhaust velocity is roughly 85% of M or M * (1 + .85) = 4400 meters/second
# the above real world exhaust velocity is 4400 meters/second in a vacuum
 */


/* all mass variables use kilograms as unit of measurement*/
        double mass_payload = 100;
        double mass_propellant = 2000;
/* structure of the rocket weighs 300;*/
        double mass_structure = 300;
        double mass_initial = 2400;
        double final_mass_after_propellant_is_burnt = (mass_payload + mass_structure);

/*
 * nominal gravitational acceleration of an object in a vacuum near the surface of the Earth is 9.81 meters / seconds^(2)
 *
 * */
        double nominal_gravitational_acceleration = 9.81;
        double isp = 210;/* 210 seconds */

        /*
         * Calculation comes out incorrect
         * double delta_v = nominal_gravitational_acceleration * isp * log(mass_initial / final_mass_after_propellant_is_burnt);
         * */

    double delta_v = (nominal_gravitational_acceleration * isp);

    printf("%f", delta_v);


        return 0;
}
