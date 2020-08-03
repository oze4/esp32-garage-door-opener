/**
 * This code is a melting pot.  Pieces of this code were discovered across many resources.
 * Resources that I did not make note of, but are easy to find when Googling for this
 * sort of thing.
 * 
 * Matt Oestreich
 */

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <FS.h>
#include "SPIFFS.h"

/**
 * Set WiFi Credentials
 */

const char *ssid = "<wifi_name>";
const char *password = "<wifi_secret>";

/**
 * This needs to match the following line in `/data/index.js`:
 * `const POST_SECRET = "<some_madeup_secret_for_pseudo_security>";`
 */

const char POST_SECRET[] = "<some_madeup_secret_for_pseudo_security>";

/**
 * Set database SPIFFS path
 */

const char DATABASE_PATH[] = "/database.txt";

/**
 * Create AsyncWebServer object on port 80
 */

AsyncWebServer server(80);

/**
 * --------------------------------------------------------------------------------
 * SETUP HERE
 * --------------------------------------------------------------------------------
 */

void setup()
{
  /**
   * Serial port for debugging purposes
   */

  Serial.begin(115200);

  /**
   * Initialize SPIFFS and database then print SPIFFS contents
   */

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  initializeDatabase();
  printSpiffsContents();

  /**
   * Connect to Wi-Fi
   */

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  /**
   * Print ESP32 Local IP Address
   */

  Serial.println(WiFi.localIP());

  /**
   * ROUTES
   */

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html");
  });

  // LOAD CSS/JS ASSETS
  server.on("/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
  });

  server.on("/bootstrap.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/bootstrap.min.js", "text/javascript");
  });

  server.on("/jquery-3.3.1.slim.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/jquery-3.3.1.slim.min.js", "text/javascript");
  });

  server.on("/popper.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/popper.min.js", "text/javascript");
  });

  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.js", "text/javascript");
  });

  // SANITY CHECK
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
    File db = SPIFFS.open(DATABASE_PATH);
    if (!db)
    {
      char resp[] = "Database not found, creating default database now.";
      Serial.println(resp);
      request->send(500, "text/plain", resp);
      db.close();
      return;
    }
    String rez = db.readString();
    Serial.println(rez);
    request->send(200, "text/plain", rez);
    db.close();
  });

  // OPEN GARAGE DOOR
  server.on(
      "/open_garage",
      HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        String secret;
        for (size_t i = 0; i < len; i++)
        {
          secret += char(data[i]);
        }
        if (secret == POST_SECRET)
        {
          File db = SPIFFS.open(DATABASE_PATH, FILE_WRITE);
          if (db.print("true"))
          {
            //
            // CHANGE PIN VOLTAGE HERE
            //
            request->send(200, "text/plain", "true");
            db.close();
            return;
          }
          else
          {
            request->send(200, "text/plain", "false");
            db.close();
            return;
          }
        }
        request->send(404, "text/plain", "");
      });

  // CLOSE GARAGE DOOR
  server.on(
      "/close_garage",
      HTTP_POST,
      [](AsyncWebServerRequest *request) {},
      NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        String secret;
        for (size_t i = 0; i < len; i++)
        {
          secret += char(data[i]);
        }
        if (secret == POST_SECRET)
        {
          File db = SPIFFS.open(DATABASE_PATH, FILE_WRITE);
          if (db.print("false"))
          {
            //
            // CHANGE PIN VOLTAGE HERE
            //
            request->send(200, "text/plain", "true");
            db.close();
            return;
          }
          else
          {
            request->send(200, "text/plain", "false");
            db.close();
            return;
          }
        }
        request->send(404, "text/plain", "");
      });

  /**
   * Start server
   */

  server.begin();
}

/**
 * --------------------------------------------------------------------------------
 * LOOP HERE
 * --------------------------------------------------------------------------------
 */

void loop() {}


/**
 * HELPER FUNCTIONS
 */

void initializeDatabase()
{
  if (!SPIFFS.exists(DATABASE_PATH))
  {
    Serial.println("Database not found, creating default database now.");

    File db = SPIFFS.open(DATABASE_PATH, FILE_WRITE);
    char defaultConfig[] = "false";

    if (db.print(defaultConfig))
    {
      Serial.println(" - Successfully created default database");
    }
    else
    {
      Serial.println(" - !!! ERROR !!! Unable to create database!");
    }
  }
  else
  {
    Serial.println("Database exists!");
  }
}

void printSpiffsContents()
{
  File root = SPIFFS.open("/");
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
