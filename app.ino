
#include "hw.h"
#include "app.h"

void setup()
{
  while(digitalRead(USR_SWT));
  hw_setup();
}

void loop()
{

  tempo_atual = micros();
  dT = tempo_atual - tempo_ant;

  if(dT >= sample_time) 
  {
    tempo_ant = tempo_atual;   
     
    get_pos_lin();
    get_ang();
    vel_lin = get_vel_lin(pos, pos_1);
    vel_ang = get_vel_ang(ang_pos, ang_pos_1);
    
    states[0] = pos;
    states[1] = vel_lin;
    states[2] = ang_pos;
    states[3] = vel_ang;

    u = app_controlador(K, states); 

    if (u >= 0){
      // sentido horário
      hw_horario();
    }

    else {
      //sentido anti-horário
      u = abs(u);
      hw_anti_horario();
    } 

    // Aplicando o controle na planta
    if(u >= 100) u = 100;
    hw_pwm_motor(u);
    
    hw_print_states(states);
  }
}
