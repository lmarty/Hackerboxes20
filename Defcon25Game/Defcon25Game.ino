

#include <mySD.h>
#include "WiFi.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_NeoPixel.h"
#include "Mac.h"
#include "ESP8266WiFi.h"
//#include <IoTtweetESP32.h>
#include <ArduinoJson.h>
//#include "user_interface.h"
//#include "ip_addr.h"
//#include <Adafruit_ESP8266.h>



//DEVICES
// TFT Values
#define TFT_CS   19
#define TFT_DC   22
#define TFT_MOSI 23
#define TFT_CLK  26
#define TFT_RST  21
#define TFT_MISO 25

//NEOPIXEL Values
#define PIXELPIN  5
#define NUMPIXELS 5
#define pixlux   20

// Audio Buzzer Values
const int buzzerPin = 18;
const int f = 349;
const int gS = 415;
const int a = 440;
const int cH = 523;
const int eH = 659;
const int fH = 698;
const int e6 = 1319;
const int g6 = 1568;
const int a6 = 1760;
const int as6 = 1865;
const int b6 = 1976;
const int c7 = 2093;
const int d7 = 2349;
const int e7 = 2637;
const int f7 = 2794;
const int g7 = 3136;

// for deauth
extern "C" {
  #include "user_interface.h"
}


//===== Deauth Detect SETTINGS =====//
#define channel 1 //the channel it should scan on (1-14)
#define channelHopping true //scan on all channels
#define maxChannel 13 //US = 11, EU = 13, Japan = 14
#define ledPin 2 //led pin ( 2 = built-in LED)
#define inverted true // invert HIGH/LOW for the LED
#define packetRate 3 //min. packets before it gets recognized as an attack
//=================================//


//================deauth vars===========//
//Mac from;
//Mac to;
unsigned long c = 0;
unsigned long prevTime = 0;
unsigned long curTime = 0;
int curChannel = channel;
//=======================================//

#define scanTime 500 //scan time per channel in ms


// init the display and leds
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

// setup our game variables
char hackers_found[13][70];
int next_hacker_found = 0;
int mute_touched = false;
char ssid[]="HackerBoxer_MrmuthafuckinE";  //put your handle after the underscore
int muted = true; // default to muted
unsigned long debounce;
char infectionState = "clean"; // clean, infected, carrier



// setup our sd card object
File root;

