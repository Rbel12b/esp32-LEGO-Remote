#include <Arduino.h>
#include <LegoHubController.h>
#include <HardwareSerial.h>
#include "mbedtls/base64.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#define RXD2 16
#define TXD2 17


LegoHubController Controller(7);
HardwareSerial hSerial(1);
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
Data_Package data;
Data_Package prevData;
bool Serialdata = false;
int SerialDataCount = 0;
bool SerialStopStart = true;
bool SerialError = false;
byte Serialchecksum = 0xa5;
byte programHubCount = 0;

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
char input_line[MAX_LENGHT];
unsigned int input_pos = 0;
char Base64Data[64];
size_t joystickTimer0[4];
size_t joystickTimer1[4];

int returnval = 0;
void updateConntroller();
int joystickToButton(byte);


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

// here to process incoming serial data after a terminator received
void process_data(const char *ReceivedData)
{
    if (ReceivedData[0] != '*')
    {
        log_e("No * received: %c, %x", ReceivedData[0], ReceivedData[0]);
        return;
    }
    int lenght = 0;
    for (lenght = 1; ReceivedData[lenght] != '*' && ReceivedData[lenght] != 0; lenght++)
        ;
    uint16_t CRC = 0;
    for (int i = 0; i < lenght + 3; i++)
    {
        CRC = crc_xmodem_update(CRC, ReceivedData[i]);
    }
    uint16_t ReceivedCRC = ReceivedData[lenght + 2];
    ReceivedCRC += ReceivedData[lenght + 1] << 8;
    if (CRC != 0)
    {
        log_e("The received data is corrupted, the CRC is not zero: %x", CRC);
        return;
    }
    lenght--;
    strcpy(Base64Data, &ReceivedData[1]);
    Base64Data[lenght] = '\0';
    unsigned char output[64];
    size_t outlen;
    mbedtls_base64_decode(output, 64, &outlen, (const unsigned char *)Base64Data, lenght);
    for (int i = 0; i < sizeof(Data_Package); i++)
    {
        (*(&data.LjoyX + i)) = output[i];
    }
} // end of process_data

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


void updateConntroller()
{
    while (Serial2.available() > 0)
    {
        processIncomingByte(Serial2.read());
    }
}

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Controller.init();
    for (int i = 0; i < programHubCount; i++)
    {
        Controller.initHub(i);
        for (size_t i = millis(); millis() - i < 500;)
        {
            yield();
        }
        while (!Controller.ConnectToAHub(i))
        {
            yield();
        }
        Controller.HUBS[0].Hub->setLedRGBColor(0, 0, 255);
    }
}

void loop()
{

    returnval = Controller.ControlHubs(programHubCount);
    if (returnval == 1)
    {
        log_e("Invalid Hub number.");
    }
    int Number = 0;
    for (int i = 0; i < 4; i++)
    {
        Number = map((*(&data.LjoyX + i)), 0, 255, -100, 100);
        if (Number > -3 && Number < 3)
        {
            Number = 0;
        }
        Controller.vars[i] = Number;
    }
    for (int i = 4; i < sizeof(Data_Package); i++)
    {
        Number = (*(&data.LjoyX + i));
        Controller.vars[i] = Number;
        if (buttonpressed(i - 4))
        {
            Controller.vars[i + 16] = Controller.vars[i + 16] ? 0 : 1;
        }
    }
    updateConntroller();
}