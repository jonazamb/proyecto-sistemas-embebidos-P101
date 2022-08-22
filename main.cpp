#include <Arduino_FreeRTOS.h>
#include <Wire.h>
#include <util/delay.h>
#include <avr/io.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd1(0x27,16,2);
void menu(void);
void TaskAnalogRead( void *pvParameters );
void pajuela1( void *pvParameters );
float sensorValue;
float temperatura;
char valor='1';
const int rele= 12;

const byte FILAS = 4; //four rows
const byte COLUMNAS = 3; //four columns
//define the cymbols on the buttons of the keypads
char keys[FILAS][COLUMNAS] = {
  { '1','2','3'},
  { '4','5','6' },
  { '7','8','9'},
  { '#','0','*'}
};
 byte pinesFilas[FILAS] = {8,7,6,5}; //connect to the row pinouts of the keypad
 byte pinesColumnas[COLUMNAS] = {4,3,2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad teclado = Keypad( makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS); 
char teclado_valor;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(rele, OUTPUT);
  lcd1.init();
  lcd1.backlight();
  
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  xTaskCreate(
    TaskAnalogRead
    ,  "AnalogRead"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

    xTaskCreate(
    pajuela1
    ,  "Analisis1"
    ,  128  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL );

    menu();
}

void loop()
{
  // Empty. Things are done in Tasks.
}

void TaskAnalogRead(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  

  for (;;)
  {
    // read the input on analog pin 0:
    sensorValue = analogRead(A0);
    temperatura =(sensorValue*5.0*100)/1024.0;
    // print out the value you read:
    Serial.println(temperatura);
    vTaskDelay(1);  // one tick delay (15ms) in between reads for stability
  }
}


void pajuela1(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  

  for (;;)
  {
    char teclado_valor=teclado.getKey();
   
    if(teclado_valor==valor) {
       digitalWrite(rele, HIGH);
       lcd1.setCursor(0,0);
       lcd1.print("selec opcion:1"); 
       lcd1.setCursor(0,1);
       lcd1.print("50ml,35 grados C"); 
       _delay_ms(1000);
       //digitalWrite(ledPIN , LOW);
       //sensorValue = analogRead(A0);
       //temperatura =(sensorValue*5.0*100)/1024.0;

       lcd1.clear();
       lcd1.setCursor(0,0);
       lcd1.println(temperatura);
        //---------------------------------------//
       bool bandera=true ; 
       while(bandera){
      
          
        if(temperatura>35){
          digitalWrite(rele,HIGH);
          lcd1.clear(); 
          lcd1.setCursor(0,0);
          lcd1.print("mayor a 36 C"); 
          _delay_ms(1000);};

        if(temperatura<36){
          
          digitalWrite(rele,LOW);
          lcd1.clear(); 
          lcd1.setCursor(0,0);
          lcd1.print("menor a 36 C");
          _delay_ms(1000);};

          
        if(34.9<temperatura &&  temperatura<35.9){
          digitalWrite(rele,LOW);
          lcd1.clear(); 
          lcd1.setCursor(0,0);
          lcd1.print("TEMP.CORRECTA");
          lcd1.setCursor(0,1);
          lcd1.print(" 35 GRADOS "); 
          _delay_ms(1000);};
            };
           

     //---------------------------------------//  
   };
 }
}


void menu(void){
  lcd1.setCursor(1,0);
  lcd1.print("Descongelador");
  _delay_ms(1000);
  lcd1.clear();
  lcd1.setCursor(1,0);
  lcd1.print("de ");
  _delay_ms(1000);
  lcd1.clear();
  lcd1.setCursor(1,0);
  lcd1.print("pajuelas ");
  _delay_ms(1000);
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("MENU:1)0.5[ml] ");
  
  lcd1.setCursor(5,1);
  lcd1.print("2)0.25[ml] "); 
  _delay_ms(1000);
  //lcd1.clear(); 
   // lcd1.setCursor(0,0);
  //lcd1.print("***SELECCIONE*** ");
  }