// run our main setup function
void setup()
{
  debounce = millis();
  Serial.begin(115200);
  Serial.println("Firing up our setup function");
  Serial.print("Initializing SD card...");

  /* initialize SD library with SPI pins */
  if (!SD.begin(17, 16, 4, 0)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //===============deauth =============//
  wifi_set_opmode(STATION_MODE);
wifi_promiscuous_enable(0);
WiFi.disconnect();
wifi_set_promiscuous_rx_cb(sniffer);
wifi_set_channel(curChannel);
wifi_promiscuous_enable(1);

//   pinMode(ledPin, OUTPUT); // find another notifier!!

//===================================//

  /* load the root object with the contents of the filesystem "/" */
  root = SD.open("/");
  if (root) {
    printDirectory(root, 0);
    root.close();
  } else {
    Serial.println("error opening root FS.. we can likely recover");
  }


  delay(1000);
  /* open our seen ssids file and read it out on the serial */
  root = SD.open("seen.txt");
  if (root) {
    /* read from the file until there's nothing else in it */
    Serial.println("All the SSIDs in seen.txt");
    while (root.available()) {
      /* read the file and print to Terminal */
      Serial.write(root.read());
    }
    root.close();
  } else {
    Serial.println("error opening seen.txt from line91");
  }

    // start the screen
  tft.begin();
  tft.setRotation(3); // rotate 3*(pi/2)

  //GFX lib start for the leds
  pixels.begin();

  // Set our buzzer pin as an OUTPUT
  pinMode(buzzerPin, OUTPUT);

  // hacker count array. Loop through till we hit the linelimit of the screen using this font.
  for (int i = 0; i<13; i++)
  {
    hackers_found[i][0] = 0; //empty array of strings
    touchAttachInterrupt(15, mutebutton, 40);  //threshold 40. why is this in this loop? TODO: Investigate
  }
}

void loop()
{
  // display BitHead HackerBoxes Mascot
  BitHead2LCD();

  // cycle some NeoPixel Rainbows
  RainbowLEDcycle(17);

  // scan other SSIDs
  wifiScan2LCD();

  // cycle some NeoPixel Rainbows
  RainbowLEDcycle(18);

  // start broadcating SSID (AP on)
  WiFi.softAP(ssid, NULL, 1, 0, 1);

  // Play Mario Theme on Buzzer TODO : Fix this
//  mute_handler();
//  if (!muted)
//    MarioTheme();

  // chill here for a while
  delay(10000);

  //diplay list of found hackers tagged
  found2LCD();

  // cycle some NeoPixel Blues
  BlueLEDcycle(18);

  // Play Imperial March on Buzzer TODO : fix this
//  mute_handler();
//  if (!muted)
//    ImperialMarch();

  // chill here for a while
  delay(10000);

  // stop broadcating SSID (AP off)
  WiFi.softAPdisconnect(1);

  // ======== start deauth detect loop==========//

  curTime = millis();

  if(curTime - prevTime >= scanTime){
    prevTime = curTime;
    Serial.println((String)c);
    // FIND A BETTER NOTIFICATION
    if(c >= packetRate){
      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_YELLOW);
      tft.setTextSize(4);
      tft.println(" SOMEONE IS RUNNING A DEAUTH ATTACK");
      tft.setTextColor(ILI9341_RED);
      tft.setTextSize(2);
    }else{
      tft.setCursor(0, 0);
      tft.setTextColor(ILI9341_YELLOW);
      tft.setTextSize(4);
      tft.println(" Not seeing any deauths right now");
      tft.setTextColor(ILI9341_RED);
      tft.setTextSize(2);
    }

    c = 0;
    if(channelHopping){
      curChannel++;
      if(curChannel > maxChannel) curChannel = 1;
      wifi_set_channel(curChannel);
    }
  }
  //================= end deauth ================//
}

//
// OUR FUNCTIONS
//

//
// Need implementation
//

void otaserver() {
  // webserver component required for OTA updates.  Will hold binaries for downloading, and that's it
  // pin to a core and run constantly if possible.
};

void getupdate() {
  // when connected to a hackerboxes AP, check to see if there is an update available
};

void checkInfected(){
  //when connected to a hackerboxes AP, check if remote end is infected.
};

void infectedFreakout(){
  // go batshit if in contact with an infected badge
};

void transferInfection(){
  // transfer our virus(s) and change infected state
};

void showInfection(){
  //show infection state on the main badge loop
};



void mute_handler() // if the mut button has been touched, flip variable
{
  if (mute_touched)
  {
    if (muted)
    {
      muted = false;
    }
    else
    {
      muted = true;
    }
    mute_touched = false;
  }
}

void wifiScan2LCD()
{
  int netsfound;
  int displaylines=13;

  tft.fillScreen(ILI9341_BLACK);
  paint_mute_indicator();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(4);
  tft.println(" Nets Scanned");
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);

  while (displaylines > 0)
  {
    netsfound = WiFi.scanNetworks();
    if (netsfound==0)
    {
      tft.println(". . .");
      displaylines--;
    }
    for (int i = 0; i < netsfound; ++i)
    {
      if (WiFi.SSID(i).startsWith("HackerBoxer"))
      {
        WiFi.SSID(i).toCharArray(hackers_found[next_hacker_found],70);
        hackers_found[next_hacker_found][25] = 0;  //truncate for display
        next_hacker_found++;
        if (next_hacker_found == 13)
          next_hacker_found = 0;
      }
      else
      {
        // Print SSID and RSSI for each network found
        tft.print(" [");
        tft.print(WiFi.RSSI(i));
        tft.print("] ");
        tft.print(WiFi.SSID(i).substring(0,17));
        tft.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(50);
        displaylines--;
        // LOG our findings TODO : Break out to its own function
        //Open the seen file
        root = SD.open("seen.txt", FILE_WRITE);
        // if the file is available, write to it TODO : Only write first encounter info, do not log the same again for 5 mins.
        if (root) {
          root.print(" [");
          root.print(netsfound);
          root.println("] ");
          root.print(" [");
          root.print(WiFi.RSSI(i));
          root.println("] ");
          root.println(WiFi.SSID(i).substring(0,17));
          root.println(WiFi.encryptionType(i));
          root.close();
        }
      }
    }
  }
}

