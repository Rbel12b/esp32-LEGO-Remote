#include <LegoHubController.h>

LegoHubController::LegoHubController(int numHUBs)
    : PFHUB(4)
{
    if (numHUBs < 8)
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
    HUBS = new TechnicHUBConf[_numHubs];

    HUBCodeOffs = 0;
    num_if = 0;
}
void LegoHubController::initHub(int num)
{
    HUBS[num].init();
}
/**
 * @param numberOfHubs the amount of hubs that is conneted.
 * @returns An Integer, If it's a non-zero number then some errors occured
 * 1: Invalid HUB number
 * 2: invalid opcode
 * */
int LegoHubController::ControlHubs(int numberOfHubs)
{
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
            return 0;
        }
    }
    if(num_if)
    {
        if(else_locations[num_if - 1] == HUBCodeOffs)
        {
            num_if--;
            HUBCodeOffs = else_end_locations[num_if];
            log_debug("skip else: s=%i, e=%i, if statements",else_locations[num_if],else_end_locations[num_if], num_if);
        }
    }
    switch (data[HUBCodeOffs++])
    {
    case 0x00: // End of program
    {
        HUBCodeOffs--;
        return 0;
    }
    case 0x01: // Turn Motor by degrees
    {
        if (data[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[data[HUBCodeOffs++]];
        log_debug("Turn Motor at port: %i, by %f degrees, hub: %i", data[HUBCodeOffs - 2], var2, HUBnumTable[data[HUBCodeOffs - 3]]);
        HUBS[HUBnumTable[data[HUBCodeOffs - 3]]].Hub->setTachoMotorSpeedForDegrees(data[HUBCodeOffs - 2], 100, var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x02: // Turn on Motor with speed
    {
        if (data[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[data[HUBCodeOffs++]];
        log_debug("Starting Motor at port: %i, speed: %f, hub: %i", data[HUBCodeOffs - 2], var2, HUBnumTable[data[HUBCodeOffs - 3]]);
        HUBS[HUBnumTable[data[HUBCodeOffs - 3]]].Hub->setTachoMotorSpeed(data[HUBCodeOffs - 2], var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x03: // Turn on Motor for milliseconds
    {
        if (data[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[data[HUBCodeOffs++]];
        log_debug("Turn Motor at port: %i, for %f seconds, hub: %i", data[HUBCodeOffs - 2], var2 / 1000, HUBnumTable[data[HUBCodeOffs - 3]]);
        HUBS[HUBnumTable[data[HUBCodeOffs - 3]]].Hub->setTachoMotorSpeedForTime(data[HUBCodeOffs - 2], 100, var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x04: // Turn Motor to absolute position
    {
        if (data[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[data[HUBCodeOffs++]];
        log_debug("Turn Motor at port: %i, to absolute position %f, hub: %i", data[HUBCodeOffs - 2], var2, HUBnumTable[data[HUBCodeOffs - 3]]);
        HUBS[HUBnumTable[data[HUBCodeOffs - 3]]].Hub->setAbsoluteMotorPosition(data[HUBCodeOffs - 2], 100, var2, 100, BrakingStyle::HOLD);
        break;
    }
    case 0x05: // Set Motor encoder position
    {
        if (data[HUBCodeOffs] >= _numHubs)
        {
            HUBCodeOffs--;
            return 1; // Invalid Hub number
        }
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = vars[data[HUBCodeOffs++]];
        log_debug("Set Motor encoder at port: %i, to absolute position %f, hub: %i", data[HUBCodeOffs - 2], var2, HUBnumTable[data[HUBCodeOffs - 3]]);
        HUBS[HUBnumTable[data[HUBCodeOffs - 3]]].Hub->setAbsoluteMotorEncoderPosition(data[HUBCodeOffs - 2], var2);
        break;
    }
    case 0x06: // Jump to specified location
    {
        var = *((uint8_t *)&data[HUBCodeOffs]);
        HUBCodeOffs++;
        HUBCodeOffs = var;
        log_debug("jump to %i",var);
        break;
    }
    case 0x07: // Set variable
    {
        vars[data[HUBCodeOffs++]] = *((double *)&data[HUBCodeOffs]);
        HUBCodeOffs += 8;
        break;
    }
    case 0x08: // Multiply
    {
        var2 = vars[data[HUBCodeOffs++]] * vars[data[HUBCodeOffs++]];
        vars[data[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x09: // Divide
    {
        var2 = vars[data[HUBCodeOffs++]] / vars[data[HUBCodeOffs++]];
        vars[data[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0a: // Add
    {
        var2 = vars[data[HUBCodeOffs++]] + vars[data[HUBCodeOffs++]];
        vars[data[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0b: // Subtract
    {
        var2 = vars[data[HUBCodeOffs++]] - vars[data[HUBCodeOffs++]];
        vars[data[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0c: // If
    {
        log_debug("if: %i, adrress 1: %i, 2: %i",vars[data[HUBCodeOffs++]],*((uint16_t *)&data[HUBCodeOffs + 1]),*((uint16_t *)&data[HUBCodeOffs + 3]));
        if (vars[data[HUBCodeOffs++]] != (int)0)
        {
            var = *((uint16_t *)&data[HUBCodeOffs++]);
            else_locations[num_if] = var;
            HUBCodeOffs = var;
            HUBCodeOffs++;
            var = *((uint16_t *)&data[HUBCodeOffs++]);
            else_end_locations[num_if] = var;
            HUBCodeOffs++;
            num_if++;
        }
        else
        {
            HUBCodeOffs += 4;
        }
        break;
    }
    case 0x0d: // Boolean operations
    {
        if (vars[data[HUBCodeOffs++]])
        {
            b1 = true;
        }
        if (vars[data[HUBCodeOffs++]])
        {
            b2 = true;
        }
        switch (data[HUBCodeOffs++])
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
            vars[data[HUBCodeOffs++]] = 1.0f;
        }
        else
        {
            vars[data[HUBCodeOffs++]] = 0.0f;
        }
        break;
    }
    case 0x0e: // Math
    {
        var1 = vars[data[HUBCodeOffs++]];

        switch (data[HUBCodeOffs++])
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
        vars[data[HUBCodeOffs++]] = var2;
        break;
    }
    case 0x0f:
    {
        ivar1 = vars[data[HUBCodeOffs++]];
        ivar2 = vars[data[HUBCodeOffs++]];
        vars[data[HUBCodeOffs++]] = ivar1 % ivar2;
        break;
    }
    case 0x10: // Turn on Motor with speed
    {
        HUBCodeOffs++;
        HUBCodeOffs++;
        var2 = (int)map(vars[data[HUBCodeOffs++]], 0, 255, -100, 100);
        log_debug("Starting Motor at port: %s, speed: %f, channel: %i", data[HUBCodeOffs - 2] ? "blue" : "red", var2, data[HUBCodeOffs - 3]);
        PFHUB.single_pwm((PowerFunctionsPort)data[HUBCodeOffs - 2], PFHUB.speedToPwm(var2), data[HUBCodeOffs - 3]);
        break;
    }
    case 0x11: // Increment motor speed
    {
        HUBCodeOffs += 2;
        log_debug("Increment Motor speed at port: %s, channel: %i", data[HUBCodeOffs - 1] ? "blue" : "red", data[HUBCodeOffs - 2]);
        PFHUB.single_increment((PowerFunctionsPort)data[HUBCodeOffs - 1], data[HUBCodeOffs - 2]);
        break;
    }
    case 0x12: // Decremnet motor speed
    {
        HUBCodeOffs += 2;
        log_debug("Decrement Motor speed at port: %s, channel: %i", data[HUBCodeOffs - 1] ? "blue" : "red", data[HUBCodeOffs - 2]);
        PFHUB.single_decrement((PowerFunctionsPort)data[HUBCodeOffs - 1], data[HUBCodeOffs - 2]);
        break;
    }
    default:
    {
        log_e("Invlaid opcode %i", data[HUBCodeOffs - 1]);
        return 2;
    }
    }
    return 0;
}

int LegoHubController::ConnectToAHub(int num)
{
    if (HUBS[num].CheckHUB())
    {
        return 0;
    }
    HUBS[num].Hub->setLedRGBColor(0, 255, 0);
    return 1;
}
NimBLEAddress LegoHubController::getAddressOfConnectedHub(int num)
{
    Serial.println(HUBS[num].Hub->getHubAddress());
    return HUBS[num].Hub->getHubAddress();
}
void LegoHubController::setAddressOfHubByNum(int numHub, NimBLEAddress Address)
{
    HUBS->Hub->shutDownHub();
    // HUBS[numHub].SetAddress(Address);
    HUBS[numHub].init(false);
}

TechnicHUBConf::TechnicHUBConf()
{
}
TechnicHUBConf::~TechnicHUBConf()
{
    delete Hub;
}
void TechnicHUBConf::init(bool withAddress)
{
    Hub = new Lpf2Hub;
    if (withAddress)
    {
        Hub->init(Address);
        return;
    }
    Hub->init();
}
int TechnicHUBConf::CheckHUB()
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
int TechnicHUBConf::Connect()
{
    return CheckHUB();
}
void TechnicHUBConf::SetAddress(NimBLEAddress NewAddress)
{
    Address = NewAddress.toString();
}