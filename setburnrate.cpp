#include "lander.h"
#include <stdint.h>
#include<math.h>

/**
 * BRANCH: upg_challencode
 *
 * Plans:
 * 1. Updating the end of all functions to include T1 += 1. Assuming T1 is the time slice.
 *
 * Questions:
 * 1. What do the T1, T2, and flag variables represent?
 *
 * Problems:
 * 1. Acceleration peaks at time slice #1 w/ value of 61 ,
 * decreases thereafter, and becomes a negative number shortly after.
 */
/*Set the shipweight explicitly since we can't acces the .ino file*/

double mass_rocket = ship_weight; /* Measured in kgs */
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
double range = hover_range; /* measured in meters */
double top_of_range = final_height + range;
double bottom_of_range = final_height - range;
/* Originally initialized as bottom_of_range + hover_range but that is not defined*/
double target_height = top_of_range;
double height_cutoff = target_height - 200;
double velocity_max = sqrt(4000);
const double TIME_SLICE = 1;
double the_gravity = 10.0;


/**
 * What do the integer values assigned to this variable
 * throughout the program represent?
 *
 * Do they represent the run state constants defined in lander.h? No
 *
 * Numbers assigned to flag throughout program: 0 (initialization), 2, 3
 */
int flag = 0;
/* Most likely time keeper.
 * TIME_SLICE should NOT be modified*/
double T1 = 0;
double T2 = 0;
// access altitude from ship w/o passing in ship as variable
// try and access it with global access

double take_off() 
{
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
    mass_total = mass_total - (burn_rate * TIME_SLICE);
    acceleration = (force_1 / mass_total) - 10;
    altitude = altitude + (initial_velocity * TIME_SLICE) + (0.5 * acceleration * TIME_SLICE * TIME_SLICE);
    final_velocity = initial_velocity + (acceleration * TIME_SLICE);
    initial_velocity = final_velocity;
    change_in_altitude_since_last_TIME_SLICE = height_cutoff - altitude;
    T1 += 1;

    return burn_rate;
}

double take_us_to_goal() 
{
    if (initial_velocity >= velocity_max) 
    {
        //
        force_2 = 0.0;
    } 
    else 
    {
        force_2 = (((velocity_max * velocity_max) - (initial_velocity * initial_velocity)) * mass_total) /
                ( 2.0 * change_in_altitude_since_last_TIME_SLICE);
        force_1 = mass_total * 109;
    }

    /**
     * Known issue here.
     * Changed smaller than symbol (<) for greater than symbol (>)
     */
    if (force_1 > force_2) {
        burn_rate = (force_1 / nozzle_velocity);
        mass_total = mass_total - (burn_rate * TIME_SLICE);
        acceleration = (force_1 / mass_total) - the_gravity;
    } else {
        burn_rate = force_2 / nozzle_velocity;
        mass_total = mass_total - (burn_rate * TIME_SLICE);
        acceleration = (force_2 / mass_total) - the_gravity;
    }

    altitude = altitude + (initial_velocity * TIME_SLICE) + (0.5 * acceleration * TIME_SLICE * TIME_SLICE);
    final_velocity = initial_velocity + (acceleration * TIME_SLICE);
    initial_velocity = final_velocity;
    change_in_altitude_since_last_TIME_SLICE = height_cutoff - altitude;


    /**
     * TRACK: flag
     * PROBLEM: highly likely that altitude will never == top of range.
     * Thus, flag is never set to 2.
     */
    if (altitude == top_of_range){
        flag = 2;
    }

    T1 += 1;


    return burn_rate;
}


/**
 * Broken because of conditional on line 114.
 * @return
 */
double hover() {

    /**
     * What is flag == 2 symbolize
     */
  while (flag == 2) {
 //     double T2 = 0.0; // Time I initialized up top
  
      while (altitude == bottom_of_range) {
            change_in_altitude_since_last_TIME_SLICE = top_of_range - bottom_of_range;
            final_velocity = sqrt(2.0 * the_gravity * change_in_altitude_since_last_TIME_SLICE);
            force_2 = (final_velocity * final_velocity * mass_total) / (2.0 * change_in_altitude_since_last_TIME_SLICE);
            force_1 = mass_total * 109;
            if (force_1 < force_2) {
                burn_rate = force_1 / nozzle_velocity;
                mass_total = mass_total - (burn_rate * TIME_SLICE);
                acceleration = (force_1 / mass_total) - 10;
            } else {
                burn_rate = force_2 / nozzle_velocity;
                mass_total = mass_total - (burn_rate * TIME_SLICE);
                acceleration = (force_2 / mass_total) - 10;
            }
            altitude = altitude + (initial_velocity * TIME_SLICE) + (0.5 * acceleration * TIME_SLICE * TIME_SLICE);

            final_velocity = initial_velocity + (acceleration * TIME_SLICE);
            initial_velocity = final_velocity;

            
             if (T1 == 5) {
                 flag = 3;
             }
        }

        T2 += 1;
  }
    return burn_rate;
}

double landing() {
    while (flag == 3) {
        altitude = altitude + (initial_velocity * TIME_SLICE) + (0.5 * (-the_gravity) * TIME_SLICE * TIME_SLICE);
        final_velocity = initial_velocity + (-the_gravity * TIME_SLICE);
        initial_velocity = final_velocity;

        if (altitude > 100 && altitude < 200) {
            force_2 = (((-3.0 * -3.0) - (initial_velocity * initial_velocity) * mass_total) / (2.0 * altitude));
            force_1 = mass_total * 109;

            if (force_1 > force_2) {
                burn_rate = (force_1 / nozzle_velocity);
                mass_total = mass_total - (burn_rate * TIME_SLICE);
                acceleration = (force_1 / mass_total) - the_gravity;
            } else {
                burn_rate = (force_2 / nozzle_velocity);
                mass_total = mass_total - (burn_rate * TIME_SLICE);
                acceleration = (force_2 / mass_total) - the_gravity;
            }
            altitude = altitude + (initial_velocity * TIME_SLICE) + (0.5 * acceleration * TIME_SLICE * TIME_SLICE);
            final_velocity = initial_velocity + (acceleration * TIME_SLICE);
            initial_velocity = final_velocity;
        }

    }
    return burn_rate;
}

/*
 * Use a stack initialized within this stack that pushes the initial altitude onto it, and
 * then pops it into the parameter of this function along with the final_height in order to return the change
 * in altitude
 * double calculate_change_in_altitude(double initial_altitude_at_beginning_of_TIME_SLICE, double final_height) {
 *
 *  return final_height - initial_altitude_at_beginning_of_TIME_SLICE;
 * }
 * */


/**
 * Never gets used. Consider deleting.
 * @param the_initial_velocity_at_beginning_of_TIME_SLICE
 * @param the_final_velocity
 * @return
 */
double calculate_change_in_velocity(double the_initial_velocity_at_beginning_of_TIME_SLICE, double the_final_velocity) {
    return the_final_velocity - the_initial_velocity_at_beginning_of_TIME_SLICE;
}

double setBurnRate(ship_state_type ss) {
   double ze_burn_rate = 0.0;

  // blast off for 5 seconds
  if (T1 < 5) {
        ze_burn_rate = take_off();
  } else if (T1 >= 5) {
        ze_burn_rate = take_us_to_goal();
  } 
    return ze_burn_rate;
}


