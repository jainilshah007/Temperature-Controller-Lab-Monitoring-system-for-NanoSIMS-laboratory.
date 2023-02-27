/*
  Web Server

 A simple web server that shows the temperature & humidity from
 a DHT11 sensor using an Arduino Wiznet Ethernet shield. 

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Data from DHT11 is at A2 (analog input 2)

 */

#include <DHT.h>

#define DHTPIN 8     // what pin we're connected the DHT output
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>



byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,137, 1776);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  dht.begin();  
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
                    // add a meta refresh tag, so the browser pulls again every 5 seconds:
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          client.println("<title>");
          client.print("Temperature and Humidity");
          client.println("</title>");
          
          // output the value of temperature and humuidity from DHT
          client.println("<center>");
          client.println("<h1>");
          client.print("Data Center");
          client.println("</h1>");
          client.println("<h2>");
          client.print("Server Room Temperature and Humidity");
          client.println("</h2>");
          client.println("<h4>");
          client.print("Temperature : ");
          client.print(t);
          client.print("<sup>0</sup>");
          client.print("C");
          client.println("");
          client.print("Humidity : ");
          client.print(h);
          client.print("%");
          client.println("</h4>");
          client.println("</center>");
           
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}