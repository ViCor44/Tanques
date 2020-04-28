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

File webFile;
char *filename;

/************ ETHERNET STUFF ************/
byte mac[] = { 0xDE, 0x45, 0xBE, 0xEF, 0xFE, 0xE6 };
IPAddress ip(191,188,127, 22);
IPAddress gateway(191,188,127,254);
IPAddress subnet(255,255,254,0);
EthernetServer server(8022);

void HtmlHeaderOK(EthernetClient client) {
   
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");                        
    client.println("Cache-Control: no-cache");
    client.println("Cache-Control: no-store");
    client.println("");
}

void HtmlHeader404(EthernetClient client) {
    client.println("HTTP/1.1 404 Not Found");
    client.println("Content-Type: text/xml");                        
    client.println("");
    client.println("<h2>File Not Found!</h2>");
    client.println();
}

void setup() {
    Serial.begin (9600);
    /*Ethernet.begin(mac, ip, dns, gateway, subnet);
    server.begin();
    // see if the card is present and can be initialized:
    if (!SD.begin(4)) {
        Serial.println("Card failed, or not present");
        // don't do anything more:
        return;
    }
    Serial.println("card initialized.");*/   
    
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
    lcd.print("Actual em 28/04/2020");
    for (int positionCounter = 0; positionCounter < 20; positionCounter++) 
    {
      // scroll one position left:
      lcd.scrollDisplayLeft();
      // wait a bit:
      delay(300);
    }
    delay(1500);  
    lcd.clear();
}

void Conection(){
    char clientline[BUFSIZ];
    int index = 0;     
    EthernetClient client = server.available();
    if (client) {
        // an http request ends with a blank line
        boolean current_line_is_blank = true;
       
        // reset the input buffer
        index = 0;       
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
           
                // If it isn't a new line, add the character to the buffer
                if (c != '\n' && c != '\r') {
                    clientline[index] = c;
                    index++;
                    // are we too big for the buffer? start tossing out data
                    if (index >= BUFSIZ)
                        index = BUFSIZ -1;             
                    // continue to read more data!
                    continue;
               }           
               // got a \n or \r new line, which means the string is done
               clientline[index] = 0;
           
              // Print it out for debugging
              //Serial.println(clientline);
           
              // Look for substring such as a request to get the root file
          
              if (strstr(clientline, "ajax_inputs")) {
                  // send rest of HTTP header
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/xml");
                  client.println("Connection: keep-alive");
                  client.println("Cache-Control: no-cache");
                  client.println("Cache-Control: no-store");
                  client.println();
                  // send XML file containing input states
                  XML_response(client);
               }             
               else if (strstr(clientline, "GET /") != 0) {
                  // this time no space after the /, so a sub-file!            
                  filename = strtok(clientline + 5, "?"); // look after the "GET /" (5 chars) but before
                  // the "?" if a data file has been specified. A little trick, look for the " HTTP/1.1"
                  // string and turn the first character of the substring into a 0 to clear it out.
                  (strstr(clientline, " HTTP"))[0] = 0;
                  // print the file we want
                  Serial.println(filename);
                  File file = SD.open(filename,FILE_READ);
                  if (!file) {
                      HtmlHeader404(client);
                      break;
                  }
            
                  Serial.println("Opened!");                       
                  HtmlHeaderOK(client);
             
                  int16_t c;
                  while ((c = file.read()) > 0) {
                      // uncomment the serial to debug (slow!)
                      //Serial.print((char)c);
                      client.print((char)c);
                  }
                  file.close();
               }                
               else {
                  // everything else is a 404
                  HtmlHeader404(client);
               }
               break;
            }
         }
         // give the web browser time to receive the data
         delay(1);
         client.stop();
     }
}

