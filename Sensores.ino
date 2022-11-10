#include <dht.h>
#include <EmonLib.h>


#define dht_apin A2 // Analog Pin sensor is connected to
#define CURRENT_CAL 17.50 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA
#define VOLT_CAL 211.6

double supplyVoltage = 0.0;
float voltage = 0;
const int pinoSensor = A0; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
float ruido = 0.08; //RUÍDO PRODUZIDO NA SAÍDA DO SENSOR (DEVE SER AJUSTADO COM A CARGA DESLIGADA APÓS CARREGAMENTO DO CÓDIGO NO ARDUINO)
double currentDraw = 0.0;
float power = 0.0;
float medA = 0;
float medT = 0;
int i = 0;
float humidity = 0;
float temp = 0;
const int pResistor = A3; // Photoresistor at Arduino analog pin A3
const int ledPin=9;       // Led pin at Arduino pin 9
int value;  
const int Respin = 8;

EnergyMonitor emon1; //CRIA UMA INSTÂNCIA
EnergyMonitor emon2;

dht DHT;

void setup() {
  Serial.begin(9600); //INICIALIZA A SERIAL
  emon1.voltage(1, VOLT_CAL, 1.7);
  emon2.current(pinoSensor, CURRENT_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
  pinMode(ledPin, OUTPUT);  // Set lepPin - 9 pin as an output
  pinMode(Respin, OUTPUT);
  pinMode(pResistor, INPUT);// Set pResistor - A0 pin as an input (optional)
  Serial.print("Luminosidade (0-1024)");
  Serial.print("\t");
  Serial.print("Tensão (V)");
  Serial.print("\t");
  Serial.print("Corrente (A)");
  Serial.print("\t");
  Serial.print("Potência (W)");
  Serial.print("\t");
  Serial.print("Temperatura (°C)");
  Serial.print("\t");
  Serial.println("Umidade (%)");
}

void loop() {
  Tensao();
  Corrente();
  Power();
  temphum();
  lumen();  
  Serial.print(voltage);
  Serial.print("\t");
  Serial.print(currentDraw);
  Serial.print("\t");
  Serial.print(power);
  Serial.print("\t");
  Serial.print(humidity); 
  Serial.print("\t");
  Serial.println(temp);
  if (temp < 22){
      digitalWrite(Respin, LOW);
  }
  else{
      digitalWrite(Respin, HIGH);
  }
  delay(1000);
}

void Corrente(){
  medA = 0;
  emon2.calcVI(17,1000); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  currentDraw = emon2.Irms; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  currentDraw = currentDraw-ruido; //VARIÁVEL RECEBE O VALOR RESULTANTE DA CORRENTE RMS MENOS O RUÍDO
  
  if(currentDraw < 0){ //SE O VALOR DA VARIÁVEL FOR MENOR QUE 0, FAZ 
        currentDraw = 0; //VARIÁVEL RECEBE 0
  }
  delay(10);
}

void Tensao(){
  medT = 0;
  emon1.calcVI(17, 2000);                        //FUNÇÃO DE CÁLCULO (17 SEMICICLOS, TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  float supplyVoltage = emon1.Vrms;           //VARIÁVEL RECEBE O VALOR DE TENSÃO RMS OBTIDO
  voltage = supplyVoltage;

  delay(10);
  
}

void Power(){
  power = voltage * currentDraw;
}

void temphum(){
  DHT.read11(dht_apin);
  humidity = DHT.humidity;
  temp = DHT.temperature;
}

void lumen(){
  value = analogRead(pResistor);
  Serial.print(value);
  Serial.print("\t");
  
  //You can change value "25"
  if (value > 550){
    digitalWrite(ledPin, LOW);  //Turn led off
  }
  else{
    digitalWrite(ledPin, HIGH); //Turn led on
  }
}