//===================deauth funcs =============//

void sniffer(uint8_t *buf, uint16_t len) {
  //if(len>27){
    //from.set(buf[16],buf[17],buf[18],buf[19],buf[20],buf[21]);
    //to.set(buf[22],buf[23],buf[24],buf[25],buf[26],buf[27]);

    if(buf[12] == 0xA0 || buf[12] == 0xC0){
      /*Serial.print("From ");
      from._println();
      Serial.print("To ");
      to._println();
      Serial.println();*/

      c++;
    }

  //}
}


// Our funtion to loop throught the filesystem and print index to serial
void printDirectory(File dir, int numTabs) {

  while(true) {
     File entry =  dir.openNextFile();
     if (! entry) {
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');   // we'll have a nice indentation
     }
     // Print the name
     Serial.print(entry.name());
     /* Recurse for directories, otherwise print the file size */
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       /* files have sizes, directories do not */
       Serial.print("\t\t");
       Serial.println(entry.size());
     }
     entry.close();
   }
}


void found2LCD()
{
  tft.fillScreen(ILI9341_BLACK);
  paint_mute_indicator();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(4);
  tft.println(" TAG ur iT");
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(2);
  for (int i=0; i<13; i++)
  {
    tft.print(" ");
    tft.println(hackers_found[i]+12);
    Serial.println(hackers_found[i]+12);
  }
}

// setup led funcs
void RainbowLEDcycle(int cycles)
{
  int i=0;
  while(cycles)
  {
//    pixels.setPixelColor(i, pixels.Color(pixlux,0,0));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(pixlux,pixlux,0));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(0,pixlux,0));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(0,0,pixlux));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(pixlux,0,pixlux));
    i = (i==4) ? 0 : (i+1);
    i = (i==4) ? 0 : (i+1);
//    pixels.show();
    delay(150);
    cycles--;
  }
}

void BlueLEDcycle(int cycles)
{
  int i=0;
  while(cycles)
  {
//    pixels.setPixelColor(i, pixels.Color(0,0,pixlux*2));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
//    pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
 //   pixels.setPixelColor(i, pixels.Color(0,0,pixlux/2));
    i = (i==4) ? 0 : (i+1);
    i = (i==4) ? 0 : (i+1);
//    pixels.show();
    delay(150);
    cycles--;
  }
}

//  changes state as pad is touched
void mutebutton()
{
  if ((millis() - debounce) > 100)
  {
    debounce = millis();
    if (muted)  {
      muted = false;
    }
    else {
      muted = true;
    }
  paint_mute_indicator();
  }
}

void paint_mute_indicator() {
  if (muted) {
    tft.fillRect(0, 0, 18, 18, ILI9341_RED);
  }
  else {
    tft.fillRect(0, 0, 18, 18, ILI9341_GREEN);
  }
}

