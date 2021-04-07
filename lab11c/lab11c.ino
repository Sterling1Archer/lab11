#include <Stepper.h> 

int numberofstep = 48*64; 
                                  

Stepper motor(numberofstep,9,11,10,6);    // we use pins 9, 11, 10, 6        
void setup() 
{ 
 
  motor.setSpeed(8.5); // we set motor speed at 9
}
void loop() 
{ 
  motor.step(600); // the motor will run during 600 step (you can change the step)
  motor.step(-800); // the motor will run during 800 step in reverse (you can change the step)

}
