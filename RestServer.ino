void inicializarServer() {
  server.on("/", raiz);
  server.on("/tanque", tanque);
  server.on("/peso", peso);
  server.on("/giro", rotar);
  server.on("/hora", hora);
  server.begin();  
  Serial.println("Servidor REST iniciado");
}

void raiz() {
  server.send(200, "text/html", "<h1>Dispositivo " + (String) WiFi.macAddress() + " funciona</h1>"+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>"+
  "<a href='/hora'>Hora</a>"
  );
}

void tanque() {

  int nivelInicio = getNivel(); 
  server.send(200, "text/html", "nivel=" + (String) nivelInicio+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>"+
  "<a href='/hora'>Hora</a>");

}

void peso() {
  server.send(200, "text/html", "peso=" + (String)getPeso()+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>"+
  "<a href='/hora'>Hora</a>");
}

void rotar(){
    server.send(200, "text/html", (String)"Giro"+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>"+
  "<a href='/hora'>Hora</a>");
    motor.step(giro);
    motor.step(-24);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
  }

  void hora(){
    long millisPasados = millis() - millisEncendido;
    int hora = millisPasados / 1000 / 60 / 60;
    hora += encendido / 60;
    hora%=24;

    int minuto = millisPasados/1000/60;
    minuto += encendido;
    minuto %= 1440;
    
    server.send(200, "text/html",
    "Millis ESP8266: " + (String)millis()
    +"<br>Ultima sincronizacion: "+(String) encendido
    +"<br>Millis al sincronizar:"+ (String) millisEncendido
    +"<br>Millis pasados:" + (String) millisPasados
    +"<br>Hora calculada: "+ (String) hora
    +"<br>Minuto calculado: " + (String) minuto
    +"<br><a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>"+
  "<a href='/hora'>Hora</a>");
    }
