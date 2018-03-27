# SafeEve Device
Arduino sketch for SafeEve device that runs on a Nano coupled with a SIM800L module

SafEve is a safety device conceptualised during the National Innovation Drive Hackathon 2017, at PDPM Indian Institute of Information Technology, Design & Manufacturing, Jabalpur.

The device is aims to enhance safety of users with limited access or knowledge of technology viz. rural population, children. The device features a trigger button that sends a geo-tagged distress signal wirelessly over 2G band to nearby help viz. a police station. It works as an independent device and NOT as a peripheral to a Smartphone or Computer. The SafEve Monitor App (https://github.com/ajwad-shaikh/SafEve_Monitor) is designed for the designated help to manage the distress signals and respond effectively and immediately.

P.S. - The sketch sends blank messages with the original Adafruit Fona Library. I have played around with the header file to fix the issue and have released a mod of the library. Please find the modified library here -> https://github.com/ajwad-shaikh/Adafruit_FONA_mod

Project by - Amanat Mishra & Ajwad Shaikh PDPM Indian Institute of Information Technology, Design and Manufacturing, Jabalpur.

Code Written by Ajwad Shaikh, SchrödingerTech.
