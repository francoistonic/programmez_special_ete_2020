/*
  D'après Simple Wifi Switch de Jeffrey Roshan
  https://gist.github.com/jeffreyroshan/
  
  Adaptation Française : François Mocq
  www.framboise314.fr
  */

// Inclure les bibliothèques utilisées dans le script

// Gestion du WifI pour l'ESP8266
#include <ESP8266WiFi.h>

// Client capable de se connecter à une adresse IP
#include <WiFiClient.h>

// Serveur web capable de gérer les requêtes HTTP POST et GET d'un client unique
#include <ESP8266WebServer.h>


// Identifiants de connexion à la box
const char* ssid = "Livebox-57A9";
const char* password = "258xxxxxxxxxxxxxxxxx279";


ESP8266WebServer server(80);

String web_on_html = "<h2>Etat actuel : ON</h2><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";
String web_off_html = "<h2>Etat actuel : OFF</h2><p><a href=\"on\"><button>ON</button></a>&nbsp;<a href=\"off\"><button>OFF</button></a></p>";

int gpio_13_led = 13;
int gpio_12_relais = 12;

void setup(void){  
  //  Initialisation des sorties LED et Relais
  // Eteindre la LED
  pinMode(gpio_13_led, OUTPUT);
  digitalWrite(gpio_13_led, HIGH);

  // Décoller le Relais
  pinMode(gpio_12_relais, OUTPUT);
  digitalWrite(gpio_12_relais, LOW);
 
  // Configurer la vitesse du port série
  Serial.begin(115200); 
  delay(5000);

  // Connecter l'ESP8266 au WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connection au WiFi en cours...");

  // Attendre la connexion en faisant clignoter la LED
  while (WiFi.status() != WL_CONNECTED) {
    // Allumer la LED
    digitalWrite(gpio_13_led, LOW);
    delay(500);
    Serial.print(".");
    // Attente de l'adresse IP
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.status());
    // Eteindre la LED
    digitalWrite(gpio_13_led, HIGH);
    delay(500);
  }
  
  Serial.println("");
  Serial.print("Connecté à ");
  Serial.println(ssid);
  Serial.print("Accédez au serveur à l'adresse : ");
  Serial.println(WiFi.localIP());
  
  // Réponse du serveur  
  server.on("/", [](){
    if(digitalRead(gpio_12_relais)==HIGH) {
      server.send(200, "text/html", web_on_html);
    } else {
      server.send(200, "text/html", web_off_html);
    }
  });
  
  // Réponse lors de l'appui sur ON
  server.on("/on", [](){
    server.send(200, "text/html", web_on_html);
    // Allumer la LED, coller le Relais
    digitalWrite(gpio_13_led, LOW);
    digitalWrite(gpio_12_relais, HIGH);
    delay(1000);
  });
  
  // Réponse lors de l'appui sur OFF
  server.on("/off", [](){
    server.send(200, "text/html", web_off_html);
    // Eteindre la LED, décoller le Relais
    digitalWrite(gpio_13_led, HIGH);
    digitalWrite(gpio_12_relais, LOW);
    delay(1000); 
  });
  
// Démarrage du serveur web
  server.begin();
  Serial.println("Serveur web prêt..");
}
 
// Boucle principale du programme
// Gestion du client web 
void loop(void){
  server.handleClient();
} 
