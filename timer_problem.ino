/*
   Takes PWM input from pin 2 and 3, writes the same PWM output to pin 10 and 11 respectively << triggered every 20000us (to maintain 50Hz Refresh Rate)
   Led on pin changes its state every 100000us (10Hz Refresh Rate)
   if input is 0 for more than 2seconds, respective output is set to 0
*/

#define pin_input_1 2
#define pin_input_2 3

#define pin_led 13

#define pin_output_1 10
#define pin_output_2 11


uint32_t time_loop;
uint32_t pwm_in[2];
uint32_t pwm_out[2];

volatile uint32_t _pwm1, _pwm2, ap_time, bp_time, _dead1, _dead2;

boolean state;
short count;
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
}
void loop()
{
  /* 50Hz Looptime */
  if ((micros() - time_loop) > 20000)
  {
    pwm_in[0] = _pwm1;
    pwm_in[1] = _pwm2;

    pwm_out[0] = (pwm_in[0] > 20000) || (pwm_in[0] < 900) ? 900 : pwm_in[0]; // Limit the pulse width in the range between 0.9ms to 2ms
    pwm_out[1] = (pwm_in[1] > 20000) || (pwm_in[1] < 900) ? 900 : pwm_in[1]; // Limit the pulse width in the range between 0.9ms to 2ms

    /* Write output to pins*/
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
      digitalWrite(pin_led, state); //  Blinks LED at 10HZ
    }
  } ///////////50HZ loop Ends Here//////////////

  // Anything Outside 50Hz loop
  else
  {
    if (digitalRead(pin_input_1))
      _dead1 = micros();
    else if ((micros() - _dead1) > 2000000) // if input is dead for more than 2 seconds write 0.9ms to output
      _pwm1 = 900;

    if (digitalRead(pin_input_2))
      _dead2 = micros();
    else if ((micros() - _dead2) > 2000000)// if input is dead for more than 2 seconds write 0.9ms to output
      _pwm2 = 900;
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
