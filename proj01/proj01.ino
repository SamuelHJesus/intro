//================================ Incluindo bibliotecas ============================//
#include  <LiquidCrystal.h>         // Inclui a biblioteca para utilizar o LCD.
#include <Thermistor.h>


//================================ Mapeamento de Hardware ============================//
LiquidCrystal LCD (12, 11, 5, 4, 3, 2); //Esta função declara quais os pinos do Arduino serão utilizados para o controle do LCD 

#define Sensor 0  // Define o pino A0 como “sensor”
#define PotenciometroAjusteTemp 1  // Define o pino A0 como “sensor”
#define Rele_Peltier 8

//=========================== Declarando Variáveis Globais =========================//
float Constante = 51.15;// Constante para estabelecer a faixa de temperatura de 0 à 20 Graus / 1023/20 = 51.15.
float  Temperatura; // Variável que recebe o valor convertido para temperatura.
int ValorAjustadoTemp = 0;

//======================== Array que desenha o simbolo de grau =====================//
byte a[8] = {B00110, B01001, B00110, B00000, B00000, B00000, B00000, B00000,};

//========================= Criando o objeto para o termistor ======================//
Thermistor temp(Sensor);

bool leituraSensor;
bool leituraAnterior;

void setup() {
  
  //Sensor
  pinMode(8, INPUT);

  //Atuador
  pinMode(12, OUTPUT);
  
  //LEDs
  pinMode(5, OUTPUT);  //vermelho
  pinMode(6, OUTPUT);  //amarelo
  pinMode(7, OUTPUT);  //verde

  pinMode( Rele_Peltier, OUTPUT);// Declara o pino do rele como 

  LCD.begin(16, 2);                            // Diz para o Arduino que o display é 16x2.
  LCD.setCursor(0, 0);           // Move o cursor do display para a segunda linha.
  LCD.print("Temp: RT:  AJT:");
}

void loop() {

  leituraSensor = digitalRead(8);

  if (leituraSensor == HIGH) {
     //No estado seco
     digitalWrite(5, HIGH);  //vermelho
     digitalWrite(7, LOW);   //verde
  } else {
     //No estado úmido
     digitalWrite(5, LOW);   //vermelho
     digitalWrite(7, HIGH);  //verde
  }

  //Ao entrar no estado seco  
  if (leituraSensor && !leituraAnterior) {
     delay(5000);
     digitalWrite(5, LOW);   //vermelho
     digitalWrite(6, HIGH);  //amarelo

     while (digitalRead(8)) {
        digitalWrite(12, HIGH);   //rele / válvula / solenoide / bomba
        delay(500);
        digitalWrite(12, LOW);   //rele / válvula / solenoide / bomba

        delay(10000);          
     }
     digitalWrite(6, LOW);  //amarelo
  }
  
  leituraAnterior = leituraSensor;

  ValorAjustadoTemp = analogRead(PotenciometroAjusteTemp) / Constante ;

  int Temperature = temp.getTemp();

  if (Temperature >= ValorAjustadoTemp ) {
    digitalWrite(Rele_Peltier, HIGH);
  }
  if (Temperature <= (ValorAjustadoTemp - 1) ) {
    digitalWrite(Rele_Peltier, LOW);
  }

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
