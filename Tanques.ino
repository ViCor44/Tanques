/* 
 Controle de níveis nos tanques de aproveitamento de água
 Versão: 3.2
 Autor: Victor Correia
 Alterado em: 24/10/2018
 */

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <SD.h>
#include <Average.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define releFuro1 22
#define releFuro2 23
#define trigPinTanque1 25
#define echoPinTanque1 24
#define trigPinTanque2 27
#define echoPinTanque2 26
#define trigPinTanque3 21
#define echoPinTanque3 20
#define trigPinTanque4 31
#define echoPinTanque4 30
#define trigPinTanque5 33
#define echoPinTanque5 32
#define rele1Tanque1 34
#define rele2Tanque1 35
#define rele1Tanque2 36
#define rele2Tanque2 37
#define rele1Tanque3 38
#define rele2Tanque3 39
#define rele1Tanque4 40
#define rele2Tanque4 41
#define rele1Tanque5 42
#define rele2Tanque5 43
#define releValvula1 44 
#define releValvula2 45
#define releValvula3 46
#define releFiltro1 47
#define releFiltro2 48
#define resetFlag 49

int distance[5];
int level1;
int level2;
int level3;
int level4;
int level5;
int aux1;
int aux2;
int aux3;
int aux4;
int aux5;
bool flag1;
bool flag2;
bool flag3;
bool flag4;
bool flag5;

