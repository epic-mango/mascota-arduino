
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

  void llenarHorarios(){
    
  WiFiClient cliente;
  HTTPClient http;

  if (http.begin(cliente, "http://" + (String) backend + "/mascotarest/horarios.php?mac=" + (String)WiFi.macAddress())) {
    http.addHeader("Authorization", token);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    int httpCode = http.GET();

      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();

          Serial.println("Correcto: " + buscarArray("datos",payload));
        
      
    }  else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  
  }
  }
