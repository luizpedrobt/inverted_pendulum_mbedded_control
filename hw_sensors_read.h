#include <Arduino.h>
#include <ESP32Encoder.h>
#include <Wire.h>
#include <MPU6050.h>

#define sample_time 20000
#define sample_time_sec (sample_time)/1000000

ESP32Encoder encoder;
MPU6050 mpu;

unsigned long tempo_ant = 0, tempo_atual = 0, dT = 0;

// Variáveis encoder
float theta = 0.0, u = 0.0;
long contEnc = 0.0;

float pos = 0.0;
float pos_1 = 0.0;
float ang_pos = 0.0;
float ang_pos_1 = 0.0;
float vel_lin = 0.0;
float vel_ang = 0.0;

float angleX = 0, angleY = 0, angleZ = 0;
float gyroX, gyroY, gyroZ, accelX, accelY, accelZ;
float gyroOffsetY = 0;

int cont = 0;

float states[4] = {pos, vel_lin, ang_pos, vel_ang}; 

unsigned long previousTime;

void get_pos_lin() 
{
  contEnc = encoder.getCount();
  theta = contEnc*2.0*3.14/(334.0*2.0);
  pos_1 = pos;
  pos = 3.65*theta*2*PI*6.5e-1;
}

void get_ang()
{
  unsigned long currentTime = millis();
  float deltaTime = (tempo_atual - tempo_ant) / 1000000.0; // Em segundos
  tempo_ant = tempo_atual;

  // Ler dados do sensor
  accelX = mpu.getAccelerationX() / 16384.0; // Escala padrão ±2g
  accelY = mpu.getAccelerationY() / 16384.0;
  accelZ = mpu.getAccelerationZ() / 16384.0;
  gyroX = mpu.getRotationX() / 131.0; // Escala padrão ±250°/s
  gyroY = mpu.getRotationY() / 131.0;
  gyroZ = mpu.getRotationZ() / 131.0;

  // Calcular ângulos do acelerômetro
  float accelAngleX = atan2(accelY, sqrt(accelX * accelX + accelZ * accelZ)) * 180 / PI;
  float accelAngleY = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / PI;

  // Atualizar ângulos com filtro complementar
  angleX = 0.9 * (angleX + gyroX * deltaTime) + 0.1 * accelAngleX;
  angleY = 0.9 * (angleY + gyroY * deltaTime) + 0.1 * accelAngleY;
  
  angleZ += gyroZ * deltaTime;
  ang_pos_1 = ang_pos;
  ang_pos = angleX+80.0; 
}

void hw_calibrate_GyroY() 
{
  float sum = 0;
  int numSamples = 200;

  Serial.println("Calibrando giroscópio no eixo Y...");
  for (int i = 0; i < numSamples; i++) {
    sum += mpu.getRotationY() / 131.0;
    delay(10);
  }
  gyroOffsetY = sum / numSamples;
  Serial.print("Offset do giroscópio em Z: ");
  Serial.println(gyroOffsetY);
}

float get_vel_lin(float pos, float pos_1)
{
  return ((pos-pos_1)/sample_time_sec);
}

float get_vel_ang(float ang_pos, float ang_pos_1)
{
  return ((ang_pos-ang_pos_1)/sample_time_sec);
}