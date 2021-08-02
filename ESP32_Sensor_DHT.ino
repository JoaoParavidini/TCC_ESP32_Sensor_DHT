//Bibliotecas requisitadas
#include "WiFi.h"
#include "ESPAsyncWebServer.h" 
#include <Adafruit_Sensor.h>
#include <DHT.h>

//Credenciais de internet local
const char* ssid = "LORENA";
const char* password = "56736212";

#define DHTPIN 27     // Digital pin conectado ao sensor DHT

#define DHTTYPE    DHT11     //Sensor que está sendo usado

DHT dht(DHTPIN, DHTTYPE);

//Cria objeto AsyncWebServer na porta 80
AsyncWebServer server(80);

String readDHTTemperature() {
  //Leitura de Temperatura em Celcius
  float t = dht.readTemperature();
  
  if (isnan(t)) {    
    Serial.println("Falha na leitura de Temperatura do sensor DHT!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
    //Função que converte float e string
  }
}

String readDHTHumidity() {
  //Leitura de Humidade
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Falha na leitura de Humidade do sensor DHT!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
    //Função que converte float e string
  }
}

//META - Site funcionar em qualquer browser de internet
//LINK - Carregar os ícones
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>

<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">

  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>

<body>
  <meta charset = UTF-8>
  <h3>João Vitor Paravidini</h3>
  <h3>Técnico em Informática</h3>
  <h3>Trabalho de Conclusão de Curso</h3>
  <p align = center>
  <h2>ESP32 - Sensor DHT</h2>
  </p>
  <p align = center>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p align = center>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 10000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 10000 ) ;
</script>
</html>)rawliteral";

// Substituir espaços da leitura do sensor DHT no código HTML
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  return String();
}

void setup(){
  
  Serial.begin(115200);

  dht.begin();
  
  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Impressão do Endereço IP local do ESP32
  Serial.println(WiFi.localIP());

  // Rota para o servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.begin();
}
 
void loop(){
  
}
