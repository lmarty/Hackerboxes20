# !!!! MOCKUP / DRAFT / WIP !!!!
This doc is my living collection of inspirational features for the badge.  Updates will happen as this is in no way final.  Features/functionality may or may not get built.  

If you have an idea and want something to be included, please feel free to submit a PR. Plug in your work and push it on up!  


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


#### Core features
  * Game platform
    * NesEmu with easy to easy to add/change ROM mechanism
    * One button game support onboard
    * BT controller support

#### Boot!

#### Init() {
```
Init Storage
Init networking
Init Webserver
Init MQTT
Connect to botnet
  * Check botnet status and Role
  * Run any queued botnet cmds
Check for any available OTA updates
```
#### }


#### Main_loop() {
```
Display boot logo
LED Init / Cycle Test
Display Avatar w/ Nick overlay
Display Tag status splash - Infected/Clean/Previous Patient - If infected = 1, Inverse colors
interrupt == Break to main menu
```
#### }

#### Main Menu
* Displayed on any interrupt during the *main()* function
* Navigation is performed using the directional buttons

|   *_Main Menu_*   |           |
---------------- | ------------ |
*- _Tools_* | |
 |  - Active APs |
 |  - Active Clients |
 |  - Signal Meter |
*- _Message_* | |
 | - Contact List
 | - Badge to badge
 | - All badge broadcast
 | - Chat room
 |
*- _Games_* | |
 | - NES Emu |
 | - PacMan |
 | - Galaga |
 | - Infection Stats (seen list, tags, times) |
*- _Badge Botnet_* | |
 | - Bots Connected |
 | - Botnet Command History |
 | - Run on Targeted (bot name) |
 | - Run on All |
*- _Mayhem_* | |
 | - Wifi Deauther w/ web remote |
 | - Wifi Pineapple Lite - Steal this directly from hacker warehouse when they release it |
 | - PCAP(tcpdump and wireshark compatible pcapture funcitonality) |
 | - subnet recon(nmap... duh) |
 | - Pivot stub (drop me in the bushes next to your target) |
 | - Stealth Mode (no leds, no sound, dim display with short timeout) |
 | - Printer Blasta! - Send giant print jobs to all listening print devices without auth |
 | - Common IOT Vuln Autohacka - throw in some scripts to exploit a few IOT devices |
*- _Settings_* | |
| - Setup/Change Handle |
| - Sleep Timer |
| - USB Host Mode |
| - Webgui On/Off |
| - File collector On/Off |
| - Remote shell/telnet On/Off |
| - TCP Forward(gateway Mode) On/Off |
| - OTA Update |
| - Tag Mode (enter pass to become "IT") |
| - Splash selector |
| - Avatar selector |
   | *- Network -* |
  | | - Airplane Mode On/Off
  | | - WiFi
  | | - Connection Status
  | | - On/Off
  | | - Join NetworkS
  | | - Forget all networks
  | *- Bluetooth -* |
  | | - On/Off
  | | - Pair
  | | - Paired Device List
  | | - Clear Paired Device List
*- _Debug_ -* |
 | - LED Test |
 | - Screen Test |
 | - Piezo Test |
 | - Debug Output(console) |
 | - Battery Status |
 | - MQTT Status (peers/topics/last connect/who's IT?) |
 | - Received File List |
 | - Flash and Storage status |
 | - Fuckit Button (restore to stock settings and FW) |
 | (fin) |

#### TODO :
- [ ] Frame up the menu structure/content
- [ ] Code up menu system
- [ ] Add supporting code for menu items    
- [ ] Validate basic functionality (break loop on interrupt, navigation works, test app launches from it, etc)
- [ ] Create  OTA mechanism and checker
- [ ]
- [ ] Stand up backend infra for MQTT
- [ ] Create stories for all proposed functionality
- [ ]
- [ ]
- [ ]
- [ ]
- [ ]

#### Notes
* Keep counter on button presses
* lock level 1-10
* penis
* Leverage a community menu lib for implementation
