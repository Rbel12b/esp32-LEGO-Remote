#ifndef LEGOHUBCONTROLL_H_
#define LEGOHUBCONTROLL_H_

#include <Arduino.h>
#include "Lpf2Hub.h"
#include "PowerFunctions.h"
 
#define DEBUGLOG
// define logging
#ifdef DEBUGLOG
#define log_debug(format, ...) log_printf(ARDUHAL_LOG_FORMAT(D, format), ##__VA_ARGS__)
#else
#define log_debug(format, ...)
#endif

#ifndef ARDUINO
#include <inttypes.h>
typedef uint8_t byte;
#endif

#define MAX_PROG_MEM 1024
#define IR_PIN 4 // the pin where the IR led is connected to

struct TechnicHUB // Object for maintaining a Lego Technic HUB
{
    TechnicHUB();
    ~TechnicHUB();

    void init(bool withAddress = false);
    int CheckHUB();
    int Connect();
    void SetAddress(NimBLEAddress NewAddress);

    Lpf2Hub *Hub;
    std::string Address = "90:84:2b:7a:31:37";
};

struct LegoHubController // An instance for Controlling Lego HUBS
{
    LegoHubController(int numHUBs);
    ~LegoHubController();

    void init();
    void initHub(int num);
    int ConnectToAHub(int num);
    int ControlHubs(int numberOfHubs);

    // The hubs
    TechnicHUB *HUBS;
    // Variables what the program can use
    double vars[256];
    // External data from the Remote
    int RemoteData[64];
    // The Code
    byte HUBCode[MAX_PROG_MEM] = {0};
    // By default it doesn't do anything

private:
    // the number of hubs
    int _numHubs = 1;
    // the offset in the HUbCode
    int HUBCodeOffs = 0;
    // an object for controlling PF with IR
    PowerFunctions PFHUB;
    // arrays to store jump locations
    uint8_t else_end_locations[10];
    uint8_t else_locations[10];
    // variable to maintain how many addresses are int the arrays
    int num_if = 0;
    /*
    the value of the HUBCode variable is a binary code, what contains opcodes.
    The structure of the opcode descriptions is: (First Opcode name, then binary value,
    how many bytes long, description of the bytes)
    Every variable contains a singed integer.
    Byte 0 is always the opcode itself.

    End of program, 0x00, 1 byte

    Turn Motor by degrees, 0x01, 4 bytes
        byte 1: the hub number.
        byte 2: the port number A:0, B:1, C:2 ...
        byte 3: the variable number, what contains the angle.

    Turn on Motor with speed, 0x02, 4 bytes
        byte 1: the hub number.
        byte 2: the port number.
        byte 3: the variable number, what contains the speed.

    Turn on Motor for milliseconds, 0x03, 4 bytes
        byte 1: the hub number.
        byte 2: the port number.
        byte 3: the variable number, what contains the time.

    Turn Motor to absolute position, 0x04, 4 bytes
        byte 1: the hub number.
        byte 2: the port number.
        byte 3: the variable number, what contains the position.

    Set Motor encoder position, 0x05, 4 bytes
        byte 1: the hub number.
        byte 2: the port number.
        byte 3: the variable number, what contains the position.

    Jump to specified location, 0x06, 3 bytes
        byte 1 - 2: the address (byte 1 << 8 + byte 2) in te code.

    Set variable, 0x07, 10 bytes
        byte 1: the variable number.
        byte 2-9: the value you vant to set (double)

    Multiply, 0x08, 4 bytes
        byte 1: the number of the variable to be multiplied.
        byte 2: the variable number of the multiplier.
        byte 3: the variable number of the end result.

    Divide, 0x09, 4 bytes
        byte 1: the number of the variable to be divided.
        byte 2: the variable number of the divider.
        byte 3: the variable number of the end result.

    Add, 0x0a, 4 bytes
        byte 1: the number of the first variable.
        byte 2: the number of the second variable.
        byte 3: the variable number of the end result.

    Subtract, 0x0b, 4 bytes
        byte 1: the number of the first variable.
        byte 2: the number of the second variable.
        byte 3: the variable number of the end result.

    If, 0x0c, 4 bytes
        byte 1: the variable number
        byte 2-3: the location where it jumps if the variable is false;
        byte 4-5: the location where it jumps when executed the code(when the var is true);

    Boolean operations, 0x0d, 5 bytes
        byte 1: the number of the first variable.
        byte 2: the number of the second variable.
        byte 3: the code of the operation:
            0 - equals;
            1 - greater than;
            2 - less than;
            3 - or
            4 - and
            5 - not
        byte 4: the variable number of the end result(true/false).

    Math, 0x0e, 4 bytes
        byte 1: the variable number.
        byte 2: the code of the operation:
            0 - round;
            1 - abs;
            2 - sqrt;
            3 - sin;
            4 - cos;
            5 - tan;
            6 - log;
            7 - ln;
            8 - e^;
            9 - 10^;
            10 - asin;
            11 - acos;
            12 - atan;
            13 - floor;
            14 - ceiling;
        byte 3: the variable number of the end result.

    Mod, 0x0f, 4 bytes
        byte 1: the number of the first variable.
        byte 2: the number of the second variable.
        byte 3: the variable number of the end result.

    Turn on Motor with speed, 0x10, 4 bytes
        byte 1: the channel number.
        byte 2: the port number.
        byte 3: the variable number, what contains the speed (0 ... 255 mapped to -100 .. 100).

    Increment motor speed, 0x11, 3 bytes
        byte 1: the channel number.
        byte 2: the port number.

    Decrement motor speed, 0x11, 3 bytes
        byte 1: the channel number.
        byte 2: the port number.

    */
};

