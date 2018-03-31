#include "lander.h"
#include <math.h>
#include <stdio.h>
/* Figure out how to calculate how far the rocket has traveled every .1 seconds */
/* Set a range for the rocket to reach */


/* Use a suicide burn in order to save the most fuel */
/* Figure out what ISP is */
/* Figure out how to calculate exiting exhaust */
/* Figure out how to recalculate the mass of the object after propellant has left the exhaust */


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
    double mass_initial = (mass_payload + mass_propellant + mass_structure);
    double mass_final = (mass_payload + mass_structure);

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

    /* Logarithm of the initial mass / final mass */
    double log_of_mi_divided_by_mf = log(mass_initial / mass_final);

    double delta_v = (nominal_gravitational_acceleration * isp * log_of_mi_divided_by_mf);

    /*  double log(mass_initial/mass_final);*/

    /* The seconds squared cancels out because of isp * nominal gravitational acceleration */
    printf("%s %f meters/second \n", "Delta V: ", delta_v);

    return 0;
}
