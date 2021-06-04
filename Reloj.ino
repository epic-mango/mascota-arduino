
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

    return minuto;
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

        Serial.println("Correcto: " + payload);  

        if(buscarArgumento("estado",payload)=="true"){

          String arr = buscarArray("datos",payload);

        conteoHorarios=0;
        for(int i = 0; i < arr.length(); i++){
          int inicio = arr.indexOf("{", i);
          int fin = arr.indexOf("}", i);

          String extraida = buscarArgumento("minuto", arr.substring(inicio,fin));
          horarios[conteoHorarios][0] = extraida.toInt();

          extraida = buscarArgumento("gramos", arr.substring(inicio,fin));
          horarios[conteoHorarios][1] = extraida.toInt();

          conteoHorarios++;
          i=fin;
          }

          for(int i = 0; i < conteoHorarios ; i++){
            Serial.println("Minuto: " + (String)horarios[i][0] + " Gramos: "+(String)horarios[i][1]);
            }
        }
        
      
    }  else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

      
  
  }
    repararHorarios();
  
  }

  boolean alimentar(){

    delay(500);
    
    if(obtenerMinuto() > ultimaAlimentacion && obtenerMinuto() > siguienteAlimentacion)
      return true;
    else if(obtenerMinuto ()> siguienteAlimentacion &&obtenerMinuto ()< ultimaAlimentacion)
      return true;
    else
      return false;
    }

    void repararHorarios(){

      Serial.println("Reparando horarios");

      ultimaAlimentacion = obtenerMinuto();
    Serial.println("Reparando horarios B");
      siguienteAlimentacion = 1440;

      Serial.println("Reparando horarios C");
      for(int i = 0; i < conteoHorarios; i++){

        Serial.println("Reparando horarios D" + (String) i);
        if(siguienteAlimentacion > horarios[i][0] && horarios[i][0] > ultimaAlimentacion)
        siguienteAlimentacion = horarios[i][0];
        gramosSiguientes = horarios[i][1];
        }

        if(siguienteAlimentacion==1440)
          {
          
            for(int i = 0 ; i < conteoHorarios; i++){
              Serial.println("Reparando horarios E" + (String) i);
              if(siguienteAlimentacion > horarios[i][0])
              siguienteAlimentacion = horarios[i][0];
              gramosSiguientes = horarios[i][1];
              }
          }

      Serial.println("Siguiente: "+ (String) siguienteAlimentacion +" " +(String) gramosSiguientes + " gramos");
      }
