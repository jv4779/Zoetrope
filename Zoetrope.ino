
#include <AFMotor.h>

AF_DCMotor led(3);
AF_Stepper motor(200,1);

#define RPM 60
#define FRAMES 12
#define STEPS_PER_REV 400
#define STROBE_ON_MICROS 800
#define STEPPER_DIRECTION BACKWARD

static const unsigned long micros_per_step = RPM * 1000UL * 1000UL / 60 / STEPS_PER_REV;
static const unsigned long micros_per_strobe = (1000UL * 1000UL) / FRAMES;

static unsigned long last_micros = 0;

void setup() {
  //Serial.begin(9600);
  //Serial.println("Zoetrope!");
  
  led.run(RELEASE);
  led.setSpeed(255);
  motor.release();
  
  last_micros = micros();
}

void loop() {
  static unsigned long step_counter = 0;
  static unsigned long strobe_counter = 0;
  static unsigned char strobe_on = 0;
  static unsigned long strobe_on_counter = 0;
  
  unsigned long this_micros = micros();
  
  unsigned long delta_micros = this_micros - last_micros;

  // stepper control  
  step_counter += delta_micros;
  
  if ( step_counter >= micros_per_step ) {
    motor.onestep( STEPPER_DIRECTION, INTERLEAVE );
    step_counter -= micros_per_step;
  }
  
  // strobe control
  strobe_counter += delta_micros;
  
  if ( strobe_counter >= micros_per_strobe ) {
    led.setSpeed(255);
    led.run(FORWARD);
    // turn off the strobe after some micros
    strobe_on = 1;
    strobe_on_counter = 0;
    
    strobe_counter -= micros_per_strobe; 
  }
  else if ( strobe_on ) {

    strobe_on_counter += delta_micros;
    
    if ( strobe_on_counter >= STROBE_ON_MICROS ) {
      // strobe is done
      led.run(RELEASE);
      strobe_on = 0;
    }
  }

  last_micros = this_micros;
}


