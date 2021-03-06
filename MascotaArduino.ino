//-------------------------------------------------------Servidor REST
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "INFINITUM1515_2.4";
const char* wpa2 = "aS5zeNLknC";

ESP8266WebServer server(80);

const int intentosConexion=1000;
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
#define backend "pw4.kyared.com/S17030221"
#define pass "123"
String token ="";

//--------------------------------------------------Reloj
#include <NTPClient.h>
const long utcOffsetInSeconds = -18000;

int encendido= 0;
int millisEncendido = 0;
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
int conteoHorarios =24;
int horarios[24][2];

int ultimaAlimentacion = 0;
int siguienteAlimentacion = 1440;
int gramosSiguientes = 10;

//----------------------------------------------------Estados
int estado = 1;
const int ESTADO_SERVER = 1;
const int ESTADO_ALIMENTAR = 2;

void setup() {
  //----------------------------------------------------------------SETUP SERIAL
  Serial.begin(115200);
  //-----------------------------------------------------------------SETUP SERVER REST
  WiFi.begin(ssid, wpa2);
  Serial.println("Conectando a " + (String)ssid);

  int intentos = 0;
  while (WiFi.status() != WL_CONNECTED && intentos++ < intentosConexion) {
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
  llenarHorarios();
  Serial.println("Minutos:" + (String) encendido);
}


String buscarArgumento(String busqueda, String cadena){
    int inicio = cadena.indexOf("\""+busqueda+"\":\"");    
    int fin = cadena.indexOf("\"", inicio+busqueda.length()+2);
    inicio = fin;    
    fin = cadena.indexOf("\"", inicio+1);
    
    String resultado = cadena.substring(inicio+1,fin);

    return resultado;
  }

  String buscarArray(String busqueda, String cadena){
    int inicio = cadena.indexOf("\""+busqueda+"\":[");
    inicio+= busqueda.length()+3;    

    int fin = cadena.indexOf("]", inicio);

    return cadena.substring(inicio+1, fin);
    }

    
void loop() {
  if(estado == ESTADO_SERVER){
    if(alimentar()){

      Serial.println("Minuto actual: " + (String) obtenerMinuto());
      Serial.println("Ultima: " + (String) ultimaAlimentacion);
      Serial.println("Siguiente: " + (String) siguienteAlimentacion);
      
      estado=ESTADO_ALIMENTAR;
      
      }
      else{
    server.handleClient();
    }
  } else if (estado == ESTADO_ALIMENTAR){
      if(gramosSiguientes > 0){
        Serial.println("Alimentando: " + (String) gramosSiguientes);
        motor.step(giro);
        motor.step(-24);

        gramosSiguientes--;

        delay(500);

        
        } else {
          repararHorarios();
          registrarNivel();
          estado = ESTADO_SERVER;
          Serial.println("Actuando como server");
          }
    }
    }
