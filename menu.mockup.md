# MOCKUP #

### Program Execution Flow and Main Menu
 Program flow pseudocode and mock main menusystem layout for our badge. Project goal is to create a collection of useful functions and apps that retains your interest beyond the first weekend of owning it.
 From badge to desktop conversation piece, retro game system, lightweight messaging platform, pentest implant and mini-toolbox.
 design principal centers around balance of function, fun and bling.
 Not too much of any of them.  Something oyu can leave at your desk and get a few more kicks out of after the initial glam wears off.
 * Messaging should use local net first, then go to web
 * Messaging service should be ongoing(you made a friend, stay in contact!)
 * Software should be easy to modify and extend
 * all sub menu items and extensibility should follow the same pattern.
 * Functions content belongs in the libs, not the INO.
 * Goal of menu is to be fully functional without being too cluttered
 * Goal of main display loop is to be intuitive and entertaining, without over doing it.
 * Organize with a priority on usefulness
 * Most to least used ordering?
 * Capturing ideas as they come.
 * Leverage a community menu lib for implementation


  Written with the Arduino IDE on a ESP32 based system.
  Components included a cheapo TFT,a piezo buzza, SD slot, 6 capacitive inputs and 5 RGB LEDs.


#### Boot!

#### Init() {
```
Init Storage
Init networking
Init Webserver
Init MQTT
Check botnet status and Role
Run any queued botnet cmds
Check for updates
```
#### }


#### Main_loop() {
```
Show startup logo
Show Avatar
LED Init / Cycle Test
Show NICK
Tag status - Infected/Clean/Previous Patient - If infected = 1, Inverse colors
interrupt == Break to main menu
```
#### }

#### Main Menu
* Displayed on any interrupt during the *main()* function
* Navigation is performed using the directional buttons


Buttons | Functions
--------|----------
*Up/Down* | navigate vertically through the list.
*Right* | arrow selects and is represented as *"\"*
*Left* | Arrow button is back and represented as *"/"*



*_Main Menu_*   |    .
------------ | ------
*- _Tools_* | .
\ |  Active APs
\ |  Active Clients
\ |  Signal Meter
/ |
*- _Messaging_* | .
\ | Contact List
\ | Badge to badge
\ | All badge broadcast
\ | Chat room
/ |
*- _Games_* | .
\ | NES Emu
\ | PacMan
\ | Galaga
\ | Infection Stats (seen list, tags, times)
/ |
*- _Botnet_* | .
\ | Bots Connected
\ | Botnet history
\ | Run on All
\ | Run on Targeted (bot name)
/ |
*- _Hacks_*| .
\ | Wifi Deauther w/ web remote
\ | Wifi Pineapple Lite - Steal this directly from hacker warehouse when they release it
\ | PCAP(tcpdump and wireshark compatible pcapture funcitonality)
\ | subnet recon(nmap... duh)
\ | Pivot stub (drop me in the bushes next to your target)
\ | Stealth Mode (no leds, no sound, dim display with short timeout)
\ | Printer Blasta! - Send giant print jobs to all listening print devices without auth
\ | Common IOT Vuln Autohacka - throw in some scripts to exploit a few IOT devices
/ |
*- _Settings_* | .
\ | WiFi/Bt/Ble On/Off w/ status
\ | Setup/Change Handle
\ | Sleep Timer
\ | USB Host Mode
\ | Webserver On/Off
\ | TCP Forward(gateway Mode) On/Off
\ | OTA Update
\ | Tag Mode (enter pass to become "IT")
\ | Splash selector
/ |
*- _Debug_*| .
\ | LED Test
\ | Screen Test
\ | Piezo Test
\ | Debug Output(console)
\ | Battery Status
\ | MQTT Status (peers/topics/last connect/who's IT?)
\ |Received File List
\ | Flash and Storage status
\ | Fuckit Button (restore to stock settings and FW)
/ | (fin)
 | .


#### Notes
* Keep counter on button presses
* lock level 1-10
* penis
