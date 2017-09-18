
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display. You will need to know where yours is!!!
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <Servo.h>
Servo myservo;

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266HTTPClient.h>
#include <EmonLib.h>

#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

const char* host = "esp8266-http-Update";
String wspeedTemp,bzTemp,btTemp;

int i,j;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  pinMode(0,OUTPUT);
  wspeedTemp = "123.4";
  btTemp = 1;
  bzTemp = 1;
  
  
  // LCD 
  lcd.begin();
  lcd.backlight();
  lcd.print("Initialising...");
  long ver = ESP.getChipId();
  lcd.setCursor(0,1);
  lcd.print("Conn:ESP");
  lcd.print(ver,DEC);
  
  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if(!wifiManager.autoConnect()) 
  {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  
  // This chunk enables the update bin server.
    MDNS.begin(host);
    httpUpdater.setup(&httpServer);
    httpServer.begin();
    MDNS.addService("http", "tcp", 80);
    //Serial.printf("HTTPUpdateServer ready! Open http://%s.local/update in your browser\n", host);

    // Assuming all OK this far, then we're running.
    lcd.clear();
    lcd.print("Running... ");
    lcd.setCursor(1,1);
    lcd.print(WiFi.localIP());
    
}


void loop(void)
{

  
 // for (j=0;j<2;j++)
 // {
     httpServer.handleClient();
     delay(1000);
 // }
  
  HTTPClient http;
//  The data is pulled from MySQL using PHP on my server. This is the address of the page.
  String pageName = "http://www.nkme.co.uk/space.php";

  http.begin(pageName); 
  int httpCode = http.GET();
  
 // The script just returns a string of data that's been 'sprintf'd So it just remains to cut it up.
 // There is no encoding (like JSON) this just adds complexity.
 
  if(httpCode > 0) 
    {    
      String payload = http.getString();     
      digitalWrite(0,HIGH);
      String UTC = payload.substring(16,22);
      lcd.setCursor(0,0);
      lcd.print(UTC);
      String xray = payload.substring(0,4);
      lcd.setCursor(6,0);
      lcd.print(" XRAY:");
      lcd.print(xray);
      delay(10);
      String sfi = payload.substring(5,8);
      lcd.setCursor(0,1);
      lcd.print("SFI:");
      lcd.print(sfi);
      delay(10);
      String a = payload.substring(9,11);
      lcd.setCursor(7,1);
      lcd.print(" A:");
      lcd.print(a);
      delay(10);
      String k = payload.substring(12,13);
      lcd.setCursor(12,1);
      lcd.print(" K:");
      lcd.print(k);
      delay(10);
      for (i=0;i<5;i++)
        {
        httpServer.handleClient();
        delay(1000);
        }
      String ssn = payload.substring(22,26);
      lcd.setCursor(0,1);
      lcd.print("  Sunspots:");
      lcd.print(ssn);
      lcd.print("   ");
      delay(10);
      for (i=0;i<5;i++)
        {
        httpServer.handleClient();  // in each delay loop, it waits for an incoming connection
        delay(1000);                // usually to update the firmware.
        }
        
      String bz = payload.substring(26,29);
      lcd.setCursor(0,1);
      lcd.print("Bz:");
      lcd.print(bz);
      lcd.print(" ");
      String bt = payload.substring(29,33);
      lcd.print("Bt:");
      lcd.print(bt);
      lcd.print(" ");
      delay(10);
      for (i=0;i<5;i++)
        {
        httpServer.handleClient();
        delay(1000);
        }
   
      String wspeed = payload.substring(34,39);
      if (wspeed.toFloat() > 0) wspeedTemp = wspeed;
      if (wspeed.toFloat() == 0) wspeed = wspeedTemp;  // Don't display 0.0 just don't update the value if there isn't a new one.
      lcd.setCursor(0,1);
      lcd.print("Speed:");
      lcd.print(wspeed);
      lcd.print(" km/s ");
      delay(10);
      for (i=0;i<5;i++)
        {
        httpServer.handleClient();
        delay(1000);
        }
      
      
    }
    else
    {
    lcd.clear();
    lcd.print("  Failed"); // The page could not be contacted. 
    lcd.setCursor(0,1);
    lcd.print(httpCode);
    }
    
    http.end();
   
  
  
  
}
