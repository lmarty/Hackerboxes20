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
#) WIFI sniffer logging to SD
#) Bluetooth sniffer logging to SD
#) Targeted WIFI deauth via menu of seen devices
#) Asshole mode : mass deauth everthing. wifi and bluetooth.

### Defcon 25 Game : INFECTED
Tag based game.  Premise is to spread the disease while at DC.  Get it?  One badge will start out flashed as infected.  Others, willing hosts.  The infected badge will tag any non-infected badge it encounters, making that person effectively "IT".  Once you've passed on the disease, you can become infected again.  Just not from the same person you gave it too.  As the infection spreads, it passes wit it a list of badges/names it's spread from with it.  Giving the sense of the virus mutating, and allowing us to enforce the "cant get it from who you got it from" state. Get out there, get the infection, spread it to friends, get it again.  The person who catches the most mutations of the infection wins.  


Scoring :
When the badge goes online, it connects and establishes a MQTT connnection to somewhere.  Should detect any open wifi and use that whenever possible for live scoring. First time connect, registers the badges info.  serial, mac addys, handle of the owner, infected state, permutations of the "virus". Any other info that sounds fun. If badge has infection info, that is uploaded as well.   

Info should be posted on a dynamic webpage.  Keeping track of players and count of infections caught.  Leaderboard style.  Most infections on top, all the way down.  Stats should be clickable for list of mutations caught.  

Infection file format:

JSON file containing the following info.  Grows as it passes.

{
  Infected:
    player: name
      infected_state: Infected/Clean/prev_host
      Prev_infections: count
      transmitted_from: player name
      infection time: datestamp
      signature: transmitted_from:value::player:value
      badge_boottime: datestamp
    player: name
      infected_state: Infected/Clean/prev_host
      Prev_infections: count
      transmitted_from: player name
      infection time: datestamp
      signature: transmitted_from:value::player:value
    ...
  }



}
Firmware and game status:
Badge should clearly show that it has the latest firmware.  Do this via an image a v. ID in the main loop.  
Badge should show infection state.  Use custom image in loop.  Red for infected, green for clean, blue for past hosts.


Wormlike update function:
badge should update any other badges it encounters that are below its current version
Current FW badge should indicate it is updating another badge.  
Old FW badge being updated should show its status as being updated, so owner doesnt leave range.

--TODO :
#) Wormlike : Auto spreading firmware : Use wifi scanner to search out other badges and the OTA libs to update them to this FW.  You know, so they can play too
#) Finish seen SSIDs logging to SD.  Works, but is ugly. add timestamp, mac, packet count, connection info, other data.
#) Implement "IT" feature : One person to start out as IT, pass a token to next seen.  Make display and LED's show IT status.
#) Implement seen lightbar counter.  Light up green LEDs for first 5 seen, yellow for next, red for next, blue for next, rainbow for next.  As to visually display how many other badges you've come across at a glance.  make state persist across badge reboots
#) Implement "Tagged" and "Hacker Found" freakout.  Make badge shit a chicken when it sees other hacker badges.  Throw a special fit when you get tagged as "IT"
#) Implement GPG key exchange on contact.  : If gpg key exists on root of SD card, copy over to other badges found. Share secure contact info with other hax0rz.  
#) Implement reset brains keypress seq : reset the badge to forget about game status and seen badges.  
#) Implement USB hist mode : hold down keypad at boot to go into USB host mode.  Allowing transfer of files to SD.  
#) Implement custom display pic and nametag code : show handle and avatar as part of badge loop
# Implement MQTT scoring : If badge sees open wifi, establish MQTT connection to some online endpoint, and upload name and stats/score.  So we can keep track of the "winner"

### 3d Printed Case
--TODO :


## Thank You!!
SD Card support is provided by libraries coded by [nhatuan84](https://github.com/nhatuan84) Thanks dude! You saved me tons of frustration.  