void setup() {
    Serial.begin (9600);
   
    lcd.begin(16, 2);
    pinMode(trigPinTanque1, OUTPUT);
    pinMode(echoPinTanque1, INPUT);
    pinMode(trigPinTanque2, OUTPUT);
    pinMode(echoPinTanque2, INPUT);
    pinMode(trigPinTanque3, OUTPUT);
    pinMode(echoPinTanque3, INPUT);
    pinMode(trigPinTanque4, OUTPUT);
    pinMode(echoPinTanque4, INPUT);
    pinMode(trigPinTanque5, OUTPUT);
    pinMode(echoPinTanque5, INPUT);
    pinMode(rele1Tanque1, OUTPUT);
    pinMode(rele2Tanque1, OUTPUT);
    pinMode(rele1Tanque2, OUTPUT);
    pinMode(rele2Tanque2, OUTPUT);
    pinMode(rele1Tanque3, OUTPUT);
    pinMode(rele2Tanque3, OUTPUT);
    pinMode(rele1Tanque4, OUTPUT);
    pinMode(rele2Tanque4, OUTPUT);
    pinMode(rele1Tanque5, OUTPUT);
    pinMode(rele2Tanque5, OUTPUT);
    pinMode(releValvula1, OUTPUT);
    pinMode(releValvula2, OUTPUT);
    pinMode(releValvula3, OUTPUT);
    pinMode(releFiltro1, OUTPUT);
    pinMode(releFiltro2, OUTPUT);
    pinMode(releFuro1, OUTPUT);
    pinMode(releFuro2, OUTPUT);
    pinMode(resetFlag, OUTPUT);

    digitalWrite(rele1Tanque1,HIGH);
    digitalWrite(rele2Tanque1,HIGH);
    digitalWrite(rele1Tanque2,HIGH);
    digitalWrite(rele2Tanque2,HIGH);
    digitalWrite(rele1Tanque3,HIGH);
    digitalWrite(rele2Tanque3,HIGH);
    digitalWrite(rele1Tanque4,HIGH);
    digitalWrite(rele2Tanque4,HIGH);
    digitalWrite(rele1Tanque5,HIGH);
    digitalWrite(rele2Tanque5,HIGH);
    digitalWrite(releFiltro1,HIGH);
    digitalWrite(releFiltro2,HIGH);
    digitalWrite(releValvula1,HIGH);
    digitalWrite(releValvula2,HIGH);
    digitalWrite(releValvula3,HIGH);      
    digitalWrite(releFuro1,HIGH);
    digitalWrite(resetFlag,HIGH);

    flag1 = false;
    flag2 = false;
    flag3 = false;
    flag4 = false;
    flag5 = false;

    aux1 = measure(trigPinTanque1, echoPinTanque1);
    aux2 = measure(trigPinTanque2, echoPinTanque2);
    aux3 = measure(trigPinTanque3, echoPinTanque3);
    aux4 = measure(trigPinTanque4, echoPinTanque4);
    aux5 = measure(trigPinTanque5, echoPinTanque5);
    
    lcd.print("Controle de niveis");
    delay(800);
    for (int positionCounter = 0; positionCounter < 19; positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    delay(800);  
    lcd.clear();
    lcd.print("Versao 3.5");
    for (int positionCounter = 0; positionCounter < 11; positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    delay(800);  
    lcd.clear();
    lcd.print("Actual em 18/08/2020");
    for (int positionCounter = 0; positionCounter < 20; positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    delay(1500);  
    lcd.clear();

    /*valvula do tanque de alimentação da Osmose 2 sempre fechada*/
    digitalWrite(releValvula2,HIGH);

}

int measure(int trig, int echo)
{
  Average<int> modal(10);
  int i; 
  int measuring;
  float duration; 
  for(i=0; i<10; i++)
  {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(20);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo, HIGH);
    measuring = (duration/2) / 29.1;    
    modal.push(measuring);    
    delay(50);
  }
  Serial.println("Distancia medida: ");
  Serial.println(modal.mode());
  return modal.mode();
}

void display()
{
  if(flag1 == false){
    lcd.print("Tanque 1: ");
    lcd.print(level1);
    lcd.print("m3");
    delay(2000);  
    lcd.clear();
  }
  if(flag2 == false){
    lcd.print("Tanque 2: ");
    lcd.print(level2);
    lcd.print("m3");
    delay(2000);
    lcd.clear();
  }
  if(flag3 == false){
    lcd.print("Tanque 3: ");
    lcd.print(level3);
    lcd.print("m3");
    delay(2000);
    lcd.clear();
  }
  if(flag4 == false){
     lcd.print("Tanque 4: ");
    lcd.print(level4);
    lcd.print("m3");
    delay(2000);
    lcd.clear();
  }
  if(flag5 == false){
    lcd.print("Tanque 5: ");
    lcd.print(level5);
    lcd.print("m3");
    delay(2000);
    lcd.clear();
  }  
}

void erroLeitura(int tanque){
  Serial.print("Erro de leitura no tanque ");
  Serial.println(tanque);
  lcd.setCursor(0, 0);
  lcd.print("Erro de leitura");
  lcd.setCursor(0, 1);
  lcd.print("Tanque ");
  lcd.print(tanque);
  delay(2000);
}

void tanque1(){ 
  if(flag1 == true){
    erroLeitura(1);
    digitalWrite(rele1Tanque1,HIGH);
    digitalWrite(rele2Tanque1,HIGH);
    distance[0] = 37;
    return;
  }
  static int count1 = 0;
  Serial.print("Aux1: ");
  Serial.println(aux1);
  distance[0] = measure(trigPinTanque1, echoPinTanque1);
  delay(1000);
  if(aux1 + 10 < distance[0] || aux1 - 10 > distance[0]){
    count1 = count1 + 1;
    distance[0] = aux1;
    Serial.print("Count1: ");
    Serial.println(count1);
    Serial.print("Dist1: ");
    Serial.println(distance[0]);
  } else{
    aux1 = distance[0];
    count1 = 0;
  }
  if(count1 >= 10){    
    flag1 = true;
  }
}

void tanque2(){ 
  if(flag2 == true){
    erroLeitura(2);
    digitalWrite(rele1Tanque2,HIGH);
    digitalWrite(rele2Tanque2,HIGH);
    distance[1] = 37;
    return;
  }
  static int count2 = 0;
  Serial.print("Aux2: ");
  Serial.println(aux2);
  distance[1] = measure(trigPinTanque2, echoPinTanque2);
  delay(1000);
  if(aux2 + 10 < distance[1] || aux2 - 10 > distance[1]){
    count2 = count2 + 1;
    distance[1] = aux2;
    Serial.print("Count2: ");
    Serial.println(count2);
    Serial.print("Dist2: ");
    Serial.println(distance[1]);
  } else{
    aux2 = distance[1];
    count2 = 0;
  }
  if(count2 >= 10){    
    flag2 = true;
  }
}

void tanque3(){ 
  if(flag3 == true){
    erroLeitura(3);
    digitalWrite(rele1Tanque3,HIGH);
    digitalWrite(rele2Tanque3,HIGH);
    distance[2] = 37;
    return;
  }
  static int count3 = 0;
  Serial.print("Aux3: ");
  Serial.println(aux3);
  distance[2] = measure(trigPinTanque3, echoPinTanque3);
  delay(1000);
  if(aux3 + 10 < distance[2] || aux3 - 10 > distance[2]){
    count3 = count3 + 1;
    distance[2] = aux3;
    Serial.print("Count3: ");
    Serial.println(count3);
    Serial.print("Dist3: ");
    Serial.println(distance[2]);
  } else{
    aux3 = distance[2];
    count3 = 0;
  }
  if(count3 >= 10){    
    flag3 = true;
  }
}

void tanque4(){ 
  if(flag4 == true){
    digitalWrite(rele1Tanque4,HIGH);
    digitalWrite(rele2Tanque4,HIGH);
    //distance[3] = 37;
    erroLeitura(4);
    return;
  }
  static int count4 = 0;
  Serial.print("Aux4: ");
  Serial.println(aux4);
  distance[3] = measure(trigPinTanque4, echoPinTanque4);
  delay(1000);
  if(aux4 + 10 < distance[3] || aux4 - 10 > distance[3]){
    count4 = count4 + 1;
    distance[3] = aux4;
    Serial.print("Count4: ");
    Serial.println(count4);
    Serial.print("Dist4: ");
    Serial.println(distance[3]);
  } else{
    aux4 = distance[3];
    count4 = 0;
  }
  if(count4 >= 10){    
    flag4 = true;
  }
}

void tanque5(){ 
  if(flag5 == true){
    digitalWrite(rele1Tanque5,HIGH);
    digitalWrite(rele2Tanque5,HIGH);
    distance[4] = 37;
    erroLeitura(5);
    return;
  }
  static int count5 = 0;
  Serial.print("Aux5: ");
  Serial.println(aux5);
  distance[4] = measure(trigPinTanque5, echoPinTanque5);
  delay(1000);
  if(aux5 + 10 < distance[4] || aux5 - 10 > distance[4]){
    count5 = count5 + 1;
    distance[4] = aux5;
    Serial.print("Count5: ");
    Serial.println(count5);
    Serial.print("Dist5: ");
    Serial.println(distance[4]);
  } else{
    aux5 = distance[4];
    count5 = 0;
  }
  if(count5 >= 10){    
    flag5 = true;
  }
}

void nivelTanque1(){
    if(flag1 == true){
      return;
    }
   /*Reles de nivel Tanque 1*/   
    if(distance[0] <= 37){
      digitalWrite(rele1Tanque1,HIGH);        
    }else if(distance[0]  >= 47){
      digitalWrite(rele1Tanque1,LOW);        
    }
    if(distance[0] >= 215){
      digitalWrite(rele2Tanque1,HIGH);      
    }else if(distance[0] <= 205){
      digitalWrite(rele2Tanque1,LOW);
    }
}

void nivelTanque2(){
    if(flag2 == true){
      return;
    }
    /*Reles de nivel Tanque 2*/
    if(distance[1] <= 37){
      digitalWrite(rele1Tanque2,HIGH);        
    }else if(distance[1]  >= 47){
      digitalWrite(rele1Tanque2,LOW);        
    }
    if(distance[1] >= 205){
      digitalWrite(rele2Tanque2,HIGH);
    }else if(distance[1] <= 200){
      digitalWrite(rele2Tanque2,LOW);
    }
}

void nivelTanque3(){
    if(flag3 == true){
      return;
    }
    /*Reles de nivel Tanque 3*/
    if(distance[2] <= 37){
      digitalWrite(rele1Tanque3,HIGH);
    }else if(distance[2]  >= 47){
      digitalWrite(rele1Tanque3,LOW);
    }
    if(distance[2] >= 215){
      digitalWrite(rele2Tanque3,HIGH);
    }else if(distance[2] <= 205){
      digitalWrite(rele2Tanque3,LOW);
    }
}

void nivelTanque4(){
    if(flag4 == true){
      return;
    }
    /*Reles de nivel Tanque 4*/
    if(distance[3] <= 47){
      digitalWrite(rele1Tanque4,HIGH);
    }else if(distance[3]  >= 57){
      digitalWrite(rele1Tanque4,LOW);
    }
    if(distance[3] >= 230){
      digitalWrite(rele2Tanque4,HIGH);
    }else if(distance[3] <= 220){
      digitalWrite(rele2Tanque4,LOW);
    }
}

void nivelTanque5(){
    if(flag5 == true){
      return;
    }
    /*Reles de nivel Tanque 5*/
    if(distance[4] <= 37){
      digitalWrite(rele1Tanque5,HIGH);
    }else if(distance[4]  >= 47){
      digitalWrite(rele1Tanque5,LOW);
    }
    if(distance[4] >= 160){
      digitalWrite(rele2Tanque5,HIGH);
    }else if(distance[4] <= 150){
      digitalWrite(rele2Tanque5,LOW);
    }
}

void loop() { 

    /*Verifica botão resetFlag*/
    if(digitalRead(resetFlag) == 0){
        flag1 = false;
        flag2 = false;
        flag3 = false;
        flag4 = false;
        flag5 = false;
        Serial.print("Reset Erro ");        
        lcd.setCursor(0, 0);
        lcd.print("Reset Erro ");        
        delay(1000);
    }
    
    /*Efectuar as medicoes*/    
    lcd.setCursor(0, 0);
    lcd.print("A Efectuar");
    lcd.setCursor(0, 1);
    lcd.print("Medicoes...");

    tanque1();
    tanque2();
    tanque3();
    tanque4();
    tanque5();

    nivelTanque1();
    nivelTanque2();
    nivelTanque3();
    nivelTanque4();
    nivelTanque5();
    
    for (int positionCounter = 0; positionCounter < 11; positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    lcd.clear(); 
    
    /*Debug das medicoes*/
    Serial.print("Tanque 1 ");
    Serial.println(distance[0]);
    Serial.print("Tanque 2 ");
    Serial.println(distance[1]);
    Serial.print("Tanque 3 ");
    Serial.println(distance[2]);
    Serial.print("Tanque 4 ");
    Serial.println(distance[3]);
    Serial.print("Tanque 5 ");
    Serial.println(distance[4]);
    level1 = map(distance[0], 35, 270, 300, 0);  
    if (level1 < 0) {
      level1 = 0;
    }
    level2 = map(distance[1], 50, 270, 123, 0);  
    if (level2 < 0) {
      level2 = 0;
    }
    level3 = map(distance[2], 35, 270, 470, 0);  
    if (level3 < 0) {
      level3 = 0;
    }
    level4 = map(distance[3], 35, 285, 218, 0);  
    if (level4 < 0) {
      level4 = 0;
    }
    level5 = map(distance[4], 35, 231, 570, 0);  
    if (level5 < 0) {
      level5 = 0;
    }    
    
    /*Actualizaçao dos reles*/    
    lcd.setCursor(0, 0);
    lcd.print("A Actualizar");
    lcd.setCursor(0, 1);
    lcd.print("Reles...");
    delay(700);   

    /* caso 1
        tanque 1 cheio. 
        tanque 2 cheio, tudo parado*/
    if(distance[0] <= 37 && distance[1] <= 37){
        digitalWrite(releFiltro1,HIGH);
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,HIGH);
    }
    /* caso 1.1
        tanque 1 cheio. 
        tanque 2 no 1º nivel, Furo não toma qualquer ação*/
    if(distance[0] <= 37 && distance[1] > 37 && distance[1] <= 47){
        digitalWrite(releFiltro1,HIGH);
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releValvula3,HIGH);      
        //digitalWrite(releFuro1,HIGH);
    }
    /* caso 1.2
        tanque 1 cheio. 
        tanque 2 no 2º nivel, valvula e filtro não tomam ação*/
    if(distance[0] <= 37 && distance[1] > 47 && distance[1] <= 67){
        //digitalWrite(releFiltro1,HIGH);
        digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);
        digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
    }
    /* caso 1.3
        tanque 1 cheio. 
        tanque 2 no 3º nivel, se tanque 5 mais de meio, abre valvula e liga filtro para tanque 2*/
    if(distance[0] <= 37 && distance[1] > 67 && distance[1] <= 205){
        //digitalWrite(releFiltro1,HIGH);                   
        digitalWrite(releFiltro2,HIGH);          
        digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){
            digitalWrite(releValvula1,LOW);
            delay(5000);
            digitalWrite(releFiltro1,LOW);              
        }
        else{
            digitalWrite(releFiltro1,HIGH);
            digitalWrite(releValvula1,HIGH);
        }
    }
    /* caso 1.4
        tanque 1 cheio. 
        tanque 2 vazio, se tanque 5 mais de meio, abre valvula e liga filtro para tanque 2*/
    if(distance[0] <= 37 && distance[1] > 205){                 
        digitalWrite(releFiltro2,HIGH);          
        digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){
            digitalWrite(releValvula1,LOW);
            delay(5000);
            digitalWrite(releFiltro1,LOW);
        }
        else{
            digitalWrite(releFiltro1,HIGH);
            digitalWrite(releValvula1,HIGH);
        }   
    }
    /* caso 2
        tanque 1 no 1ºnivel, filtros e valvula não tomam ação. 
        tanque 2, cheio furo parado*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] <= 37){
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        //digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,HIGH);
    }
    /* caso 2.1
        tanque 1 no 1º nivel, filtros e valvula não tomam ação. 
        tanque 2 no 1º nivel, valvula fechada, furo não toma ação*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 37 && distance[1] <= 47){
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        //digitalWrite(releValvula3,HIGH);      
        //digitalWrite(releFuro1,HIGH);
    }
    /* caso 2.2
        tanque 1 no 1º nivel, filtros e valvula não tomam ação. 
        tanque 2 no 2º nivel, valvula não toma ação, furo ligado*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 47 && distance[1] <= 67){
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);
        //digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
    }
    /* caso 2.3
        tanque 1 no 1º nivel, filtros e valvula não tomam ação. 
        tanque 2 no 3º nivel, se tanque 5 mais de meio então liga valvula e filtro*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 67 && distance[1] <= 205){                  
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);          
        //digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110 && distance[1] > 67 && distance[1] <= 205){
            digitalWrite(releValvula1,LOW);
            delay(5000);
            digitalWrite(releFiltro1,LOW);            
        }
        else{            
            digitalWrite(releValvula1,HIGH);
        } 
    }
    /* caso 2.4
        tanque 1 no 1º nivel, filtros e valvula não tomam ação. 
        tanque 2 vazio, se tanque 5 mais de meio então liga valvula e filtro*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 205){              
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);          
        //digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110 && distance[1] > 205){
            digitalWrite(releValvula1,LOW);
            delay(5000);
            digitalWrite(releFiltro1,LOW);
        }
        else{            
            digitalWrite(releValvula1,HIGH);
        }     
    }
    /* caso 3
        tanque 1 no 2º nivel, abre valvula e liga filtro 1. 
        tanque 2 cheio, furo e valvula desligados */
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] <= 37){
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);   
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);          
        digitalWrite(releFuro1,HIGH);
    }
    /* caso 3.1
        tanque 1 no 2º nivel, abre valvula e liga filtro 1. 
        tanque 2 no 1º nivel, furo sem ação */
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 37 && distance[1] <= 47){
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);              
        //digitalWrite(releFuro1,HIGH);
    }
    /* caso 3.2
        tanque 1 no 2º nivel, abre valvula e liga filtro 1. 
        tanque 2 no 2º nivel abre furo*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 47 && distance[1] <= 67){
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);      
        digitalWrite(releFuro1,LOW);
    }
    /* caso 3.3
        tanque 1 no 2º nivel, abre valvula e liga filtro 1. 
        tanque 2 no 3º nivel, se tanque 5 mais de meio, abre valvula*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 67 && distance[1] <= 205){          
        digitalWrite(releValvula3,LOW);
        delay(15000);          
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,HIGH);
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110 && distance[1] > 67 && distance[1] <= 205){              
            digitalWrite(releValvula1,LOW);
        }
        else{              
            digitalWrite(releValvula1,HIGH);
        }
    }
    /* caso 3.4
        tanque 1 no 2º nivel, abre valvula e liga filtro 1. 
        tanque 2 vazio, se tanque 5 mais de meio, abre valvula*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 205){         
        digitalWrite(releValvula3,LOW);
        delay(15000);  
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,HIGH);          
        digitalWrite(releFuro1,LOW);
         if(distance[4] <=110 && distance[1] > 205){              
            digitalWrite(releValvula1,LOW);
        }
        else{              
            digitalWrite(releValvula1,HIGH);
        }
    }
    /* caso 4
        tanque 1 no 3º nivel, abre valvula e liga filtro 1, se tanque 5 mais de meio, abre filtro 2. 
        tanque 2 cheio. */
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] <= 37){          
        digitalWrite(releValvula3,LOW);
        delay(15000); 
        digitalWrite(releFiltro1,LOW);          
        digitalWrite(releValvula1,HIGH);          
        digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110 && distance[0] > 67 && distance[0] <= 215){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        } 
    }
    /* caso 4.1
        tanque 1 no 3º nivel, abre valvula e liga filtro 1, se tanque 5 mais de meio, abre filtro 2. 
        tanque 2 no 1º nivel, furo sem ação.*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] > 37 && distance[1] <= 47){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        digitalWrite(releValvula1,HIGH);
        //digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110 && distance[0] > 67 && distance[0] <= 215){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 4.2
        tanque 1 no 3º nivel, abre valvula e liga filtro 1, se tanque 5 mais de meio, abre filtro 2. 
        tanque 2 no 2º nivel, abre furo*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] > 47 && distance[1] <= 67){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        //digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110 && distance[0] > 67 && distance[0] <= 215){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 4.3
    tanque 1 no 3º nivel, abre valvula e liga filtro 1, se tanque 5 mais de meio, abre filtro 2. 
    tanque 2 no 3º nivel, se tanque 5 mais de meio, abre valvula*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] > 67 && distance[1] <= 205){          
        digitalWrite(releValvula3,LOW);
        delay(15000); 
        digitalWrite(releFiltro1,LOW);                   
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releValvula1,LOW);
            digitalWrite(releFiltro2,LOW); 
        }
        else{              
            digitalWrite(releValvula1,HIGH);
            digitalWrite(releFiltro2,HIGH); 
        } 
    }
    /* caso 4.4
        tanque 1 no 3º nivel, abre valvula e liga filtro 1, se tanque 5 mais de meio, abre filtro 2. 
        tanque 2 vazio, se tanque 5 mais de meio, abre valvula*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] > 205){          
        digitalWrite(releValvula3,LOW);
        delay(15000); 
        digitalWrite(releFiltro1,LOW);                   
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releValvula1,LOW);
            digitalWrite(releFiltro2,LOW); 
        }
        else{              
            digitalWrite(releValvula1,HIGH);
            digitalWrite(releFiltro2,HIGH); 
        } 
    }
    /* caso 5
        tanque 1 vazio, se tanque 5 mais de meio, abre filtro 2.
        tanque 2 cheio*/
    if(distance[0] > 215 && distance[1] <= 37){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);        
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110 && distance[0] > 215){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.1
        tanque 1 vazio, se tanque 5 mais de meio, abre filtro 2.
        tanque 2 no 1º nivel, furo sem ação*/
    if(distance[0] > 215 && distance[1] > 37 && distance[1] <= 47){        
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        digitalWrite(releValvula1,HIGH);
        //digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110 && distance[0] > 215){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.2
        tanque 1 vazio, se tanque 5 mais de meio, abre filtro 2.
        tanque 2 no 2º nivel, abre furo*/
    if(distance[0] > 215 && distance[1] > 47 && distance[1] <= 67){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        //digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110 && distance[0] > 215){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.3
        tanque 1 vazio, se tanque 5 mais de meio, abre filtro 2.
        tanque 2 no 3º nivel, se tanque 5 mais de meio, abre valvula*/
    if(distance[0] > 215 && distance[1] > 67 && distance[1] <= 205){            
        digitalWrite(releValvula3,LOW);
        delay(15000);  
        digitalWrite(releFiltro1,LOW);   
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releValvula1,LOW);
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releValvula1,HIGH);
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.4
        tanque 1 vazio, se tanque 5 mais de meio, abre filtro 2.
        tanque 2 vazio, se tanque 5 mais de meio, abre valvula*/
    if(distance[0] > 215 && distance[1] > 205){          
        digitalWrite(releValvula3,LOW);
        delay(15000);  
        digitalWrite(releFiltro1,LOW);                    
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releValvula1,LOW);
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releValvula1,HIGH);
            digitalWrite(releFiltro2,HIGH);
        }
    }
    
    for (int positionCounter = 0; positionCounter < 12; positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    lcd.clear();    

    /*Medicoes no display*/    
    display();    
}