void BitHead2LCD() //Brute-force TFT bit blit of the HackerBoxes mascot BitHead
{
char *bithead[] = {
"00000000000000000000000000000000000000000000000000001111111111111111111110000000000000000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000000000111111111111111111111111111111110000000000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000001111111111111111111111111111111111111111000000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000001111111111111111111111111111111111111111111111100000000000000000000000000000000000000000000",
"00000000000000000000000000000000000001111111111111111111111111111111111111111111111111111100000000000000000000000000000000000000000",
"00000000000000000000000000000000000111111111111111111111111111111111111111111111111111111111100000000000000000000000000000000000000",
"00000000000000000000000000000000011111111111111111111100000000000000000000000111111111111111111000000000000000000000000000000000000",
"00000000000000000000000000000001111111111111111110000000000000000000000000000000011111111111111110000000000000000000000000000000000",
"00000000000000000000000000000111111111111111100011110000011100000001110000001110000011111111111111100000000000000000000000000000000",
"00000000000000000000000000001111111111111111000111111000011110000011111000011110000011111111111111111000000000000000000000000000000",
"00000000000000000000000000111111111111110011000110011100001110000111001100000110000011001111111111111110000000000000000000000000000",
"00000000000000000000000001111111111111100011101110001100000110000110001100000110000111001110111111111111000000000000000000000000000",
"00000000000000000000000011111111111101100011101100001100001110000110001100000110000110000110011111111111110000000000000000000000000",
"00000000000000000000000111111111110001100011101100001100001110000110001100000110000110000110000111111111111000000000000000000000000",
"00000000000000000000001111111111100001100011101110001100000110000110001100000110000111001110000111111111111100000000000000000000000",
"00000000000000000000011111111111100001110011000110011100000110000110001100000110000011001100000111111111111110000000000000000000000",
"00000000000000000000111111111111111000111111000111111000111111100011111000011111100011111100011111111111111111000000000000000000000",
"00000000000000000001111111111111111000011110000011110000111111100001111000011111100001111000011111100111111111100000000000000000000",
"00000000000000000011111111110000000000000000000000000000000000000000000000000000000000000000000000000011111111110000000000000000000",
"00000000000000000111111111100000000000000000000000000000000000000000000000000000000000000000000000000001111111111000000000000000000",
"00000000000000001111111111100001111000001111100001111000000111000000111110000111110000111100000011110000111111111100000000000000000",
"00000000000000001111111111100001111000011101110001111000001111000001110110001110111000111100000111111000011111111110000000000000000",
"00000000000000011111111101110000011000011000110000011000000011000001100011001100011000001100001110011100001111111110000000000000000",
"00000000000000111111111000110000011000011000110000011000000011000001100011001100011000001100001110001100001111111111000000000000000",
"00000000000000111111111000110000011000011000111000011000000011000001100011001100011000001100001100001100001111111111100000000000000",
"00000000000001111111111000110000011000011000110000011000000011000001100011001100011000001100001100001100001111111111100000000000000",
"00000000000001111111111000110000011000011000110000011000000011000001100011001100011000001100001110011100000101111111110000000000000",
"00000000000011111111011001100000111000011101110000011000000011100001110111001110111000001100000110011000001110111111110000000000000",
"00000000000011111110011111100011111110001111100001111110001111111000111110000111110000111111000111111000111111111111111000000000000",
"00000000000111111110000111000001111100000111000000111100000111110000011100000011100000111110000001100000011111011111111000000000000",
"00000000000111111100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111100000000000",
"00000000001111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111111100000000000",
"00000000001111111000111110000111110000011100000011100000011111000011111000011110000001111000001111000001111000001111111110000000000",
"00000000001111110001111111000111111000111100000011100000111111100011111100011110000011111100001111000001111000001111111110000000000",
"00000000011111110001100011001110011000001100000001100000110001100110001100000110000011001110000011000000011000000011111110000000000",
"00000000011111100001100011001100011000001100000001100000110001100110001100000110000111000110000011000000011000000011111111000000000",
"00000000011111100001100011001100011100001100000001100000110001100110001110000110000111000110000011000000011000000011111111000000000",
"00000000111111100001100011001100011100001100000001100000110001100110001100000110000111000110000011000000011000000011111111000000000",
"00000000111111100001100011001100011000001100000001100000110001100110001100000110000111000110000011000000011000000011111111100000000",
"00000000111111100001110111001110011000001100000001110000111011100111011100000110000011001100000111000000011000000011111111100000000",
"00000000111111111000111110000111110000111111000111111100011111000011111000011111100011111100011111110001111110001111111111100000000",
"00000001111111110000011100000011100000111111000011111000001110000001110000011111100000111000001111100001111100001111111111100000000",
"00000001111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111100000000",
"00000001111110000011000000001000000001000000001000000000100000000100000001100000000100000000110000000010000000010000011111110000000",
"00000001111100001111100001111000000111110000111100000111100000111100000011111000011111000011111000011110000001110000001111110000000",
"00000001111100001101110000111000001110111000111100000111100000011100000111011000111011100011011100011110000001110000011111110000000",
"00000011111100011100110000011000001100011000001100000001100000001100000110001100110001100110001100000110000000110000011111110000000",
"00000011111100011000111000011000001100011000001100000001100000001100001110001100110001100110001110000110000000110000011111110000000",
"00000011111100011000111000011000011100011000001100000001100000001100001110001100110001100110001110000110000000110000011111110000000",
"00000011111000011000111000011000001100011000001100000001100000001100001110001100110001100110001110000110000000110000011111111000000",
"00000011111000011100110000011000001100011000001100000001100000001100000110001100110001100110001100000110000000110000011111111000000",
"00000011111100001101110000011100001110111000011100000001110000001110000111011100111011100111011100000111000000111000011111111000000",
"00000011111110001111100001111110000111110001111111000111111000111111000011111000011111000011111000011111100011111110001111111000000",
"00000011111000000011000000000000000011000000000000000000000000000000000001100000001100000000110000000000000000000000000111111000000",
"00000011111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000011111001110000001110000001111000001111000000110000000111000000111000000011000000011100000011100000011100000001100111111000000",
"00000111110011111000011110000011111100011111100001111000001111000001111110001111000001111110000111100000111111000111100011111000000",
"00000111110110011100000110000111001100011001100000111000000011000011100110000011000001100111000001100001110011000001100011111000000",
"00000111111110001100000110000110001110111000110000011000000011000011000110000011000001100011000001100001100011000001100011111000000",
"00000111111110001100000110000110001110110000110000011000000011000011000111000011000011100011000001100001100011100001100011111000000",
"00000111111110001100000110000110001110110000110000011000000011000011000111000011000011100011000001100001100011100001100011111000000",
"00000111111110001100000110000110001110111000110000011000000011000011000111000011000001100011000001100001100011000001100011111000000",
"00000111110110001100000110000111001100011001110000011000000011000011100110000011000001100111000001100001110011000001100011111000000",
"00000111110111111000011111100011111100011111100011111100001111110001111110001111110001111110000111111000111111000111111111111000000",
"00000111110011111000011111100001111000001111000011111110001111110000111100001111110000111100001111111000011110000111111111111000000",
"00000111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111000000",
"00000111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111110000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000111111000000",
"00000111111000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001111110000000",
"00000111111000000010000000000000000111111100000000000000000000000000000000000000000000000000000000000000000000000000001111110000000",
"00000011111100000110000000000001111111111111110000000000000000000000000000000000011111111111111000000000000000100000001111110000000",
"00000011111100001110000000000111111111111111111100000000000000000000000000000001111111111111111111000000000001100000001111110000000",
"00000011111100001110000000011111111111111111111111000000000000000000000000000111111111111111111111110000000001100000011111110000000",
"00000011111100001100000000111111111111111111111111100000000000000000000000001111111111111111111111111100000001110000011111110000000",
"00000011111100011100000001111111111111111111111111110000000000000000000000011111111111111111111111111110000001110000011111100000000",
"00000011111110011100000011111111111111111111111111111000000000000000000000111111111111111111111111111111000001110000011111100000000",
"00000011111110011000000111111111111111111111111111111100000000000000000001111111111111111111111111111111100001110000111111100000000",
"00000011111110011000001111111111111111111111111111111110000000000000000001111111111111111111111111111111110001111000111111100000000",
"00000011111111011000011111111111111111111111111111111110000000000000000001111111111111111111111111111111110001111000111111100000000",
"00000011111111111000011111111111111111111111111111111111000000000000000001111111111111111111111111111111111001111001111111000000000",
"00000001111111111000111111111111111111111111111111111111000000000000000011111111111111111111111111111111111001111001111111000000000",
"00000001111111110000111111111111111111111111111111111111000000000000000011111111111111111111111111111111111101111011111111000000000",
"00000001111111110000111111111111111111111111111111111111000000000000000011111111111111111111111111111111111101111011111110000000000",
"00000001111111110001111111111111111111111111111111111110000000000000000001111111111111111111111111111111111100111111111110000000000",
"00000001111111110001111111111111111111111111111111111110000000000000000001111111111111111111111111111111111100111111111110000000000",
"00000001111111110000111111111111111111111111111111111100000000000000000001111111111111111111111111111111111100111111111100000000000",
"00000000111111100000111111111111111111111111111111111000000000000000000001111111111111111111111111111111111100111111111100000000000",
"00000000111111100000111111111111111111111111111111110000000000000000000001111111111111111111111111111111111100011111111100000000000",
"00000000111111100000111111111111111111111111111111100000000000000000000000111111111111111111111111111111111100011111111000000000000",
"00000000111111100000011111111111111111111111111111100000000000000000000000111111111111111111111111111111111000011111111000000000000",
"00000000111111100000011111111111111111111111111110000000000000000000000000011111111111111111111111111111111000001111110000000000000",
"00000000111111100000001111111111111111111111111100000000000000000000000000001111111111111111111111111111111000001111110000000000000",
"00000000111111100000000111111111111111111111111000000000000000000000000000000111111111111111111111111111110000001111111000000000000",
"00000000111111100000000111111111111111111111110000000000000000000000000000000011111111111111111111111111100000000111111000000000000",
"00000000111111100000000011111111111111111111000000000000000000100000000000000001111111111111111111111111100000000111111100000000000",
"00000000111111100000000001111111111111111100000000000000000100110000000000000000011111111111111111111111000000000011111100000000000",
"00000000111111100000000000111111111111110000000000000000000110110000000000000000000111111111111111111100000000000011111110000000000",
"00000000111111100000000000011111111110000000000000000000001110111000000000000000000001111111111111110000000000000001111110000000000",
"00000000111111100000000000001110000000000000000000000000001110111100000000000000000000000111111110000000000000000001111110000000000",
"00000000111111100000000000000000000000000000000000000000011110111100000000000000000000000000000000000000000000000000111110000000000",
"00000000111111110000000000000000000000000000000000000000011110111110000000000000000000000000000000000000000000000000111110000000000",
"00000000011111110000000000000000000000000000000000000000111110111110000000000000000000000000000000000000000000000001111110000000000",
"00000000011111110000000000000000000000000000000000000000111110111110000000000000000000000000000000000000000000000001111110000000000",
"00000000001111111000000000000000000000000000000000000001111110111111000000000000000000000000000000000000000000000011111110000000000",
"00000000001111111000000000000000000000000000000000000001111111111111000000000000000000000000000000000000000000000011111100000000000",
"00000000000111111100000000000000000000000000000000000011111110111111000000000000000000000000000000000000000000000111111100000000000",
"00000000000111111110000000000000000000000000000000000011111110111111000000000000000000000000000000000000000000001111111000000000000",
"00000000000011111111000000000000000000000000000000000011111110111111000000000000000000000000000000000000000000011111111000000000000",
"00000000000001111111110000000000001100000000000000000011111100111111000000000000000000000000000110000000000000111111110000000000000",
"00000000000000111111111111111111111110000000000000000011111100111111000000000000000000000000011111000000000001111111110000000000000",
"00000000000000011111111111111111111110000000000000000011111100111111000000000000000000000001111111100000000011111111100000000000000",
"00000000000000001111111111111111111111000000000000000011111000111111000000000000000000000011111111111100011111111111000000000000000",
"00000000000000000111111111111111111111000000000000000011110000011111000000000000000000000111111111111111111111111110000000000000000",
"00000000000000000001111111111111111111000000000000000001100000001111000000000000000000000111111111111111111111111100000000000000000",
"00000000000000000000111111111111111111000000000000000000000000000110000000000000000000001111111111111111111111111000000000000000000",
"00000000000000000000001111111111111111000000000000000000000000000000000000000000000000011111110111111111111111110000000000000000000",
"00000000000000000000000011111100011111000000000000000000000000000000000000000000000000011111100000111111111111000000000000000000000",
"00000000000000000000000000000000011111000110000000000000000000000000000000000000000000011111100000000001110000000000000000000000000",
"00000000000000000000000000000000011111101111000000000000000000000000000000000000000000111111100000000000000000000000000000000000000",
"00000000000000000000000000000000011111101111011100000000000000000000000000000011000000111111000000000000000000000000000000000000000",
"00000000000000000000000000000000011111101111111100001000000000100000001000000111000000111111000000000000000000000000000000000000000",
"00000000000000000000000000000000011111101111111100011100000001100000011100000111000000111111000000000000000000000000000000000000000",
"00000000000000000000000000000000011111111111111100011110000001100000011110000111000000111111000000000000000000000000000000000000000",
"00000000000000000000000000000000011111111110111100011110000011100000011110000111000000111111000000000000000000000000000000000000000",
"00000000000000000000000000000000011111111110111100011110000011100000011110000111000000111110000000000000000000000000000000000000000",
"00000000000000000000000000000000011111111100111100011110000011100000011110000111000000111110000000000000000000000000000000000000000",
"00000000000000000000000000000000011111111100111100011110000011100000011110000111000011111110000000000000000000000000000000000000000",
"00000000000000000000000000000000011111111100111000011110000011100000011110000111100111111110000000000000000000000000000000000000000",
"00000000000000000000000000000000001111111100111000011110000011100000011110000111100111111110000000000000000000000000000000000000000",
"00000000000000000000000000000000001111111100111000011110000011100000001110000111101111111110000000000000000000000000000000000000000",
"00000000000000000000000000000000000111111110111000011110000011100000001110000111111111111110000000000000000000000000000000000000000",
"00000000000000000000000000000000000111111111111000001110000011100000001110000011111111111100000000000000000000000000000000000000000",
"00000000000000000000000000000000000011111111111000001110000011100000001110000011111111111100000000000000000000000000000000000000000",
"00000000000000000000000000000000000001111111111110011110000011110000001111000111111111111000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000111111111111111110000011110000011111111111111111111000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000011111111111111110000111111111111111111111111111110000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000111111111111111111111111111111111111111111111000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000011111111111111111111111111111111111111111110000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000000011111111111111111111111111111111111111000000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000000000001111111111111111111111111111111000000000000000000000000000000000000000000000000000",
"00000000000000000000000000000000000000000000000000000000111110011111111100000000000000000000000000000000000000000000000000000000000"};

const int bithead_width = 131;
const int bithead_height = 147;

  tft.fillScreen(ILI9341_BLACK);
  paint_mute_indicator();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_CYAN);
  tft.setTextSize(4);
  tft.println(" HackerBoxes");

  for (int y=0; y < bithead_height; y++)
  {
    for (int x=0; x < bithead_width; x++)
    {
      if (bithead[y][x] == '1')
      {
        tft.drawPixel(x+100, y+55, ILI9341_YELLOW);
      }
    }
  }
}
