## Control your garage without an Arduino!

### Experimenting with *ESP32* + *garage doors* + the *web*!

---

### DISCLAIMER
***You will need to have prior knowledge of where to find the dependencies that are being used (in the .ino file), as well as how to upload data to SPIFFS!! This README does not cover that. (If I found out how to do it via Google, so can you :))***

To be clear - I am not responsible if you blow your house up or catch it on fire or electrocute yourself, or for ANYTHING else! 

**USE THIS CODE/GUIDE AT YOUR OWN RISK!**

---

## Details

Our front end uses:
 - Bootstrap + jQuery

Our back end uses:
 - `AsyncWebServer` framework
 - Database is a 'local' flat file (on the ESP32)
   - .txt file in SPIFFS flash (non-volatile)

---

## DIY?

If you want to use this code you will need to make changes to 2 files (*you can find detailed info within the comments of these files*):

 - `/garage_door_opener.ino`
 - `/data/index.js`