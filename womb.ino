// NOTES
/*
Author: doncato
Repo: https://github.com/chicken-womb/

This Project was initiated by a Friend of mine, needing a breeder for
chicken eggs. This is the arduino script controlling the breeder

All units are in °C!
*/

// IMPORTS
#include <math.h>
// - Imports for Display
#include <Wire.h>
#include "rgb_lcd.h"
// - Imports for temperature Sensor
#include "Adafruit_MCP9808.h"
// - Imports for Humidity Sensor
#include "DHT.h"

// SETTINGS
// If you plan on running this on your own these are some settings you might want to change
// - Misc
const char VERSION[4] = "v1.0"; // Version Number; Should only be 4 characters (e.g. "v0.1")
long motor_time = 3; // The time in minutes when the motor should be activated
int motor_turn_time = 12; // The time in seconds for the motor to be active CANNOT BE BIGGER THAN 32
const int dim_after = 10; // Dim the display after n iterations
const int iteration_delay = 250; // The time in milliseconds to wait between iterations of the main loop, change only if you know what you are doing

// - Temperature Settings
float target_temp = 37.5; // Default target temperature
const float target_temp_variation[2] = {0.3, 0}; // The allowed variation between measured and target temperature downwards/upwards before heater will be activated/deactivated
const float max_temp_range[2] = {36.0, 39.0}; // Min-Max temperature Range to be allowed as target temperature
const float max_sens_range[2] = {15.0, 45.0}; // Min-Max temperature Range to be allowed to be measured, values beyond this range will disable almost everything
// ATTENTION: THE MAX SENS RANGE SHOULD BE OUTSIDE OF THE MAX TEMP RANGE!
// I do not know how the script reacts otherwise, I'm too tired thinking about it.

// - Pin Numbers for some devices
const int button_null = 2; // WIP UNUSED; Pin for the Button with the NULL function
const int button_up = 4; // Pin for the Button to raise the target temperature
const int button_down = 3; // Pin for the Button to lower the target temperature
const int button_mode = 2; // WIP UNUSED; Pin for the Button with the Mode function

const int relay_heat = 9; // Pin for the Relay controlling the heat
const int relay_air = 10; // WIP UNUSED; Pin for the Relay controlling additional air vents and similar
const int relay_motor = 11; // Pin for the Relay controlling the rotational motor (for turning the eggs)
const int relay_4 = 12; // WIP UNUSED; Additional Pin

Adafruit_MCP9808 tempsensor_1 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor_2 = Adafruit_MCP9808();
const Adafruit_MCP9808 tempsensors[] = {tempsensor_1, tempsensor_2};
const int temp_sensor_addrs[] = {0x18, 0x19}; // Array of the addresses for the Adafruit_MCP9808 Temp sensors

// END OF SETTINGS

// Define modules
// - Define lcd display
rgb_lcd lcd;
// - Set the pin for readout of the Humidity Sensor
#define DHTPIN 6
#define DHTTYPE DHT22   // DHT 22  (AM2302) (Model Number)
DHT humisensor(DHTPIN, DHTTYPE);
// - Set the buttons pins
const int buttons[4] = {button_null, button_up, button_down, button_mode};
// - Set the relay pins
const int relais[4] = {relay_heat, relay_air, relay_motor, relay_4};

// Variables for the current state, temperature and stuff
long last_time = 0;
bool motor_on = false;
bool motor_setup = false;
long turn_motor_off = 0;
bool shouldheat = false;
float curr_humi = 0.0;
float curr_temp = 0.0;
int empty_iterations = 0; 
bool it_switch = false; // Is true every second iteration

// The Setup function
void setup() {
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Initializing");
  // Set starting Text for display
  lcd.begin(16, 2);
  lcd.setRGB(128,128,128);
  lcd.setCursor(0,0);
  lcd.print("womb        " + String(VERSION));
  lcd.setCursor(0,1);
  lcd.print("Setup");
  // Setup the relay pins for output
  for (int i=0; i<sizeof relais/sizeof relais[0]; i++) {
    pinMode(relais[i], OUTPUT);
  }
  // Setup the button pins for input
  for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
    pinMode(buttons[i], INPUT);
  }
  // Setup the sensors
  // Set temperature Sensors
  for (int i=0; i<sizeof temp_sensor_addrs/sizeof temp_sensor_addrs[0]; i++) {
    //Adafruit_MCP9808 sensor = tempsensors[i];
    // Get the sensor with the specified address, if it wasn't found don't save it
    if (tempsensors[i].begin(temp_sensor_addrs[i])) {
      tempsensors[i].setResolution(3);
      tempsensors[i].wake();
      //Serial.println(sensor.readTempC());
    }
  }
  /*
  tempsensor.begin(0x19);
  tempsensor.setResolution(3);
  tempsensor.wake();
  */
  // Set Humidity Sensor
  humisensor.begin();

  lcd.setCursor(0,1);
  lcd.print("Author: doncato");
  delay(500);
}

