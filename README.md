# Hackerboxes20
Projects for the Hackerboxes #20 badge hardware

## What is this?
This is a collection of projects for the [Hackerboxes](http://hackerboxes.com) Kit \#20 badge hardware.  The kit consists of an esp32 based "Dev Kit C" board, a nifty QVGA Color TFT LCD Display, a buzzer, 5 LEDs, 5 touch inputs and an SD_card slot.  For more information on the hardware platform, please see their [Instructables](https://www.instructables.com/id/HackerBoxes-0020-Summer-Camp/)  If you built the project to their specs, this should work like a champ for you.

The projects rely on the espressif esp32 Arduino libraries located [HERE.](https://github.com/espressif/arduino-esp32)

## Instructions

Install the Arduino IDE. Install the [espressif libraries](https://github.com/espressif/arduino-esp32).  Install [nhatuan84's SD libraries](https://github.com/nhatuan84/esp32-micro-sdcard). Clone this repo down to your code folder and link the project dirs to ~/Documents/Arduino/<PROJECTNAME>.  

## Projects

### Ultimate badge
--TODO :
### Defcon 25 Game
--TODO :
#) Auto spreading firmware : Use the OTA libs to search out other badges and update them to this FW.  You know, so they can play too
#) Finish seen SSIDs logging
#) Implement "IT" feature : One person to start out as IT, pass a token to next seen.  Make display and LED's show IT status.
#) Implement seen lightbar counter.  Light up green LEDs for first 5 seen, yellow for next, red for next, blue for next, rainbow for next.  As to visually display how many other badges you've come across at a glance.  make state persist across badge reboots
#) Implement "Tagged" and "Hacker Found" freakout.  Make badge shit a chicken when it sees other hacker badges.  Throw a special fit when you get tagged as "IT"
#) Implement GPG key exchange on contact.  : copy over yourEMAILaddy.gpg to badges found.  
#) Implement reset brains keypress seq : reset the badge to forget about game status and seen badges.  
#) Implement USB hist mode : hold down keypad at boot to go into USB host mode.  Allowing transfer of files to SD.  
#) Implement custom display pic and nametag code : show handle and avatar as part of badge loop
# Implement MQTT scoring : If badge sees open wifi, establish MQTT connection to some online endpoint, and upload name and stats/score.  So we can keep track of the "winner"

### 3d Printed Case
--TODO :


## Thank You!!
SD Card support is provided by libraries coded by [nhatuan84](https://github.com/nhatuan84) Thanks dude! You saved me tons of frustration.  
