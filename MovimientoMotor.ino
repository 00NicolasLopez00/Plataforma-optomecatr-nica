 //Declare pin functions on RedBoard
#define STEP 2
#define DIR 3
#define STEP_DELAY 2
int  g_CurrDir;

void setup() {
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    String entrada = Serial.readString();
    char* mensaje = entrada.c_str();
    char simbolo;
    int pasos;
    if (sscanf(mensaje,"%c,%d", &simbolo,&pasos)==2){
      switch(simbolo) {
        case '1':
          StepForwardDefault(pasos);
          Serial.println(pasos);
          break;
        case '2':
          g_CurrDir = pasos & 1;
          digitalWrite(DIR,g_CurrDir);
          Serial.println(g_CurrDir);
          break;
        case '3':
          g_CurrDir ^= 1;
          digitalWrite(DIR,g_CurrDir);
          Serial.println(g_CurrDir);
          break;
      }
      
    }

  }
}

//Default microstep mode function
void StepForwardDefault(int steps)
{
  for(int x= 0; x<steps; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(STEP,HIGH); //Trigger one step forward
    delay(STEP_DELAY);
    digitalWrite(STEP,LOW); //Pull step pin low so it can be triggered again
    delay(STEP_DELAY);
  }
  Serial.println("Enter new option");
  Serial.println();
}