// The main loop
void loop() {
  // Define vars in current iteration
  // - Define whether or not an action was done by the user
  bool actioned = false;
  // - Define an array to hold the values of all buttons as a boolean
  bool button_values[sizeof buttons/sizeof buttons[0]];
  // Poll all Sensors
  // - Get the value of the temperature sensor
  curr_temp = computeCurrTemp();
  // - Get the value of the humidity sensor
  curr_humi = humisensor.readHumidity();

  // Get the value of all buttons
  for (int i=0; i<sizeof buttons/sizeof buttons[0]; i++) {
    button_values[i] = digitalRead(buttons[i]) == HIGH;
  }

  // Execute appropriate function to the button press
  if (button_values[3]) {
    actioned = true;
    motor_setup = true;
    digitalWrite(relay_motor, HIGH);
  }
  else if (motor_setup){
    motor_setup = false;
    digitalWrite(relay_motor, LOW);
  }
  if (button_values[0]) {
    actioned = true;
    buttonNull();
  }
  else if (button_values[1]) {
    actioned = true;
    target_temp += 0.1;
  }
  else if (button_values[2]) {
    actioned = true;
    target_temp -= 0.1;
  }

  // Correct the target temperature if it is out of max_temp_range
  correctTargetTemp();

  // Check if the measured temperature is outside max_sens_range
  if (invalidTemp()) {
    if (it_switch) {
      lcd.setRGB(255,0,0);
    }
    else {
      lcd.setRGB(0,0,255);
    }
  }
  // Check if the temperature is outside max_temp_range and blink the screen red if it is
  else if (alarmingTemp()) {
    if (it_switch) {
      lcd.setRGB(255,0,0);
    }
    else {
      lcd.setRGB(0,0,0);
    }
  }
  // If the temperature is not alarming, dim the screen after inactivity
  else {
    if (!actioned) {
      empty_iterations += 1;
    }
    if (empty_iterations >= dim_after) {
      lcd.setRGB(0,0,0);
      empty_iterations = dim_after; // Just to prevent empty_iterations from getting too big and causing an overflow
    }
    if (actioned) {
      empty_iterations = 0;
      lcd.setRGB(128,128,128);
    }
  }
  // Only at this point the curr_temp variable should be used for any real-life action

  shouldheat = heaterShouldHeat();
  if (shouldheat) {
    digitalWrite(relay_heat, HIGH);
  }
  else {
    digitalWrite(relay_heat, LOW);
  }

  // Check if it is time to turn the motor on
  if (turnMotor()) {
    Serial.println("MOTORTIME! ON");
    digitalWrite(relay_motor, HIGH);
    motor_on = true;
    turn_motor_off = millis() + (motor_turn_time*1000);
  }
  // Check if it is time to turn the motor off
  if (motor_on && millis() >= turn_motor_off) {
    Serial.println("MOTORTIME! OFF");
    digitalWrite(relay_motor, LOW);
    motor_on = false;
  }
 
  // Do some stuff at the end 
  it_switch = !it_switch;
  // Print the current data to display
  printState();
  delay(iteration_delay);
}


void buttonNull() {
  Serial.println("NULL FUNCTION"); // More a placeholder
}

void correctTargetTemp() {
  if (target_temp >= max_temp_range[1]) {
    target_temp = max_temp_range[1];
  }
  else if (target_temp <= max_temp_range[0]) {
    target_temp = max_temp_range[0];
  }
}

float computeCurrTemp() {
  // TODO: Ignore sensor if value is unrealistic, but how to determine that?
  float sum = 0.0;
  //Serial.println(temp_sensors[0].readTempC());
  for (int i=0; i<sizeof tempsensors/sizeof tempsensors[0]; i++) {
    Adafruit_MCP9808 sensor = tempsensors[i];
    float temp = sensor.readTempC();
    if (!isnan(temp)) {
      sum += temp;
    }
  }
  //Serial.println(sizeof tempsensors);
  float result = (sum / (sizeof tempsensors/sizeof tempsensors[0]));
  if (!isnan(result)) {
    return result;
  }
  else {
    return 0.0;
  }
}

bool heaterShouldHeat() {
  // If the measured temperature is under the target temperature - variation, heat
  if (curr_temp <= target_temp - target_temp_variation[0]) {
    return true;
  }
  // If the measured temperature is over the target temperature + variation, don't heat
  else if (curr_temp >= target_temp + target_temp_variation[1]) {
    return false;
  }
  // Else keep the previous state to save relay cycles, I guess?
  return shouldheat;
}

bool alarmingTemp() {
  if (curr_temp >= max_temp_range[1]) {
    return true;
  }
  else if (curr_temp <= max_temp_range[0]) {
    return true;
  }
  return false;
}

bool invalidTemp() {
  if (curr_temp >= max_sens_range[1]) {
    return true;
  }
  else if (curr_temp <= max_sens_range[0]) {
    //curr_temp = max_sens_range[1] + 1; // Set the temperature to something high so the heater gets disabled
    return true;
  }
  return false;
}

bool turnMotor() {
  long now = millis();
  long diff = now - last_time;
  if (diff < 0) { //if the millis() timer had an int overflow
    diff = now;
    last_time = 0;
  }
  if (diff >= (motor_time * 60 * 1000)) {
    last_time = now;
    return true;
  }
  return false;
}

void printState() {
  char indicator = ' ';
  if (shouldheat) {
    indicator = '*';
  }
  int remaining_timer = ((motor_time * 1000 * 60) - (millis() - last_time)) / 1000 / 60;
  char r[4];
  sprintf(r, "%4d", remaining_timer);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("C: " + String(curr_temp, 2) + "   " + String(curr_humi, 1) + "%");
  lcd.setCursor(0,1);
  lcd.print("T: " + String(target_temp, 2) + " " + indicator + "  " + String(r));
}
