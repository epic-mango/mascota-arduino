
void inicializarReloj(){
  timeClient.begin();
  }

void sincronizar(){
    timeClient.update();
    Serial.println("Hours:" + (String)timeClient.getHours());
    Serial.println("Minutes:" + (String)timeClient.getMinutes());

    int minuto = timeClient.getHours() * 60;
    minuto += timeClient.getMinutes();
  
    encendido = minuto;
    millisEncendido = millis();
}

int obtenerMinuto(){
  
    long millisPasados = millis() - millisEncendido;
    int minuto = millisPasados/1000/60;
    minuto += encendido;
    minuto %= 1440;
  }
