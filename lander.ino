#include <stdio.h>
#include <ArduinoJson.h>
#include "lander.h"

#define rows_per_report   20

void initShipState(ship_state_type *ss);
void showValues(ship_state_type *ss);
void update(ship_state_type *ss);
void runSimulation(ship_state_type *ss);
double calcScore(ship_state_type *ss);

double  ship_weight = 0.0;
double  hover_altitude = 0.0;
double  hover_range = 0.0;

// For test purposes: {"ship_weight":534.0,"hover_altitude":1205.4,"hover_range":2.3}

static ship_state_type ss;

void setup() {
  // put your setup code here, to run once:
  String json;
  bool setupSuccess = false;

  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial) continue;

  StaticJsonBuffer<200> jsonBuffer;

  while (!setupSuccess) {
    digitalWrite(PIN_LED1, HIGH);

    while (!Serial.available());
    json = Serial.readString();
    digitalWrite(PIN_LED1, LOW);
    digitalWrite(PIN_LED2, HIGH);

    JsonObject& root = jsonBuffer.parseObject(json);

    if (root.success()) {
      digitalWrite(PIN_LED2, LOW);
      digitalWrite(PIN_LED3, HIGH);
      setupSuccess = true;
      ship_weight = root["ship_weight"];
      hover_altitude = root["hover_altitude"];
      hover_range = root["hover_range"];
    }
  }

  initShipState(&ss);

  Serial.println("Initial state:");

  showValues(&ss);
}

static int i = rows_per_report;

void loop() {

  if (ss.run_state <= run_still_active)
  {
    i++;

    if(i >= rows_per_report)
    {
      Serial.println("");
      Serial.println("Time       Fuel       Accel      Velocity   Altitude   Burn       Hovered?");
      Serial.println("---------- ---------- ---------- ---------- ---------- ---------- --------");
      
      i = 0;
    }

    update(&ss);

    showValues(&ss);
  }
  delay(100);
}

void initShipState(ship_state_type *ss) {
  ss->fuel_weight         = initial_fuel_weight;
  ss->acceleration        =    0.0; 
  ss->velocity            =    0.0; 
  ss->altitude            =    0.0;

  ss->time                =    0; 
  ss->hover_count         =    0;
  ss->hover_accomplished  =    false;

  ss->run_state           = run_pre_launch;
}

/* -------------------------------------------------------------
*/

void showValues(ship_state_type *ss)
{
  char ssBuffer[200];
  int ledLevel;
  sprintf(ssBuffer, "%10d %10.3f %10.3f %10.3f %10.3f %10.3f     %d",
    ss->time,ss->fuel_weight,ss->acceleration,ss->velocity,
    ss->altitude,ss->prior_burn_rate,ss->hover_accomplished);
  Serial.println(ssBuffer);
  switch (ss->run_state) {
    case run_pre_launch:
    case run_flying:
      ledLevel = 255 - int(floor(ss->prior_burn_rate)*3);
      analogWrite(PIN_LED_R, ledLevel);
      analogWrite(PIN_LED_G, ledLevel);
      analogWrite(PIN_LED_B, ledLevel);
      break;
    case run_landed:
      analogWrite(PIN_LED_G, 0);
      analogWrite(PIN_LED_R, 255);
      analogWrite(PIN_LED_B, 255);
      break;
    default:
      analogWrite(PIN_LED_R, 0);
      analogWrite(PIN_LED_G, 255);
      analogWrite(PIN_LED_B, 255);
      break;
  }
}

/* -------------------------------------------------------------
*/

