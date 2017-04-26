/* Development Board : Teensy 3.2
   Freescale MK20D running at 72Mhz

*/
#include<Servo.h>

#define pin_input_1 2
#define pin_input_2 3

#define pin_led 13

#define pin_output_1 10
#define pin_output_2 11

Servo M1, M2;

uint32_t time_loop;
uint32_t pwm_in[2];
uint32_t pwm_out[2];
volatile uint32_t _pwm1, _pwm2;
boolean state;
void setup()
{
  pinMode(pin_input_1, INPUT);
  digitalWrite(pin_input_1, HIGH); // Pullup
  pinMode(pin_input_2, INPUT);
  digitalWrite(pin_input_2, HIGH); // Pullup

  attachInterrupt(pin_input_1, ISR_R1, RISING);
  attachInterrupt(pin_input_2, ISR_R2, RISING);

  pinMode(pin_output_1, OUTPUT);
  pinMode(pin_output_2, OUTPUT);

  pinMode(pin_led, OUTPUT);

  M1.attach(pin_output_2, 900, 2000)
  M2.attach(pin_output_2, 900, 2000)
}
void loop()
{
  ////////////50HZ/////////////////
  if ((micros() - time_loop) > 20000)
  {

    pwm_in[0] = pwm_1; //update values at 50 hz
    pwm_in[1] = pwm_2;

    pwm_out[0] = pwm_in[0] > 2000000 ? 900 : pwm_in[0]; // Limit the pulse width to 2 seconds
    pwm_out[1] = pwm_in[1] > 2000000 ? 900 : pwm_in[1];

    digitalWrite(pin_output_1, HIGH);
    digitalWrite(pin_output_2, HIGH);
    if (pwm_out[0] < pwm_out[1])
    {
      delayMicroseconds(pwm_out[0]);
      digitalWrite(pin_output_1, LOW);
      delayMicroseconds(pwm_out[1] - pwm_out[0]);
      digitalWrite(pin_output_2, LOW);
    }
    else
    {
      delayMicroseconds(pwm_out[1]);
      digitalWrite(pin_output_2, LOW);
      delayMicroseconds(pwm_out[0] - pwm_out[1]);
      digitalWrite(pin_output_1, LOW);
    }

    time_loop = micros();
    count++;
    if (count == 5)
    {
      count = 0;
      state != state;
      digitalWrite(pin_led, state); //Blinks LED at 10HZ
    }
  }
}
void ISR_R1() // Interrupt Subrutine for input A Rising
{
  attachInterrupt(pin_input_1, ISR_F1, FALLING);
  ap_time = micros();
}
void ISR_F1() // Interrupt Subrutine for input A Falling
{
  attachInterrupt(pin_input_1, ISR_R1, RISING);
  _pwm1 = micros() - ap_time;
}

void ISR_R2() // Interrupt Subrutine for input B Rising
{
  attachInterrupt(pin_input_2, ISR_F1, FALLING);
  bp_time = micros();
}
void ISR_F2() // Interrupt Subrutine for input B Falling
{
  attachInterrupt(pin_input_2, ISR_R1, RISING);
  _pwm2 = micros() - bp_time;
}
