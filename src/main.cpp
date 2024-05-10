#include <Arduino.h>
#include <LegoHubController.h>
#include <HardwareSerial.h>
#include "mbedtls/base64.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "Secrets.h"
#include "FS.h"
#include "SPIFFS.h"
AsyncWebServer server(80);
#define RXD2 16 // hardware serial receive pin
#define TXD2 17 // hardware serial trnsmit pin

LegoHubController Controller(7); // An instance of the class LegoHubController with the maximum of 7 hubs
HardwareSerial hSerial(1);       // A hardwareSerial object
struct Data_Package
{
    byte LjoyX;
    byte LjoyY;
    byte RjoyX;
    byte RjoyY;
    bool Button1;
    bool Button2;
    bool Button3;
    bool Button4;
    bool Button5;
    bool Button6;
    bool Button7;
    bool Button8;
    bool Button9;
    bool Button10;
    bool Button11;
    bool Button12;
    bool Button13;
    bool Button14;
    bool Button15;
    bool Button16;
};

// definitions for the joysticks and buttons

#define RC_LX 0
#define RC_LY 1
#define RC_RX 2
#define RC_RY 3
#define RC_L1 0
#define RC_L2 1
#define RC_R1 2
#define RC_R2 3
#define RC_LEFT_PAD_DOWN 4
#define RC_LEFT_PAD_UP 5
#define RC_LEFT_PAD_LEFT 6
#define RC_LEFT_PAD_RIGHT 7
#define RC_RIGHT_PAD_LEFT 8
#define RC_RIGHT_PAD_UP 9
#define RC_RIGHT_PAD_RIGHT 10
#define RC_RIGHT_PAD_DOWN 11
#define RC_LEFT_MIDDLE 12
#define RC_RIGHT_MIDDLE 13

#define RC_JOY_POSITIVE_TRESHOLD 188
#define RC_JOY_ZERO 128
#define RC_JOY_NEGATIVE_TRESHOLD 68

// how much serial data we expect before a newline
#define MAX_LENGHT 50
char input_line[MAX_LENGHT + 1]; // received bytes
unsigned int input_pos = 0;      // how many bytes did we receive
char Base64Data[MAX_LENGHT + 1]; // stores the base64 encoded data
Data_Package data;               // struct for storing the data received from the serial port
Data_Package prevData;           // struct for storing the previous data received from the serial port
size_t Code_Len;                 // Leght of the program (Connroller.HUBCode)

byte programHubCount = 0; // how many hubs are used in a program, max 7

void updateConntroller();
uint16_t crc_xmodem_update(uint16_t crc, uint8_t data);
void process_data(const char *ReceivedData);
void processIncomingByte(const byte inByte);
bool buttonpressed(int button);

void setup()
{
    // initialize the serial connections to 115200 baud
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    SPIFFS.begin();
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    // Print the ESP32's IP address
    Serial.print("ESP32 Web Server's IP address: ");
    Serial.println(WiFi.localIP());

    server.on(
        "/Code",
        HTTP_POST,
        [](AsyncWebServerRequest *request)
        {
            request->send(201, "text/plain", "OK");
        },
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            Code_Len = 0;
            for (Code_Len = 0; Code_Len < len; Code_Len++)
            {
                if (Code_Len == MAX_PROG_MEM)
                {
                    break;
                }
                Controller.HUBCode[Code_Len] = data[Code_Len];
            }
            if (Code_Len > MAX_PROG_MEM)
            {
                Code_Len = MAX_PROG_MEM;
            }
            Controller.HUBCode[MAX_PROG_MEM] = 0;
            request->send(200, "text/plain", "OK");
        });
    server.on(
        "/Code",
        HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send(201, "text/plain", (char*)Controller.HUBCode);
        });
    server.on(
        "/",
        HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send(SPIFFS, "/index.html");
        });
    server.serveStatic("/", SPIFFS, "/", "max-age=1800");
    // Start the server
    server.begin();
    // initialize the controller
    Controller.init();
    if (programHubCount > 7)
    {
        // if the number iof hubs is too large stop.
        log_e("Too many hubs: %i", programHubCount);
        while (1)
            ;
    }
    // connect to {programHubCount} hubs
    for (int i = 0; i < programHubCount; i++)
    {
        // initialize the HUB
        Controller.initHub(i);
        // wait 500 ms
        for (size_t i = millis(); millis() - i < 500;)
        {
            yield();
        }
        // wait until the HUB connects
        while (!Controller.ConnectToAHub(i))
        {
            yield();
        }
        // set the HUB's led color to blue
        Controller.HUBS[i].Hub->setLedRGBColor(0, 0, 255);
    }
}

