

#include <mySD.h>
#include "WiFi.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_NeoPixel.h"

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

// init the display and leds
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

// setup our game variables
char hackers_found[13][70];
int next_hacker_found = 0;
int muted = false;
int mute_touched = false;
char ssid[]="HackerBoxer_MrmuthafuckinE";  //put your handle after the underscore

// setup our sd card object
File root;

// run our main setup function
void setup()
{
  Serial.begin(115200);
  Serial.println("Firing up our setup function");
  Serial.print("Initializing SD card...");

  /* initialize SD library with SPI pins */
  if (!SD.begin(17, 16, 4, 0)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

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
    // cycle some NeoPixel Rainbows
  RainbowLEDcycle(18);

  // scan other SSIDs
  wifiScan2LCD();

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
}

//
// OUR FUNCTIONS
//

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
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(4);
  tft.println(" HAX0R B0X3S");
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);

  while (displaylines > 0)
  {
    // Load our object
    netsfound = WiFi.scanNetworks();

    //check our status
    if (netsfound==0)
    {
      tft.println(". . .");
      displaylines--;
    }
    for (int i = 0; i < netsfound; ++i)
    {
      if (WiFi.SSID(i).startsWith("HackerBoxer"))
      {
        //convert
        WiFi.SSID(i).toCharArray(hackers_found[next_hacker_found],70);
        hackers_found[next_hacker_found][25] = 0;  //truncate string
        next_hacker_found++;
        //set display limit
        if (next_hacker_found == 13)
          next_hacker_found = 0;
        //
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
  mute_touched = true;
}