void update(ship_state_type *ss)
{
  double fuel_consumption;   
  double v_zero, burn_rate;
  time_t start_time;

  /* Grab the system time so we know how long the contestant's code 
     takes to return.  */
  start_time = _CP0_GET_COUNT();

  /* Call the contestant's code. */
  burn_rate = setBurnRate(*ss);
  
  /* Check for and terminate on an illegal value. */
  if(burn_rate < 0.0)
  {
    ss->run_state = run_illegal_burn_rate;
    return;
  }

  /* See if the contestant's code took too long. */
  if(_CP0_GET_COUNT() - start_time > max_time_per_turn)
  {
    ss->run_state = run_exceeded_time;
    return;
  }

  /* Remember the velocity at the start of the time slice */
  v_zero = ss->velocity;

  /* Calculate the amount of fuel we'll burn in this time slice */
  fuel_consumption = burn_rate * delta_t;

   /* Can't burn more fuel in this slice than we have, so we
      clip the value, if needed. */
  if(fuel_consumption > ss->fuel_weight)
    fuel_consumption = ss->fuel_weight;

  /* No fuel, so our burn rate has to be 0.0 */
  if(fuel_consumption <= 0.0)
    burn_rate = 0.0;
  else
    /* Ensure that the burn rate and consumption agree, just in
       case the consumption was clipped above. */
    burn_rate = fuel_consumption / delta_t;

  /* a = F/m - g */
  ss->acceleration = ((nozzle_velocity * burn_rate) / 
    (ship_weight + ss->fuel_weight)) - gravity;

  /* If our new accel. is negative and we haven't yet taken
     off, force our acceleration to 0.0 to keep the ship from
     mysteriously sinking into the ground. */
  if((ss->acceleration < 0.0) && (ss->run_state == run_pre_launch))
    ss->acceleration = 0.0;

  /* Check our acceleration limit.  We only have to worry about 
     the positive comparison, since negative acceleration can't 
     exceed -1 * g. */
  if(ss->acceleration > max_acceleration)
  {
    ss->run_state = run_exceeded_accel;
    return;
  }

  /* Current velocity = prior velocity + effects of acceleration */
  ss->velocity = v_zero + delta_t * ss->acceleration;

  /* New altitude = old altitude plus effects of old velocity and
     new acceleration */
  ss->altitude = ss->altitude + v_zero * delta_t + 0.5 * 
    ss->acceleration * delta_t * delta_t;

  /* If we're off the ground, we're flying */
  if(ss->altitude > 0.0)
    ss->run_state = run_flying;

  /* If we've launched and our altitude is now zero the run's over 
     and we have to determine whether we crashed or landed safely. 

     A safe landing has a velocity greater than max_landing_velocity,
     since that constant is a negative number. */
  if((ss->run_state == run_flying) && (ss->altitude <= 0.0))
  {
    if(ss->velocity >= max_landing_velocity)
      ss->run_state = run_landed;
    else
      ss->run_state = run_crashed;

    ss->altitude = 0.0;

//    return;
  }

  /* Check for hovering, but only if we know the maneuver has not
     yet been accomplished. */

  if(ss->hover_accomplished == 0)
  {
    if((ss->altitude >= (hover_altitude - hover_range)) && 
       (ss->altitude <= (hover_altitude + hover_range)))
      ss->hover_count++;
    else
      ss->hover_count = 0;

    if(ss->hover_count >= hover_count_goal)
      ss->hover_accomplished = 1;
  }
  
  /* Count this fuel consumption, and ensure we don't 
     somehow wind up with a negative fuel weight. */
  ss->fuel_weight -= fuel_consumption;

  if(ss->fuel_weight < 0.0)
    ss->fuel_weight = 0.0;

  /* Count this simulated time slice */
  ss->time++;

  /* If we haven't taken off within the first min_launch_time
     time slices, the launch failed and we terminate the run. */
  if((ss->time > min_launch_time) && 
     (ss->run_state == run_pre_launch))
  {
    ss->run_state = run_failed_launch;
    return;
  }

  ss->prior_burn_rate = burn_rate;
}

/* -------------------------------------------------------------
   If the run ended without a safe landing or without a success
   hover maneuver, then the score is 0.0.  

   Otherwise, the user gets a bonus for the amount of fuel 
   remaining and the difference between the constant 
   max_landing_velocity and the ship's velocity when it landed.  
   This difference is not limited to the absolute value of 
   max_landing_velocity, which makes it possible to get an even 
   higher bonus if the landing velocity is positive.
*/


double calcScore(ship_state_type *ss)
{
  if((ss->run_state != run_landed ) ||
     (ss->hover_accomplished == 0))
    return 0.0;
  else
    return (ss->fuel_weight * fuel_bonus) + 
           (fabs(max_landing_velocity - ss->velocity) * velocity_bonus);
}


