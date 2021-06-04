//-------------------------------------------------------Servidor REST
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "INFINITUM1515_2.4";
const char* wpa2 = "aS5zeNLknC";

ESP8266WebServer server(80);
//-------------------------------------------------------Nivel del tanque
#define nivel_lleno D1
#define nivel_medio D2
#define nivel_vacio D0
#define tira_led D7

//-------------------------------------------------------Motor a pasos
#define giro 48
#include <Stepper.h>
Stepper motor (giro, D3, D4, D5,D6 );


//-------------------------------------------------------Pesa
#define pesa A0

//------------------------------------------------------ClienteWeb
#include <ESP8266HTTPClient.h>
#define backend "192.168.7.70"
#define pass "123"
String token ="";

//--------------------------------------------------Reloj
#include <NTPClient.h>
const long utcOffsetInSeconds = -18000;

int encendido= 0;
int millisEncendido = 0;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//----------------------------------------------------Estados
const int ESTADO_SERVER = 1;

void setup() {
  //----------------------------------------------------------------SETUP SERIAL
  Serial.begin(115200);
  //-----------------------------------------------------------------SETUP SERVER REST
  WiFi.begin(ssid, wpa2);
  Serial.println("Conectando a " + (String)ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.print("\nConectado con la IP: ");
  Serial.println( WiFi.localIP());

  inicializarServer();
  //-----------------------------------------------------------------SETUP NIVEL
  pinMode(nivel_lleno, INPUT);
  pinMode(nivel_medio, INPUT);
  pinMode(nivel_vacio, INPUT);
  pinMode(tira_led, OUTPUT);
  digitalWrite(tira_led, LOW);

  //----------------------------------------------------------------SETUP MOTOR
  motor.setSpeed(120); //RPM
   digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);

  //-------------------------------------------------------SETUP PESA
  pinMode(pesa, INPUT);

  //-----------------------------------------------------SETUP CLIENTE WEB
  loginDispositivo();
  registrarNivel();
  inicializarReloj();
  sincronizar();
  Serial.println("Minutos:" + (String) encendido);
}

void loop() {
    server.handleClient();
    }

String buscarArgumento(String busqueda, String cadena){
    int inicio = cadena.indexOf("\""+busqueda+"\":\"");
    int fin = cadena.indexOf("\"", inicio+busqueda.length()+2);

    inicio = fin;
    fin = cadena.indexOf("\"", inicio+1);
    String resultado = cadena.substring(inicio+1,fin);

    return resultado;
  }
