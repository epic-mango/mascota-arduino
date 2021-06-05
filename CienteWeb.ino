void loginDispositivo() {
  WiFiClient cliente;
  HTTPClient http;

  if (http.begin(cliente, "http://" + (String) backend + "/dispositivos.php?mac=" + (String)WiFi.macAddress() + "&pass=" + (String)pass)) {

    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();

        if (buscarArgumento("login", payload) == "true") {
          token = buscarArgumento("token", payload);
          Serial.println("Dispositivo logeado correctamente");
        } else {
          Serial.println("Error de autentificación: " + payload);
        }
      }
    }  else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }
}

void registrarNivel() {


  WiFiClient cliente;
  HTTPClient http;

  if (http.begin(cliente, "http://" + (String) backend + "/sensores.php")) {
    http.addHeader("Authorization", token);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpCode = http.POST("mac=" + (String)WiFi.macAddress() + "&tipo=tan&valor=" + (String)getNivel());

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = http.getString();

        if (buscarArgumento("agregado", payload) == "true") {

          Serial.println("Valor del tanque registrado correctamente");
        } else {
          Serial.println("NO se ha registrado el valor del tanque" + payload);
        }
      } else {
        Serial.println("Error al registrar nivel del tanque (HTTP Code): " + (String)httpCode);
      }
    }  else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
  }
}
