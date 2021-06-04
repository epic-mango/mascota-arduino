void inicializarServer() {
  server.on("/", raiz);
  server.on("/tanque", tanque);
  server.on("/peso", peso);
  server.on("/giro", rotar);
  server.begin();  
  Serial.println("Servidor REST iniciado");
}

void raiz() {
  server.send(200, "text/html", "<h1>Dispositivo " + (String) WiFi.macAddress() + " funciona</h1>"+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>"
  );
}

void tanque() {

  int nivelInicio = getNivel(); 
  server.send(200, "text/html", "nivel=" + (String) nivelInicio+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>");

}

void peso() {
  server.send(200, "text/html", "peso=" + (String)getPeso()+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>");
}

void rotar(){
    server.send(200, "text/html", (String)"Giro"+
  "<a href='/giro'>Giro</a><br>"+
  "<a href='/tanque'>Tanque</a><br>"+
  "<a href='/peso'>Peso</a>");
    motor.step(giro);
    motor.step(-24);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
  }
