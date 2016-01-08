/*
 * Copyright 2016 BrewPi/Elco Jacobs.
 * Copyright 2016 Matthew McGowan.
 *
 * This file is part of BrewPi.
 *
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ControllerMixins.h"
#include "Pid.h"
#include "TempSensor.h"
#include "TempSensorBasic.h"
#include "TempSensorMock.h"
#include "OneWireTempSensor.h"
#include "TempSensorExternal.h"
#include "ValveController.h"
#include "ActuatorInterfaces.h"
#include "ActuatorTimeLimited.h"
#include "ActuatorSetPoint.h"
#include "ActuatorPwm.h"
#include "ActuatorOneWire.h"
#include "ActuatorMutexGroup.h"
#include "ActuatorMutexDriver.h"
#include "ActuatorMocks.h"

// These macros are equivalent to ESJ, except for that they add obj-> in front of the member variable name
// use JSON_E for local variables and JSON_OE for member variables of the obj pointer

// Serializable element with more elements to follow (alternative to JSON_E)
#define JSON_OE(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,true)
// Serializable terminator, keep things simple for the JSON writer. No stream manipulations required (alternative to JSON_T)
#define JSON_OT(json_adapter,class_member)  JSON::stream(json_adapter,_ASTRING(#class_member),obj->class_member,false)



void PidMixin::serialize(JSON::Adapter & adapter)
{
    Pid * obj = (Pid *) this;

    JSON::Class root(adapter, "Pid");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OE(adapter, enabled);
    JSON_OE(adapter, setPoint);
    JSON_OE(adapter, inputSensor);
    JSON_OE(adapter, inputError);
    JSON_OE(adapter, Kp);
    JSON_OE(adapter, Ti);
    JSON_OE(adapter, Td);
    JSON_OE(adapter, p);
    JSON_OE(adapter, i);
    JSON_OE(adapter, d);
    JSON_OE(adapter, actuatorIsNegative);
    JSON_OT(adapter, outputActuator);
}


void TempSensorMixin::serialize(JSON::Adapter & adapter)
{
    TempSensor * obj = (TempSensor *) this;

    JSON::Class root(adapter, "TempSensor");
    std::string name(getName());    // get name as std string for json_writer
    JSON_E(adapter, name);
    JSON_OT(adapter, sensor);
}


void TempSensorMockMixin::serialize(JSON::Adapter& adapter){
    TempSensorMock * obj = (TempSensorMock *) this;

    JSON::Class root(adapter, "TempSensorMock");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}


void OneWireTempSensorMixin::serialize(JSON::Adapter& adapter){
    OneWireTempSensor * obj = (OneWireTempSensor *) this;

    JSON::Class root(adapter, "OneWireTempSensor");

    temp_t value = obj->read();
    JSON_E(adapter, value);

    bool connected = obj->isConnected();
    JSON_E(adapter, connected);

    char addressBuf[17];
    printBytes(obj->sensorAddress, 8, addressBuf); // print to hex string
    std::string address(addressBuf); // convert to std string
    JSON_E(adapter, address);

    JSON_OT(adapter, calibrationOffset);
}


void TempSensorDisconnectedMixin::serialize(JSON::Adapter& adapter){
    TempSensorDisconnected * obj = (TempSensorDisconnected *) this;

    temp_t value = obj->read();
    bool connected = false;
    JSON::Class root(adapter, "TempSensorDisconnected");
    JSON_E(adapter, value);
    JSON_T(adapter, connected);
}

void TempSensorExternalMixin::serialize(JSON::Adapter& adapter){
    TempSensorExternal * obj = (TempSensorExternal *) this;

    JSON::Class root(adapter, "TempSensorExternal");
    JSON_OE(adapter, value);
    JSON_OT(adapter, connected);
}

void ValveControllerMixin::serialize(JSON::Adapter& adapter){
    ValveController * obj = (ValveController *) this;

    JSON::Class root(adapter, "ValveController");
    JSON_OE(adapter, pio);
    JSON_OT(adapter, sense);
}

void ActuatorTimeLimitedMixin::serialize(JSON::Adapter& adapter){
    ActuatorTimeLimited * obj = (ActuatorTimeLimited *) this;

    JSON::Class root(adapter, "ActuatorTimeLimited");
    JSON_OE(adapter, minOnTime);
    JSON_OE(adapter, minOffTime);
    JSON_OE(adapter, maxOnTime);
    JSON_OE(adapter, active);
    JSON_OT(adapter, target);
}

void ActuatorSetPointMixin::serialize(JSON::Adapter& adapter){
    ActuatorSetPoint * obj = (ActuatorSetPoint *) this;

    JSON::Class root(adapter, "ActuatorSetPoint");
    JSON_OE(adapter, targetSetPoint);
    JSON_OE(adapter, targetSensor);
    JSON_OE(adapter, referenceSetPoint);
    temp_t output = obj->getValue();
    JSON_E(adapter, output);
    temp_t achieved = obj->readValue();
    JSON_E(adapter, achieved);
    JSON_OE(adapter, minimum);
    JSON_OT(adapter, maximum);
}

void ActuatorPwmMixin::serialize(JSON::Adapter& adapter){
    ActuatorPwm * obj = (ActuatorPwm *) this;

    JSON::Class root(adapter, "ActuatorPwm");
    JSON_OE(adapter, value);
    ticks_seconds_t period = obj->getPeriod(); // don't use member directly, but value in seconds
    JSON_E(adapter, period);
    JSON_OE(adapter, minVal);
    JSON_OE(adapter, maxVal);
    JSON_OT(adapter, target);
}

void ActuatorOneWireMixin::serialize(JSON::Adapter& adapter){
    ActuatorOneWire * obj = (ActuatorOneWire *) this;

    JSON::Class root(adapter, "ActuatorOneWire");
    bool active = obj->isActive();
    JSON_E(adapter, active);
    JSON_OE(adapter, pio);
    JSON_OT(adapter, invert);
}

void ActuatorMutexGroupMixin::serialize(JSON::Adapter& adapter){
    ActuatorMutexGroup * obj = (ActuatorMutexGroup *) this;

    JSON::Class root(adapter, "ActuatorMutexGroup");
    JSON_OE(adapter, deadTime);
    JSON_OT(adapter, lastActiveTime);
}


void ActuatorMutexDriverMixin::serialize(JSON::Adapter& adapter){
    ActuatorMutexDriver * obj = (ActuatorMutexDriver *) this;

    JSON::Class root(adapter, "ActuatorMutexDriver");
    JSON_OE(adapter, mutexGroup);
    JSON_OT(adapter, target);
}

void ActuatorValueMixin::serialize(JSON::Adapter& adapter){
    ActuatorValue * obj = (ActuatorValue *) this;

    JSON::Class root(adapter, "ActuatorValue");
    JSON_OE(adapter, value);
    JSON_OE(adapter, minimum);
    JSON_OT(adapter, maximum);
}

void ActuatorBoolMixin::serialize(JSON::Adapter& adapter){
    ActuatorBool * obj = (ActuatorBool *) this;

    JSON::Class root(adapter, "ActuatorBool");
    JSON_OT(adapter, state);
}

void ActuatorNopMixin::serialize(JSON::Adapter& adapter){
    ActuatorNop * obj = (ActuatorNop *) this;

    JSON::Class root(adapter, "ActuatorNop");
    bool state = obj->isActive();
    JSON_T(adapter, state);
}

void ActuatorInvalidMixin::serialize(JSON::Adapter& adapter){
    ActuatorInvalid * obj = (ActuatorInvalid *) this;

    temp_t value = obj->getValue();
    temp_t minimum = obj->min();
    temp_t maximum = obj-> max();

    JSON::Class root(adapter, "ActuatorInvalid");
    JSON_E(adapter, value);
    JSON_E(adapter, minimum);
    JSON_T(adapter, maximum);
}