#define NUM_OPCODES 19
#define MAX_OPCODE_STR_LEN 22

const char *const opcode_names[NUM_OPCODES] = {
    "End of program",
    "H%i,p%i: v%i degrees",
    "H%i,p%i: speed v%i",
    "H%i,p%i: v%i milliseconds",
    "H%i,p%i: absolute v%i",
    "H%i,p%i: encoder v%i",
    "Jump to %i",
    "Set v%i to %i",
    "v%i * v%i = v%i",
    "v%i / v%i = v%i",
    "v%i + v%i = v%i",
    "v%i - v%i = v%i",
    "If v%i == true",
    "v%i = v%i %s v%i",
    "v%i = %s v%i",
    "v%i Mod v%i = v%i",
    "C%i, p%i: v%i",
    "C%i, p%i: Inc",
    "C%i, p%i: Dec",
};
const uint8_t opcode_byteCount[NUM_OPCODES] = {
    0,
    3,
    3,
    3,
    3,
    3,
    2,
    9,
    3,
    3,
    3,
    3,
    5,
    4,
    3,
    3,
    3,
    2,
    2,
};
const bool opcode_hasStr[NUM_OPCODES] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    true,
    true,
    false,
    false,
    false,
    false,
};
const char *const opcode_bool[6] = {
    "equals",
    "greater than",
    "less than",
    "or",
    "and",
    "not",
};
const char *const opcode_math[15] = {
    "round",
    "abs",
    "sqrt",
    "sin",
    "cos",
    "tan",
    "log",
    "ln",
    "e^",
    "10^",
    "asin",
    "acos",
    "atan",
    "floor",
    "ceiling",
};
const bool opcode_special_arguments[NUM_OPCODES] = {
    false, // 0
    false, // 1
    false, // 2
    false, // 3
    false, // 4
    false, // 5
    true,  // 6
    true,  // 7
    false, // 8
    false, // 9
    false, // 10
    false, // 11
    true,  // 12
    false, // 13
    false, // 14
    false, // 15
    false, // 16
    false, // 17
    false, // 18
};
const char *const opcode_argument_names[NUM_OPCODES * 4] = {
    "", // end of program
    "",
    "",
    "",
    // Turn Motor by degrees
    "HUB: %i",  // byte 1: the hub number.
    "Port: %i", // byte 2: the port number A:0, B:1, C:2 ...
    "var: %i",  // byte 3: the variable number, what contains the angle.
    "",
    // Turn on Motor with speed, 0x02, 4 bytes
    "HUB: %i",  // byte 1: the hub number.
    "Port: %i", // byte 2: the port number.
    "var: %i",  // byte 3: the variable number, what contains the speed.
    "",

    // Turn on Motor for milliseconds, 0x03, 4 bytes
    "HUB: %i",  // byte 1: the hub number.
    "Port: %i", // byte 2: the port number.
    "var: %i",  // byte 3: the variable number, what contains the time.
    "",

    // Turn Motor to absolute position, 0x04, 4 bytes
    "HUB: %i",  // byte 1: the hub number.
    "Port: %i", // byte 2: the port number.
    "var: %i",  // byte 3: the variable number, what contains the position.
    "",

    // Set Motor encoder position, 0x05, 4 bytes
    "HUB: %i",  // byte 1: the hub number.
    "Port: %i", // byte 2: the port number.
    "var: %i",  // byte 3: the variable number, what contains the position.
    "",

    // Jump to specified location, 0x06, 3 bytes
    "\x01\000a: %i", // byte 1 - 2: the address (byte 1 << 8 + byte 2) in te code.
    "",
    "",
    "",

    // Set variable, 0x07, 10 bytes
    "var: %i",       // byte 1: the variable number.
    "\x02\x01n: %f", // byte 2-9: the value you vant to set (double)
    "",
    "",

    // Multiply, 0x08, 4 bytes
    "var1: %i",    // byte 1: the number of the variable to be multiplied.
    "var2: %i",    // byte 2: the variable number of the multiplier.
    "var end: %i", // byte 3: the variable number of the end result.
    "",

    // Divide, 0x09, 4 bytes
    "var1: %i",    // byte 1: the number of the variable to be divided.
    "var2: %i",    // byte 2: the variable number of the divider.
    "var end: %i", // byte 3: the variable number of the end result.
    "",

    // Add, 0x0a, 4 bytes
    "var1: %i",    // byte 1: the number of the first variable.
    "var2: %i",    // byte 2: the number of the second variable.
    "var end: %i", // byte 3: the variable number of the end result.
    "",

    // Subtract, 0x0b, 4 bytes
    "var1: %i",    // byte 1: the number of the first variable.
    "var2: %i",    // byte 2: the number of the second variable.
    "var end: %i", // byte 3: the variable number of the end result.
    "",

    // If, 0x0c, 4 bytes
    "var: %i",       // byte 1: the variable number
    "\x01\001a: %i", // byte 2-3: the location where it jumps if the variable is false;
    "\x01\003a: %i", // byte 4-5: the location where it jumps when executed the code(when the var is true);
    "",

    // Boolean operations, 0x0d, 5 bytes
    "var1: %i", // byte 1: the number of the first variable.
    "var2: %i", // byte 2: the number of the second variable.
    "op: %i",   // byte 3: the code of the operation:
    /*0 - equals;
    1 - greater than;
    2 - less than;
    3 - or
    4 - and
    5 - not*/
    "var end: %i", // byte 4: the variable number of the end result(true/false).

    // Math, 0x0e, 4 bytes
    "var: %i", // byte 1: the variable number.
    "op: %i",  // byte 2: the code of the operation:
    /*0 - round;
    1 - abs;
    2 - sqrt;
    3 - sin;
    4 - cos;
    5 - tan;
    6 - log;
    7 - ln;
    8 - e^;
    9 - 10^;
    10 - asin;
    11 - acos;
    12 - atan;
    13 - floor;
    14 - ceiling;*/
    "var end: %i", // byte 3: the variable number of the end result.
    "",

    // Mod, 0x0f, 4 bytes
    "var1: %i",    // byte 1: the number of the first variable.
    "var2: %i",    // byte 2: the number of the second variable.
    "var end: %i", // byte 3: the variable number of the end result.
    "",

    // Turn on Motor with speed, 0x10, 4 bytes
    "Cannel: %i", // byte 1: the channel number.
    "Port: %i",   // byte 2: the port number.
    "var: %i",    // byte 3: the variable number, what contains the speed (0 ... 255 mapped to -100 .. 100).
    "",

    // Increment motor speed, 0x11, 3 bytes
    "Cannel: %i", // byte 1: the channel number.
    "Port: %i",   // byte 2: the port number.
    "",
    "",

    // Decrement motor speed, 0x11, 3 bytes
    "Cannel: %i", // byte 1: the channel number.
    "Port: %i",   // byte 2: the port number.
    "",
    "",
};
#endif