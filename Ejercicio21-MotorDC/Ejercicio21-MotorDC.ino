//Ejercicio basado en los ejercicios 9 y 10 del libro Arduino Starter Kit
#define SEGURIDAD 1

//const int switchPin = 10;
const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int directionSwitchPin = 4;
const int onOffSwitchStateSwitchPin = 5;
const int potPin = A0;
const int emergenciaPin = 11;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int directionSwitchState = 0;
int previousDirectionSwitchState = 0;

int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;

boolean cambiaEstado = 0;
boolean cambiaDireccion = 0;

//const int motorPin = 11;
//int switchState = 0;
//encendido_apagado = 0;

void setup(){
  Serial.begin(9600);
  //pinMode(motorPin, OUTPUT);
  //pinMode(switchPin, INPUT);
  pinMode(directionSwitchPin, INPUT);
  pinMode(onOffSwitchStateSwitchPin, INPUT);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(emergenciaPin, INPUT_PULLUP);
  digitalWrite(enablePin, LOW);
}

void loop(){
  // leo el boton de encendido/apagado del pin 2
  /*switchState = digitalRead(switchPin);
   if ((switchState == HIGH)&& !encendido_apagado){	//Si esta el motor apagado y leo un pulso
   	encendido_apagado = 1;
   	digitalWrite(motorPin, HIGH);
   }
   if (switchState == HIGH)&& encendido_apagado){	//Si esta el motor encendido y leo un pulso
   	encendido_apagado = 0;
   	digitalWrite(motorPin, LOW);
   }
   */
   
  if (digitalRead(emergenciaPin) == HIGH){
    onOffSwitchState = digitalRead(onOffSwitchStateSwitchPin);
    //Serial.println(onOffSwitchState);
    delay(1);
    directionSwitchState = digitalRead(directionSwitchPin);
    motorSpeed = analogRead(potPin)/4;
    //Serial.println(motorSpeed);

    if (onOffSwitchState != previousOnOffSwitchState){
      if (onOffSwitchState == HIGH){
        motorEnabled = !motorEnabled;
		cambiaEstado = 1;
		Serial.println("boton arranco....");
      }
    }

    if (directionSwitchState != previousDirectionSwitchState) {
      if ((directionSwitchState == HIGH)&& motorEnabled){
        motorDirection = !motorDirection;
		cambiaDireccion = 1;
		Serial.println("boton cambio direccion...");
      }
    }

    if ((motorDirection == 1)&& (cambiaDireccion) && motorEnabled){
#if SEGURIDAD
	cambiaDireccion = 0;
	Serial.println("cambio dir parada....");
      //Parada en rampa
      for (int i = motorSpeed; i > 0; i--){
        analogWrite(enablePin,i);
        delay(5);
      }
#endif
      digitalWrite(controlPin1, HIGH);
      digitalWrite(controlPin2, LOW);
#if SEGURIDAD
      //Arranque en rampa
	  Serial.println("cambio dir arranca....");
      for (int i = 0; i < motorSpeed; i++){
        analogWrite(enablePin,i);
        delay(5);
      }
#endif   
    }
    else if ((motorDirection == 0)&& (cambiaDireccion) && motorEnabled) {
#if SEGURIDAD
	cambiaDireccion = 0;
	Serial.println("cambio dir parada....");
      //Parada en rampa
      for (int i = motorSpeed; i > 0; i--){
        analogWrite(enablePin,i);
        Serial.println(i);
        delay(5);
      }
#endif 
      digitalWrite(controlPin1,LOW);
      digitalWrite(controlPin2,HIGH);
#if SEGURIDAD
	Serial.println("cambio dir arranca....");
      //Arranque en rampa
      for (int i = 0; i < motorSpeed; i++){
        analogWrite(enablePin,i);
        Serial.println(i);
        delay(5);
      }
#endif    
    }

    if ((motorEnabled == 1) && cambiaEstado){
	cambiaEstado = 0;
#if SEGURIDAD
	Serial.println("arranca....");
      //Arranque en rampa
      for (int i = 0; i < motorSpeed; i++){
        analogWrite(enablePin,i);
        Serial.println(i);
        delay(5);
      }
#endif    
    }
    else if ((motorEnabled == 0) && cambiaEstado){ 
	cambiaEstado = 0;
#if SEGURIDAD
Serial.println("parada....");
      //Parada en rampa
      for (int i = motorSpeed; i > 0; i--){
        analogWrite(enablePin,i);
        Serial.println(i);
        delay(5);
      }
#endif
    }

    previousDirectionSwitchState = directionSwitchState;
    previousOnOffSwitchState = onOffSwitchState;
  }
  else {
    Serial.println("Parada Emergencia");
    analogWrite(enablePin,0);
  }
}
