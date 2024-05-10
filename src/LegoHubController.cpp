#include <LegoHubController.h>

LegoHubController::LegoHubController(int numHUBs)
    : PFHUB(IR_PIN) // Set the IR pin
{
    if (numHUBs < 8) // cannot use more tha 7 hubs
    {
        _numHubs = numHUBs;
    }
};
LegoHubController::~LegoHubController()
{
    delete[] HUBS;
}

void LegoHubController::init()
{
    // create the hub objects
    HUBS = new TechnicHUB[_numHubs];

    HUBCodeOffs = 0;
    num_if = 0;
}
/**
 * Initializes a HUB
 * @param num the number of the hub to initialize.
 * */
void LegoHubController::initHub(int num)
{
    HUBS[num].init();
}
/**
 * @param numberOfHubs the number of hubs whoose are conneted.
 * @returns An Integer, If it's a non-zero number then some errors occured
 * 1: Invalid HUB number
 * 2: invalid opcode
 * 3: The HUB is disconnected
 * */
int LegoHubController::ControlHubs(int numberOfHubs)
{
    // variable to do math
    int var = 0;
    double var1 = 0;
    double var2 = 0;
    long long int ivar1 = 0;
    long long int ivar2 = 0;
    bool b1 = false;
    bool b2 = false;
    bool b3 = false;
    if (numberOfHubs > _numHubs)
    {
        log_e("too many hubs: %i",numberOfHubs);
        return 1;
    }
    for (int i = 0; i < numberOfHubs; i++)
    {
        if (HUBS[i].CheckHUB())
        {
            return 3;
        }
    }
    // if we are inside an if statement
    if(num_if)
    {
        // if we are at the else
        if(else_locations[num_if - 1] == HUBCodeOffs)
        {
            // decrease the if depth an jump to the end of the else
            num_if--;
            HUBCodeOffs = else_end_locations[num_if];
            log_debug("skip else: s=%i, e=%i, if statements",else_locations[num_if],else_end_locations[num_if], num_if);
        }
    }
    switch (HUBCode[HUBCodeOffs++])
    {
    case 0x00: // End of program
    {
        HUBCodeOffs--;
        return 0;
    }
    case 0x01: // Turn Motor by degrees
    {
        if (HUBCode[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[HUBCode[HUBCodeOffs++]];
        log_debug("Turn Motor at port: %i, by %f degrees, hub: %i", HUBCode[HUBCodeOffs - 2], var2, HUBCode[HUBCodeOffs - 3]);
        HUBS[HUBCode[HUBCodeOffs - 3]].Hub->setTachoMotorSpeedForDegrees(HUBCode[HUBCodeOffs - 2], 100, var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x02: // Turn on Motor with speed
    {
        if (HUBCode[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[HUBCode[HUBCodeOffs++]];
        log_debug("Starting Motor at port: %i, speed: %f, hub: %i", HUBCode[HUBCodeOffs - 2], var2, HUBCode[HUBCodeOffs - 3]);
        HUBS[HUBCode[HUBCodeOffs - 3]].Hub->setTachoMotorSpeed(HUBCode[HUBCodeOffs - 2], var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x03: // Turn on Motor for milliseconds
    {
        if (HUBCode[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[HUBCode[HUBCodeOffs++]];
        log_debug("Turn Motor at port: %i, for %f seconds, hub: %i", HUBCode[HUBCodeOffs - 2], var2 / 1000, HUBCode[HUBCodeOffs - 3]);
        HUBS[HUBCode[HUBCodeOffs - 3]].Hub->setTachoMotorSpeedForTime(HUBCode[HUBCodeOffs - 2], 100, var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x04: // Turn Motor to absolute position
    {
        if (HUBCode[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[HUBCode[HUBCodeOffs++]];
        log_debug("Turn Motor at port: %i, to absolute position %f, hub: %i", HUBCode[HUBCodeOffs - 2], var2, HUBCode[HUBCodeOffs - 3]);
        HUBS[HUBCode[HUBCodeOffs - 3]].Hub->setAbsoluteMotorPosition(HUBCode[HUBCodeOffs - 2], 100, var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x05: // Set Motor encoder position
    {
        if (HUBCode[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[HUBCode[HUBCodeOffs++]];
        log_debug("Set Motor encoder at port: %i, to absolute position %f, hub: %i", HUBCode[HUBCodeOffs - 2], var2, HUBCode[HUBCodeOffs - 3]);
        HUBS[HUBCode[HUBCodeOffs - 3]].Hub->setAbsoluteMotorEncoderPosition(HUBCode[HUBCodeOffs - 2], var2);
        break;
    }
    case 0x06: // Jump to specified location
    {
        var = *((uint8_t *)&HUBCode[HUBCodeOffs]);
        HUBCodeOffs++;
        HUBCodeOffs = var;
        log_debug("jump to %i",var);
        break;
    }
    case 0x07: // Set variable
    {
        vars[HUBCode[HUBCodeOffs++]] = *((double *)&HUBCode[HUBCodeOffs]);
        HUBCodeOffs += 8;
        break;
    }
    case 0x08: // Multiply
    {
        var2 = vars[HUBCode[HUBCodeOffs++]] * vars[HUBCode[HUBCodeOffs++]];
        vars[HUBCode[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x09: // Divide
    {
        var2 = vars[HUBCode[HUBCodeOffs++]] / vars[HUBCode[HUBCodeOffs++]];
        vars[HUBCode[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0a: // Add
    {
        var2 = vars[HUBCode[HUBCodeOffs++]] + vars[HUBCode[HUBCodeOffs++]];
        vars[HUBCode[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0b: // Subtract
    {
        var2 = vars[HUBCode[HUBCodeOffs++]] - vars[HUBCode[HUBCodeOffs++]];
        vars[HUBCode[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0c: // If
    {
        log_debug("if: %i, adrress 1: %i, 2: %i",vars[HUBCode[HUBCodeOffs++]],*((uint16_t *)&HUBCode[HUBCodeOffs + 1]),*((uint16_t *)&HUBCode[HUBCodeOffs + 3]));
        if (vars[HUBCode[HUBCodeOffs++]] != (int)0)
        {
            var = *((uint16_t *)&HUBCode[HUBCodeOffs++]);
            else_locations[num_if] = var;
            HUBCodeOffs++;
            var = *((uint16_t *)&HUBCode[HUBCodeOffs++]);
            else_end_locations[num_if] = var;
            HUBCodeOffs++;
            num_if++;
        }
        else
        {
            var = *((uint16_t *)&HUBCode[HUBCodeOffs++]);
            HUBCodeOffs = var;
        }
        break;
    }
    case 0x0d: // Boolean operations
    {
        if (vars[HUBCode[HUBCodeOffs++]])
        {
            b1 = true;
        }
        if (vars[HUBCode[HUBCodeOffs++]])
        {
            b2 = true;
        }
        switch (HUBCode[HUBCodeOffs++])
        {
        case 0: // equals
        {
            if (b1 == b2)
            {
                b3 = true;
            }
            break;
        }
        case 1: // greater than
        {
            if (b1 > b2)
            {
                b3 = true;
            }
            break;
        }
        case 2: // less than
        {
            if (b1 < b2)
            {
                b3 = true;
            }
            break;
        }
        case 3: // or
        {
            if (b1 || b2)
            {
                b3 = true;
            }
            break;
        }
        case 4: // and
        {
            if (b1 && b2)
            {
                b3 = true;
            }
            break;
        }
        case 5: // not
        {
            if (!b1)
            {
                b3 = true;
            }
            break;
        }
        default:
            return 1;
        }
        if (b3)
        {
            vars[HUBCode[HUBCodeOffs++]] = 1.0f;
        }
        else
        {
            vars[HUBCode[HUBCodeOffs++]] = 0.0f;
        }
        break;
    }
    case 0x0e: // Math
    {
        var1 = vars[HUBCode[HUBCodeOffs++]];

        switch (HUBCode[HUBCodeOffs++])
        {
        case 0: // round
        {
            var2 = round(var1);
            break;
        }
        case 1: // abs
        {
            var2 = abs(var1);
            break;
        }
        case 2: // sqrt
        {
            var2 = sqrt(var1);
            break;
        }
        case 3: // sin
        {
            var2 = sin(var1 / 180 * PI);
            break;
        }
        case 4: // cos
        {
            var2 = cos(var1 / 180 * PI);
            break;
        }
        case 5: // tan
        {
            var2 = tan(var1 / 180 * PI);
            break;
        }
        case 6: // lon
        {
            var2 = log(var1);
            break;
        }
        case 7: // ln
        {
            var2 = log1p(var1);
            break;
        }
        case 8: // e^
        {
            var2 = exp(var1);
            break;
        }
        case 9: // 10^
        {
            var2 = exp10(var1);
            break;
        }
        case 10: // asin
        {
            var2 = asin(var1) / PI * 180;
            break;
        }
        case 11: // acos
        {
            var2 = acos(var1) / PI * 180;
            break;
        }
        case 12: // atan
        {
            var2 = atan(var1) / PI * 180;
            break;
        }
        case 13: // floor
        {
            var2 = floor(var1);
            break;
        }
        case 14: // ceiling
        {
            var2 = ceil(var1);
            break;
        }
        default:
            return 1;
        }
        vars[HUBCode[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0f: // mod
    {
        ivar1 = vars[HUBCode[HUBCodeOffs++]];
        ivar2 = vars[HUBCode[HUBCodeOffs++]];
        vars[HUBCode[HUBCodeOffs++]] = ivar1 % ivar2;
        break;
    }
    case 0x10: // Turn on Motor with speed
    {
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = (int)map(vars[HUBCode[HUBCodeOffs++]], 0, 255, -100, 100);
        log_debug("Starting Motor at port: %s, speed: %f, channel: %i", HUBCode[HUBCodeOffs - 2] ? "blue" : "red", var2, HUBCode[HUBCodeOffs - 3]);
        PFHUB.single_pwm((PowerFunctionsPort)HUBCode[HUBCodeOffs - 2], PFHUB.speedToPwm(var2), HUBCode[HUBCodeOffs - 3]);
        break;
    }
    case 0x11: // Increment motor speed
    {
        HUBCodeOffs += 2;
        log_debug("Increment Motor speed at port: %s, channel: %i", HUBCode[HUBCodeOffs - 1] ? "blue" : "red", HUBCode[HUBCodeOffs - 2]);
        PFHUB.single_increment((PowerFunctionsPort)HUBCode[HUBCodeOffs - 1], HUBCode[HUBCodeOffs - 2]);
        break;
    }
    case 0x12: // Decremnet motor speed
    {
        HUBCodeOffs += 2;
        log_debug("Decrement Motor speed at port: %s, channel: %i", HUBCode[HUBCodeOffs - 1] ? "blue" : "red", HUBCode[HUBCodeOffs - 2]);
        PFHUB.single_decrement((PowerFunctionsPort)HUBCode[HUBCodeOffs - 1], HUBCode[HUBCodeOffs - 2]);
        break;
    }
    default:
    {
        log_e("Invlaid opcode %i", HUBCode[HUBCodeOffs - 1]);
        return 2;
    }
    }
    return 0;
}
/**
 * connect to a hub
 * @param num the hub number
 * @returns 1 - if it succseeds
 * @returns 2 - if it fails
*/
int LegoHubController::ConnectToAHub(int num)
{
    if (HUBS[num].CheckHUB())
    {
        return 0;
    }
    HUBS[num].Hub->setLedRGBColor(0, 255, 0);
    return 1;
}

TechnicHUB::TechnicHUB()
{
}
TechnicHUB::~TechnicHUB()
{
    delete Hub;
}
/**
 * initializes the HUB(start advertising)
*/
void TechnicHUB::init(bool withAddress)
{
    Hub = new Lpf2Hub;
    if (withAddress)
    {
        Hub->init(Address);
        return;
    }
    Hub->init();
}
/**
 * checks the HUB if it isn't connected tryes to connect
 * @returns 1 - if fail
*/
int TechnicHUB::CheckHUB()
{
    // connect flow. Search for BLE services and try to connect if the uuid of the hub is found
    if (Hub->isConnecting())
    {
        log_debug("Trying to connect... :%i", 0);
        Hub->connectHub();
        if (Hub->isConnected())
        {
            log_debug("We are now connected to the HUB");
            delay(1000);
        }
        else
        {
            log_debug("We have failed to connect to the HUB");
            return 1;
        }
    }
    if (Hub->isConnected())
    {
        return 0;
    }
    return 1;
}
/**
 * connects to the HUB
 * @returns 1 - if fail
*/
int TechnicHUB::Connect()
{
    return CheckHUB();
}
/**
 * sets the MAC address of that hub, what you want to connect to
 * @param NewAddress the new MAC address
*/
void TechnicHUB::SetAddress(NimBLEAddress NewAddress)
{
    Address = NewAddress.toString();
}