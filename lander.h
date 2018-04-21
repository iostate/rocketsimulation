#ifndef __lander_h__
#define __lander_h__

/* The run_* constants are stored by the simulation program
   in the run_state field of the ship state. */

/* The ship hasn't left the ground yet. */
#define run_pre_launch        0

/* The ship has taken off */
#define run_flying            1

/* This should always be set to the largest of the "still
   active" run states above, so a simple comparison can
   be used to test for active or inactive states. */
#define run_still_active      1

#define run_landed            2
#define run_crashed           3
#define run_failed_launch     4
#define run_exceeded_accel    5
#define run_exceeded_time     6
#define run_illegal_burn_rate 7

/* -------------------------------------------------------
   The following constants WILL NOT change between the demo
   and final versions of the simulation program.  When your
   contest entry is run by LinuxProgramming.com these values
   will be exactly as shown here.

   See the files rules.txt and lander_details.txt for an
   explanation of what each constant is, its units of
   measurement, etc.
   ------------------------------------------------------- */

#define gravity              ((double)10.0)
#define max_acceleration     ((double)100.0)
#define min_launch_time      ((int)20)
#define max_time_per_turn    ((time_t)3000000)
#define delta_t              ((double)0.1)
#define initial_fuel_weight  ((double)1000.0)
#define max_landing_velocity ((double)-3.0)
#define nozzle_velocity      ((double)2000.0)
#define hover_count_goal     ((int)50)

/* These two constants are used by the calcScore() function
   in the simulation program to determine your score.  These
   numbers WILL NOT change in the final version of the code. */
#define fuel_bonus           ((double)10.0)
#define velocity_bonus       ((double)1000.0)

/* -------------------------------------------------------
   The following constants WILL change between the demo
   and final versions of the simulation program.  When your
   contest entry is run by LinuxProgramming.com these values
   will be replaced by randomly generated numbers.

   See the files rules.txt and lander_details.txt for an
   explanation of what each constant is, its units of
   measurement, as well as the range of values each constant
   can assume.
   ------------------------------------------------------- */

extern double ship_weight;
extern double hover_altitude;
extern double hover_range;

/* -------------------------------------------------------
   This is the structure that is passed to your setBurnRate()
   function when it is called at the beginning of each time
   slice.
   ------------------------------------------------------- */

typedef struct {
    double fuel_weight;      /* Fuel weight, kilograms      */
    double acceleration;     /* Upward acceleration, m/s**2 */
    double velocity;         /* Upward velocity, m/s        */
    double altitude;         /* Altitude, meters            */
    double prior_burn_rate;  /* Prior burn rate kg/s        */

    int time;                /* Time, in tenths of a second */
    int hover_count;         /* Turns at hover altitude     */
    int hover_accomplished;  /* Flag: hover finished?       */
    /* 0 == no, !0 == yes          */

    int run_state;           /* See run_* in this file      */
} ship_state_type;

extern double setBurnRate(ship_state_type ss);

#endif
