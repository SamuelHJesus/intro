#include  <LiquidCrystal.h>
//definição das saidas com nomes pra facilitar

LiquidCrystal LCD (12, 11, 5, 4, 3, 2);

	#define resistor 6
	#define releTemperatura 8
	#define releUmidade 10
	#define sensorTemperatura A4
	#define sensorUmidade A5
	#define potenciometroTemperatura A0

	#define ledVl 9
	#define ledA 7
	#define ledVd 13

//define uma temperatura de histerese para evitar ligar
	//e desligar a todo momento
	int histerese = 5;
	int val = 0;
	float umidade = 0.0;
	float temperaturaLida = 0.0;
	float temperaturaAjustada;
	float constante = 34.1;

byte a[8] = {B00110, B01001, B00110, B00000, B00000, B00000, B00000, B00000,};

void setup()
{
 //definição de cada um deles como entrada ou saída de sinais/dados
  pinMode(releTemperatura, OUTPUT);
  pinMode(releUmidade, OUTPUT);
  pinMode(resistor, OUTPUT);
  pinMode(sensorTemperatura, INPUT);
  pinMode(sensorUmidade, INPUT);
  pinMode(potenciometroTemperatura, INPUT);
  
  pinMode(ledVl, OUTPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledVd, OUTPUT);
  
  Serial.begin(9600);
  
  LCD.begin(16, 2);
  LCD.setCursor(0, 0);
  LCD.print("Temp: RT:  AJT:");
}

void loop()
{
  
  temperaturaAjustada = analogRead(potenciometroTemperatura) / constante;
  
  val = analogRead(sensorTemperatura);
  temperaturaLida = map(((val - 20) * 3.04), 0, 1023, -40, 125);
  
  float umidadeSensor = analogRead(sensorUmidade);
  umidade = (umidadeSensor/876) * 100;
  
  if(temperaturaLida > temperaturaAjustada){
  	digitalWrite(releTemperatura, HIGH);
    digitalWrite(resistor, LOW);
  }
  
  if(temperaturaLida < temperaturaAjustada - histerese){
  	digitalWrite(releTemperatura, LOW);
    digitalWrite(resistor, HIGH);
  }
  
  if(umidade < 30){
  	digitalWrite(releUmidade, HIGH);
    delay(3000);
    digitalWrite(releUmidade, LOW);
    
    digitalWrite(ledVl, HIGH);
    digitalWrite(ledA, LOW);
    digitalWrite(ledVd, LOW);
  }
  if(umidade > 30 && umidade < 70){
  	
    digitalWrite(ledVl, LOW);
    digitalWrite(ledA, HIGH);
    digitalWrite(ledVd, LOW);
  }
  if(umidade > 70){
  	
    digitalWrite(ledVl, LOW);
    digitalWrite(ledA, LOW);
    digitalWrite(ledVd, HIGH);
  }
  
  LCD.setCursor(5, 1);
  LCD.print(temperaturaLida);
  LCD.setCursor(8, 1);
  LCD.print("C");
  
  LCD.createChar(1, a);
  LCD.setCursor(7, 1);
  LCD.write(1);
  
  LCD.setCursor(11, 1);
  LCD.print(temperaturaAjustada);
  LCD.setCursor(14, 1);
  LCD.print("C");
  LCD.createChar(1, a);
  LCD.setCursor(13, 1);
  LCD.write(1);
  
  if (temperaturaLida < 10) { 
    LCD.setCursor(6, 1);
    LCD.print(" ");
  }
  
  if (temperaturaAjustada < 10) {
    LCD.setCursor(12, 1);
    LCD.print(" ");
  }
  
  
  Serial.print("Temperatura Sensor: ");
  Serial.println(temperaturaLida);
  Serial.print("Temperatura Pedida: ");
  Serial.println(temperaturaAjustada);
  Serial.print("Umidade: ");
  Serial.println(umidade);
  
  delay(1000);
}