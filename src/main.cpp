#include <Arduino.h>

byte pins [6] = {9, 10, 11, 12, 13, 19};
byte leds [6] = {2, 3, 4, 5, 6, 7};
#define btnStart  18
#define ledStart 8
#define tamanho 100
#define tempoLed 500

unsigned long tempoPisca;
bool sttLeds = false;
int HiScore = 1;


void gerarSequencia(byte sequencia []);
void ControleLeds(byte stt);
bool analizarSequencia(byte sequencia[], int pos);
void Game();
void mostrarSequenciar(byte sequencia [], int pos);
bool teclaPress(byte key);


void setup() {
  for(byte i = 0; i<6;i++){
    pinMode(pins[i], INPUT);
    pinMode(leds[i], OUTPUT);
  }

  pinMode(btnStart, INPUT);
  pinMode(ledStart, OUTPUT);

  Serial.begin(9600);
  Serial.println("Iniciado");
  tempoPisca = millis();
  
}

void loop() {
  if(!digitalRead(btnStart)){
    ControleLeds(false);
    sttLeds = false;
    
    Game();
  }


  if(tempoPisca+500 < millis()){
    ControleLeds(sttLeds?2:3);
    sttLeds = !sttLeds;
    tempoPisca = millis();
  }

}


void gerarSequencia(byte sequencia []){
  randomSeed(analogRead(0));
  // Serial.println("----------Sequencia--------------");
  for(byte i = 0; i < tamanho; i++){
    sequencia[i] = random(0, 6);
    // Serial.println(sequencia[i]);
  }
}

void Game(){

  byte sequencia [tamanho];
  gerarSequencia(sequencia);

  int pos = 1;
  
  while(true){
    mostrarSequenciar(sequencia, pos);
    Serial.println("Rodada" + (String)pos);
    if(analizarSequencia(sequencia, pos)){
      pos++;
      if(pos>HiScore){
        HiScore++;
        ControleLeds(5);
      }      
      Serial.println("Vamos pra mais uma");
    }
    else{
      ControleLeds(4);   
      Serial.println("Perdeu o Jogo");
      break;
    }
  }

}

void mostrarSequenciar(byte sequencia [], int pos){
  digitalWrite(ledStart, 1);
  for(int i = 0; i<pos; i++){
    byte led = sequencia[i];
    Serial.println("Mostrar led - "+(String)led);
    digitalWrite(leds[led], 1);
    delay(tempoLed);
    digitalWrite(leds[led], 0);
    delay(tempoLed);
  }
  digitalWrite(ledStart, 0);

}

bool teclaPress(byte key){
  unsigned long tempoPress = millis();
  while(tempoPress+5000 > millis()){
    for(byte i = 0; i < 6; i++){
      if(!digitalRead(pins[i])){
        digitalWrite(leds[i], 1);
        while(!digitalRead(pins[i]))
          delay(30);
        digitalWrite(leds[i], 0);
        Serial.println("\t acertou - "+(String)i + " Key - "+(String)key);      
        return i == key;
      }
      delay(1);
    }
  }
  return false;
}

bool analizarSequencia(byte sequencia[], int pos){

  for(int i = 0; i < pos; i++){
    if(!teclaPress(sequencia[i])){
      Serial.println("Errou pos"+(String)i);
      return false;
    }
    delay(100);
  }
  return true;
}

void ControleLeds(byte stt){
  switch(stt){
    case 0:
      for(int i = 0; i< 6; i++){
        digitalWrite(leds[i], 0);
      }
      break;
    case 1:
      for(int i = 0; i< 6; i++){
        digitalWrite(leds[i], 1);
      }
      break;
    case 2:
      for(int i = 0; i< 6; i++){
        digitalWrite(leds[i++], 1);
        digitalWrite(leds[i], 0);
      }
      break;
    case 3:
      for(int i = 0; i< 6; i++){
        digitalWrite(leds[i++], 0);
        digitalWrite(leds[i], 1);
      }
      break;
    case 4:
      for(byte i = 0; i<3; i++){
        ControleLeds(true);
        delay(500);
        ControleLeds(false);
        delay(500);
      }   
      break;
    case 5:
      ControleLeds(false);
      digitalWrite(ledStart, 0);
      for(int i = 0; i < 3; i++){   
        digitalWrite(leds[0], 1);
        digitalWrite(leds[2], 1);
        delay(150);
        digitalWrite(leds[3], 1);
        digitalWrite(leds[4], 1);
        delay(150);
        digitalWrite(leds[5], 1);
        digitalWrite(leds[0], 0);
        digitalWrite(leds[2], 0);
        delay(150);
        digitalWrite(leds[1], 1);
        digitalWrite(leds[3], 0);
        digitalWrite(leds[4], 0);
        delay(150);
        digitalWrite(ledStart, 1);        
        digitalWrite(leds[5], 0);
        delay(150);
        ControleLeds(false);
        digitalWrite(ledStart, 0);  
      }
      
      break;
  }  
}