void XML_response(EthernetClient client)
{     
    client.print("<?xml version = \"1.0\" ?>");
    client.print("<inputs>");
    client.print("<tanque1>");
    client.print(level1);
    client.print("</tanque1>");
    client.print("<tanque2>");
    client.print(level2);
    client.print("</tanque2>");
    client.print("<tanque3>");
    client.print(level3);
    client.print("</tanque3>");
    client.print("<tanque4>");
    client.print(level4);
    client.print("</tanque4>");
    client.print("<tanque5>");
    client.print(level5);
    client.print("</tanque5>");
    client.print("</inputs>");
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
  lcd.print("Tanque 1: ");
  lcd.print(level1);
  lcd.print("m3");
  delay(2000);  
  lcd.clear();
  lcd.print("Tanque 2: ");
  lcd.print(level2);
  lcd.print("m3");
  delay(2000);
  lcd.clear();
  lcd.print("Tanque 3: ");
  lcd.print(level3);
  lcd.print("m3");
  delay(2000);
  lcd.clear();
  lcd.print("Tanque 4: ");
  lcd.print(level4);
  lcd.print("m3");
  delay(2000);
  lcd.clear();
  lcd.print("Tanque 5: ");
  lcd.print(level5);
  lcd.print("m3");
  delay(2000);
  lcd.clear();
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

void loop() {   
    
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
          
    distance[0] = measure(trigPinTanque1, echoPinTanque1);
    distance[1] = measure(trigPinTanque2, echoPinTanque2);
    distance[2] = measure(trigPinTanque3, echoPinTanque3);
    distance[3] = measure(trigPinTanque4, echoPinTanque4);
    distance[4] = measure(trigPinTanque5, echoPinTanque5);
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
    
    /*valvula do tanque de alimentação da Osmose 2 sempre fechada*/
    digitalWrite(releValvula2,HIGH);

    /*Actualizaçao dos reles*/        
   
    lcd.setCursor(0, 0);
    lcd.print("A Actualizar");
    lcd.setCursor(0, 1);
    lcd.print("Reles...");
    delay(700);
          
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

    /* caso 1*/
    if(distance[0] <= 37 && distance[1] <= 37){
        digitalWrite(releFiltro1,HIGH);
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,HIGH);
    }
    /* caso 1.1*/
    if(distance[0] <= 37 && distance[1] > 37 && distance[1] <= 47){
        digitalWrite(releFiltro1,HIGH);
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releValvula3,HIGH);      
        //digitalWrite(releFuro1,HIGH);
    }
    /* caso 1.2*/
    if(distance[0] <= 37 && distance[1] > 47 && distance[1] <= 67){
        digitalWrite(releFiltro1,HIGH);
        digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);
        digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
    }
    /* caso 1.3*/
    if(distance[0] <= 37 && distance[1] > 67 && distance[1] <= 205){
        digitalWrite(releFiltro1,HIGH);                   
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
    /* caso 1.4*/
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
    /* caso 2*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] <= 37){
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);
        //digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,HIGH);
    }
    /* caso 2.1*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 37 && distance[1] <= 47){
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);
        //digitalWrite(releValvula3,HIGH);      
        //digitalWrite(releFuro1,HIGH);
    }
    /* caso 2.2*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 47 && distance[1] <= 67){
        //digitalWrite(releFiltro1,HIGH);
        //digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);
        //digitalWrite(releValvula3,HIGH);      
        digitalWrite(releFuro1,LOW);
    }
    /* caso 2.3*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 67 && distance[1] <= 205){                  
        //digitalWrite(releFiltro2,HIGH);          
        //digitalWrite(releValvula3,HIGH);      
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
    /* caso 2.4*/
    if(distance[0] > 37 && distance[0] <= 47 && distance[1] > 205){              
        //digitalWrite(releFiltro2,HIGH);          
        //digitalWrite(releValvula3,HIGH);      
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
    /* caso 3*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] <= 37){
        digitalWrite(releValvula3,LOW);
        delay(15000);
        //digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);          
        digitalWrite(releFiltro1,LOW);      
        digitalWrite(releFuro1,HIGH);
    }
    /* caso 3.1*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 37 && distance[1] <= 47){
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro2,HIGH);
        digitalWrite(releValvula1,HIGH);          
        digitalWrite(releFiltro1,LOW);      
        //digitalWrite(releFuro1,HIGH);
    }
    /* caso 3.2*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 47 && distance[1] <= 67){
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,HIGH);
        //digitalWrite(releValvula1,HIGH);      
        digitalWrite(releFuro1,LOW);
    }
    /* caso 3.3*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 67 && distance[1] <= 205){          
        digitalWrite(releValvula3,LOW);
        delay(15000);          
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,HIGH);
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releValvula1,LOW);
        }
        else{              
            digitalWrite(releValvula1,HIGH);
        }
    }
    /* caso 3.4*/
    if(distance[0] > 47 && distance[0] <= 67 && distance[1] > 205){         
        digitalWrite(releValvula3,LOW);
        delay(15000);  
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,HIGH);          
        digitalWrite(releFuro1,LOW);
         if(distance[4] <=110){              
            digitalWrite(releValvula1,LOW);
        }
        else{              
            digitalWrite(releValvula1,HIGH);
        }
    }
    /* caso 4*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] <= 37){          
        digitalWrite(releValvula3,LOW);
        delay(15000); 
        digitalWrite(releFiltro1,LOW);          
        digitalWrite(releValvula1,HIGH);          
        digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        } 
    }
    /* caso 4.1*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] > 37 && distance[1] <= 47){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 4.2*/
    if(distance[0] > 67 && distance[0] <= 215 && distance[1] > 47 && distance[1] <= 67){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        //digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 4.3*/
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
    /* caso 4.4*/
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
    /* caso 5*/
    if(distance[0] > 215 && distance[1] <= 37){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);
        //digitalWrite(releFiltro2,LOW);
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.1*/
    if(distance[0] > 215 && distance[1] > 37 && distance[1] <= 47){        
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,HIGH);
        if(distance[4] <=110){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.2*/
    if(distance[0] > 215 && distance[1] > 47 && distance[1] <= 67){          
        digitalWrite(releValvula3,LOW);
        delay(15000);
        digitalWrite(releFiltro1,LOW);          
        //digitalWrite(releValvula1,HIGH);
        digitalWrite(releFuro1,LOW);
        if(distance[4] <=110){              
            digitalWrite(releFiltro2,LOW);
        }
        else{              
            digitalWrite(releFiltro2,HIGH);
        }
    }
    /* caso 5.3*/
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
    /* caso 5.4*/
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
      
    //Conection();    
}


