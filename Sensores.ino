
int getNivel() {
  digitalWrite(tira_led, HIGH);

  delay(1000);
  int n1 = digitalRead(nivel_lleno);
  int n2 = digitalRead(nivel_medio);
  int n3 = digitalRead(nivel_vacio);

  int nivel = 7;

  if (n1) nivel -= 4;
  if (n2) nivel -= 2;
  if (n3) nivel -= 1;
  
  digitalWrite(tira_led, LOW);

  return nivel;
}

float getPeso(){
  float peso = analogRead(pesa);
  Serial.println(peso);
  return peso;
  
  }
