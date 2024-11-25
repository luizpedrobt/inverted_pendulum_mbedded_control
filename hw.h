#include "esp32-hal-gpio.h"
#include "hw_sensors_read.h"

#define USR_SWT 0
#define IN1 4                      // Ponte H - IN1           
#define IN2 16                      // Ponte H - IN2      
#define motor 15                    // Ponte H - ENA
#define ENCODER_A 18
#define ENCODER_B 19

void hw_setup() 
{
  Serial.begin(230400);

  pinMode(USR_SWT, INPUT);
  pinMode(motor, OUTPUT);          
  pinMode(IN1, OUTPUT);                  
  pinMode(IN2, OUTPUT);  

  Wire.begin();
  mpu.initialize();

  hw_calibrate_GyroY();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 não conectado!");
    while (1);
  }

  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder.attachHalfQuad(ENCODER_B, ENCODER_A);        // Encoder B, encoder A
  encoder.setCount(0);
}

void hw_horario()
{
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
}

void hw_anti_horario()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
}

void hw_pwm_motor(float u)
{
  analogWrite(motor,u*255.0/100.0);
}

void hw_print_states(float states[4])
{
    Serial.print(states[0]);
    Serial.print(" cm ");
    Serial.print(states[1],8);
    Serial.print(" cm/s ");
    Serial.print(states[2]);
    Serial.print(" ° ");
    Serial.print(states[3],8);
    Serial.print(" °/s ");
    Serial.println(u);
}