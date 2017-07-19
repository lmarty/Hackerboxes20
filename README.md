# Hackerboxes20
Projects for the Hackerboxes #20 badge hardware

## What is this?
This is a collection of projects for the [Hackerboxes](http://hackerboxes.com) Kit \#20 badge hardware.  The kit consists of an esp32 based "Dev Kit C" board, a nifty QVGA Color TFT LCD Display, a buzzer, 5 LEDs, 5 touch inputs and an SD_card slot.  For more information on the hardware platform, please see their [Instructables](https://www.instructables.com/id/HackerBoxes-0020-Summer-Camp/)  If you built the project to their specs, this should work like a champ for you. The code is backwards compatible with the premise of the original demo, with the intention of extending it much further.

To get your own kit, and other awesome projects in the mail each month, be sure to get signed up for a Hackerbox subscription at [www.hackerboxes.com](http://www.hackerboxes.com)

THE SD CARD WORKS IN THIS CODE!!!!


## Instructions

* Install the Arduino IDE.
* Install the [espressif libraries](https://github.com/espressif/arduino-esp32).
* Install [nhatuan84's SD libraries](https://github.com/nhatuan84/esp32-micro-sdcard).
* Clone this repo down to your code folder and link the project dirs to ~/Documents/Arduino/<PROJECTNAME>.

IF THE SD ISN'T WORKING...
* try holding down the flash button on the dev kit board as you boot
* If that fixes it... Pop your TFT board off the project PCB and flip it over.  There are two small pads labled "j1" on the back, solder them together. 

## Projects

### Defcon 25 Game : INFECTED
Tag based game.  Premise is to spread the disease while at DC.  Infected. Hackers. Viruses. Defcon. Get it?  One badge will start out flashed as infected.  Others, willing hosts.  The infected badge will tag any non-infected badge it encounters, making that person effectively "IT".  Once you've passed on the disease, you can become infected again.  Just not from the same person you gave it too.  As the infection spreads, it passes wit it a list of badges/names it's spread from with it.  Giving the sense of the virus mutating, and allowing us to enforce the "cant get it from who you got it from" state. Get out there, get the infection, spread it to friends, get it again.  The person who catches the most mutations of the infection wins.


Scoring :
When the badge goes online, it connects and establishes a MQTT connnection to somewhere.  Should detect any open wifi and use that whenever possible for live scoring. First time connect, registers the badges info.  serial, mac addys, handle of the owner, infected state, permutations of the "virus". Any other info that sounds fun. Any connections after serve to update the score and upload permutations of the "infection".

Info should be posted on a dynamic webpage.  Keeping track of players and count of infections caught.  Leaderboard style.  Most infections on top, all the way down.  Stats should be clickable for list of permutations caught.

Infection file format:

JSON file containing the following info.  Grows as it passes.
'''
{
  Infected:
    player: name+wifiMAC
      infected_state: Infected/Clean/prev_host
      Prev_infections: count
      transmitted_from: player name
      infection time: datestamp
      signature: transmitted_from:value::player:value
      badge_boottime: datestamp
    player: name+wifiMAC
      infected_state: Infected/Clean/prev_host
      Prev_infections: count
      transmitted_from: player name
      infection time: datestamp
      signature: transmitted_from:value::player:value
    ...
  }
'''



Firmware and game status display :
Badge should clearly show that it has the latest firmware.  Do this via an image and v.ID in the main loop.
Badge should show infection state.  Use custom image in loop.  Red for infected, green for clean, blue for past hosts, etc.


Wormlike update function:
badge should update any other badges it encounters that are below its current version. Look for hackerboxer SSID, check ver.  if [remote_ver < our_ver ] then; initiate update.

Current FW badge should indicate it is updating another badge.
Old FW badge being updated should show its status as being updated, so owner doesnt leave range.
make both badges beep and flash like mad till done.

--TODO :
1. Wormlike : Auto spreading firmware : Use wifi scanner to search out other badges and the OTA libs to update them to this FW.  You know, so they can play too
1. Finish seen SSIDs logging to SD.  Works, but is ugly. add timestamp, mac, packet count, connection info, other data. Format as json.
1. Implement "infected" feature : One person to start out as infected, pass the token to next seen.  Make display and LED's show infection status.
1. Implement seen lightbar counter.  scoring seq should start blank(off).  Light up green LEDs across the bar, left to right. For first 5 seen. Yellow for next, red for next, blue for next, rainbow for next.  As to visually display how many other badges you've come across at a glance. Overlap the colors as you count up. make state persist across badge reboots. incorporate into main loop.
1. Implement "Tagged" and "Hacker Found" freakout.  Make badge shit a chicken when it sees other hacker badges.  Throw a special fit when you get infected.
1. Implement GPG key exchange on contact.  : If gpg key exists on root of SD card, copy over to other badges found. Share secure contact info with other hax0rz.
1. Implement reset brains keypress seq : reset the badge to forget about game status and seen badges on secret key hold pattern.
1. Implement USB host mode : hold down keypad button at boot to go into USB host mode.  Allowing transfer of files to SD.
1. Implement custom display pic and nametag code : show handle and avatar as part of badge loop
1. Implement MQTT scoring : If badge sees open wifi, establish MQTT connection to some online endpoint, and upload name and stats/score.  So we can keep track of the "winner"
1. Implement AP gateway mode for badges.  Allowing a badge that is connected to the internet, via hotspot or otherwise, to allow other badges that connect to it's AP to get online for the purpose of making an MQTT connection to the scoring system


### Ultimate badge
--TODO :
1. Graphic boot menu for all the below 
1. WIFI sniffer logging to SD
1. Bluetooth sniffer logging to SD
1. Targeted WIFI deauth via menu of seen devices
1. Asshole mode : mass deauth everything. wifi and bluetooth.
1. Targeted packet dump
1. Raw packet dump


### 3d Printed Case
--TODO : Design a case for this thing

### Physical Button Upgrade
for nes and other emus
Add another button and update them to physicals vs the onboard touch

### Contributing
Fork me, branch me, PR me. We've got 2 weeks till showtime!  YOUR IDEAS ARE VALUABLE!!!! Even if you cant code, contribute your thoughts through issues and pull requests.  Let's make this an entertaining game for everyone.  #Badgelife on twitter has some inspiration too.  No contributions will go unheard.  This is open source and driven by the community. Band together, build fast!  Grab something off the board at https://github.com/lmarty/Hackerboxes20/projects/1


## Thank You!!
SD Card support is provided by libraries coded by [nhatuan84](https://github.com/nhatuan84) Thanks dude! You saved me tons of frustration.
