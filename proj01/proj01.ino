//================================ Incluindo bibliotecas ============================//
#include  <LiquidCrystal.h>         // Inclui a biblioteca para utilizar o LCD.
#include <Thermistor.h>


//================================ Mapeamento de Hardware ============================//
LiquidCrystal LCD (12, 11, 5, 4, 3, 2); //Esta função declara quais os pinos do Arduino serão utilizados para o controle do LCD
 
//definição dos sensores
#define sensorTemperatura A0
#define sensorUmidade A5

//definição dos leds
#define LedVermelho 6
#define LedVerde 9
#define LedAmarelo 6

//definição dos relés
#define Rele_Umidade 10
#define Rele_Resistor 7
#define Rele_Peltier 8

//#define Sensor 0  // Define o pino A0 como “sensor”
#define PotenciometroAjusteTemp 1  // Define o pino A0 como “sensor”



//=========================== Declarando Variáveis Globais =========================//
float Constante = 39.34;// Constante para estabelecer a faixa de temperatura de 0 à 20 Graus / 1023/20 = 51.15.
float  Temperatura; // Variável que recebe o valor convertido para temperatura.
int ValorAjustadoTemp = 0;
int histerese = 5;

//======================== Array que desenha o simbolo de grau =====================//
byte a[8] = {B00110, B01001, B00110, B00000, B00000, B00000, B00000, B00000,};

//========================= Criando o objeto para o termistor ======================//
Thermistor temp(sensorTemperatura);

bool leituraSensorUmidade;
bool leituraAnteriorUmidade;

void setup() {
  
  //SensorUmidade
  pinMode(sensorUmidade, INPUT);

  //Atuadores
  pinMode(Rele_Umidade, OUTPUT);
  pinMode(Rele_Peltier, OUTPUT);
  pinMode(Rele_Resistor, OUTPUT);

  //LEDs
  pinMode(LedVermelho, OUTPUT); //Precisa mudar  //vermelho
  pinMode(LedVerde, OUTPUT);  //amarelo
  pinMode(LedAmarelo, OUTPUT);  //verde

  //LCD
  LCD.begin(16, 2);              // Diz para o Arduino que o display é 16x2.
  LCD.setCursor(0, 0);           // Move o cursor do display para a segunda linha.
  LCD.print("Temp: RT:  AJT:");
}

void loop() {
  //Leituras
  leituraSensorUmidade = digitalRead(A5); //umidade
  int umidade = (leituraSensorUmidade) * (100/1023); //conversao para variavel
  ValorAjustadoTemp = analogRead(PotenciometroAjusteTemp) / Constante ; //ajuste de temperatura
  int Temperature = temp.getTemp(); //coleta da temperatura

  /*
  if (umidade == HIGH) {
     //No estado seco
     digitalWrite(LedVermelho, HIGH);  //vermelho
     digitalWrite(LedVerde, LOW);   //verde
  } else {
     //No estado úmido
     digitalWrite(LedVermelho, LOW);   //vermelho
     digitalWrite(LedVerde, HIGH);  //verde
  }
  */


  if(umidade < 20){
    digitalWrite(LedVermelho, HIGH);
    digitalWrite(LedVerde, LOW);
    digitalWrite(LedAmarelo, LOW);

    digitalWrite(Rele_Umidade, HIGH);
    delay(1000);
    digitalWrite(Rele_Umidade, LOW);
  } else if(umidade > 20 && umidade <= 50) {
    digitalWrite(LedVermelho, LOW);
    digitalWrite(LedVerde, LOW);
    digitalWrite(LedAmarelo, HIGH);
    delay(1000);
  } else if(umidade > 50) {
    digitalWrite(LedVermelho, LOW);
    digitalWrite(LedVerde, HIGH);
    digitalWrite(LedAmarelo, LOW);
  }

  //Ao entrar no estado seco 
  /* 
  if (leituraSensorUmidade && !leituraAnteriorUmidade) {
     delay(5000);
     digitalWrite(LedVermelho, LOW);   //vermelho
     digitalWrite(LedAmarelo, HIGH);  //amarelo

     while (digitalRead(leituraSensorUmidade)) {
        digitalWrite(Rele_Umidade, HIGH);   //rele / válvula / solenoide / bomba
        delay(500);
        digitalWrite(Rele_Umidade, LOW);   //rele / válvula / solenoide / bomba

        delay(10000);          
     }
     digitalWrite(LedAmarelo, LOW);  //amarelo
  }
  */
  
  //leituraAnteriorUmidade = leituraSensorUmidade;

  

  if (Temperature >= ValorAjustadoTemp - histerese ) {
    digitalWrite(Rele_Peltier, HIGH);
    digitalWrite(Rele_Resistor, LOW);
  }
  if (Temperature <= (ValorAjustadoTemp - histerese) ) {
    digitalWrite(Rele_Peltier, LOW);
    digitalWrite(Rele_Resistor, HIGH);
  }


  //Codigo do LCD
  LCD.setCursor(5, 1);  // Move o cursor do display para a segunda linha.
  LCD.print(Temperature); // Exibe o valor de temperatura no display.
  LCD.setCursor(8, 1); // Move o cursor do display para a segunda linha.
  LCD.print("C"); // Escreve “C” para dizer que a escala é Celsius.

  LCD.createChar(1, a);//Atribui a "1" o valor do array "A", que desenha o simbolo de grau
  LCD.setCursor(7, 1); // Move o cursor do display para a segunda linha.
  LCD.write(1); //Escreve o simbolo de grau

  LCD.setCursor(11, 1); // Move o cursor do display para a segunda linha.
  LCD.print(ValorAjustadoTemp);  // Exibe o valor de temperatura no display.
  LCD.setCursor(14, 1);// Move o cursor do display para a segunda linha.
  LCD.print("C"); // Escreve “C” para dizer que a escala é Celsius.

  LCD.createChar(1, a); //Atribui a "1" o valor do array "A", que desenha o simbolo de grau
  LCD.setCursor(13, 1);// Move o cursor do display para a segunda linha.
  LCD.write(1); //Escreve o simbolo de grau

  if (Temperature < 10) { 
    LCD.setCursor(6, 1); // Move o cursor do display para a segunda linha.
    LCD.print(" ");  // Escreve “C” para dizer que a escala é Celsius.
  }

  if (ValorAjustadoTemp < 10) { // Se for menor que 10
    LCD.setCursor(12, 1); // Seta o curso na linha 2 coluna 12.
    LCD.print(" ");  // Escreve um espaço em branco no display para apagar um das casas.
  }

  delay(1000); // Aguarda 1 segundo para efetuar uma nova leitura de temperatura.
}
