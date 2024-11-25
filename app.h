
float app_controlador(float K[4], float states[4]); 
void app_update_integrator(float y);

float ref = 0.0;
float integrador = 0.0;
float dt = 0.05;


void app_update_integrator(float y)
{
  float erro = ref - y; 
  integrador += erro * dt; 
}

float K[4] = {-12.7027, 32.7779, -8.8479, 5.5888};
float K_i = 4.4721;

float app_controlador(float K[], float states[]){

  app_update_integrator(states[0]);

  u = -(K[0] * states[0] + K[1] * states[1] + K[2] * states[2] + K[3] * states[3]);
  u -= K_i * integrador;
 
  return u;
}