void loop()
{
    // conntrol the HUBs and check the return variable
    switch (Controller.ControlHubs(programHubCount))
    {
    case 1:
        log_e("invalid HUB number");
        break;
    case 0:
        // everything is fine
        break;
    default:
        // someting went wrong..
        log_e("Error");
    }
    int Number = 0; // local variable for storing data
    // loop trought the analog variables(joysticks)
    for (int i = 0; i < 4; i++)
    {
        // read the joystick value and map it to -100 ... 100
        Number = map((*(&data.LjoyX + i)), 0, 255, -100, 100);
        // make it to zero when it's wery close
        if (Number > -3 && Number < 3)
        {
            Number = 0;
        }
        // store it in the controller
        Controller.RemoteData[i] = Number;
    }
    // loop trought the digital variables(buttons)
    for (int i = 4; i < sizeof(Data_Package); i++)
    {
        // store the data in the controller
        Controller.RemoteData[i] = (*(&data.LjoyX + i));
    }
    // check if we ha dreceived serial data
    updateConntroller();
}
/**
 * calculate CRC
 * @param crc the initial CRC
 * @param data the input byte
 * @returns the new CRC
 */
uint16_t crc_xmodem_update(uint16_t crc, uint8_t data)
{
    int i;
    crc = crc ^ ((uint16_t)data << 8);
    for (i = 0; i < 8; i++)
    {
        if (crc & 0x8000)
            crc = (crc << 1) ^ 0x1021;
        else
            crc <<= 1;
    }
    return crc;
}

/**
 * Process incoming serial data after a terminator received
 * @param ReceivedData the data to process
 * @returns void
 */
void process_data(const char *ReceivedData)
{
    if (ReceivedData[0] != '*') // the first charachter should be always '*'
    {
        log_e("No * received: %c, %x", ReceivedData[0], ReceivedData[0]);
        return;
    }
    int lenght = 0;
    for (lenght = 1; ReceivedData[lenght] != '*' && ReceivedData[lenght] != 0; lenght++)
        ; // count the carachters in the string
    // including the start and end '*'
    uint16_t CRC = 0;
    // loop trought the data
    for (int i = 0; i < lenght + 3; i++)
    {
        // and calculate its CRC
        CRC = crc_xmodem_update(CRC, ReceivedData[i]);
    }
    // the last two bytes of the data contains the CRC, so if everything went good the CRC is zero
    if (CRC != 0)
    {
        // else print the error message, and discard the data
        log_e("The received data is corrupted, the CRC is not zero: %x", CRC);
        return;
    }
    // decrease lenght by 1 to exclude the '*' charachter in the end
    lenght--;
    // copy the data to a new string excludin the first '*' charachter
    strncpy(Base64Data, &ReceivedData[1], MAX_LENGHT);
    // add the ending zero to the string
    Base64Data[lenght] = '\0';
    size_t outlen; // for storing the lenght of the data
    mbedtls_base64_decode((unsigned char *)(*(&data.LjoyX)), sizeof(Data_Package), &outlen, (const unsigned char *)Base64Data, lenght);
} // end of process_data

/**
 * Process incoming serial data
 * @param inByte the byte to process
 * @returns void
 */
void processIncomingByte(const byte inByte)
{

    switch (inByte)
    {

    case '\n':                     // end of text
        input_line[input_pos] = 0; // terminating null byte

        // terminator reached! process input_line here ...
        process_data(input_line);

        // reset buffer for next time
        input_pos = 0;
        break;

    case '\r': // discard carriage return
        break;

    default:
        // keep adding if not full ... allow for terminating null byte
        if (input_pos < (MAX_LENGHT - 1))
            input_line[input_pos++] = inByte;
        break;

    } // end of switch

} // end of processIncomingByte
/**
 * check if we had received data over the serial connection and process the received data
 * @returns void
 */
void updateConntroller()
{
    while (Serial2.available() > 0)
    {
        // if data is available process it
        processIncomingByte(Serial2.read());
    }
}
/**
 * @returns void
 */
bool buttonpressed(int button)
{
    if ((*((&data.Button1) + button)) && !(*((&prevData.Button1) + button)))
    {
        // if the button is pressed and we didn't checked it before, then set the status to checked
        (*((&prevData.Button1) + button)) = true;
        // and return true
        return true;
    }
    if (!*((&data.Button1) + button))
    {
        // if the button isn't pressed: set the status to checked
        (*((&prevData.Button1) + button)) = false;
    }
    // else return false
    return false;
}
