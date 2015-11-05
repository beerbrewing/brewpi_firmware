/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#include "ValveController.h"

ValveController::ValveController() :
    switchState(0xff), // Set outputs and inputs to OFF state
    sense(0b11), // Set sense to OFF state (in between)
    act(0b11), // set output to OFF (not open/closed, no action)
    channel('X') // Let 'X' mean that the channel is undefined
{
}

ValveController::~ValveController() {
}

/*
 * Updates the status of the member variables from what is read back from the valve
 * Checks whether the valve are is done with opening/closing and stops driving it.
 */

uint8_t maskSenseBits(uint8_t input){
    return input & 0b00110011;
}
uint8_t maskWriteBitsA(uint8_t input){
    return input & 0b00111111;
}
uint8_t maskWriteBitsB(uint8_t input){
    return input & 0b11110011;
}

void ValveController::update() {
    switchState = accessRead();
    // content of switchState:
    // bit 7-6: Valve A action: 01 = open, 10 = close, 11 = off, 00 = off but LEDS on
    // bit 5-4: Valve A status: 01 = opened, 10 = closed, 11 = in between
    // bit 3-2: Valve B action: 01 = open, 10 = close, 11 = off, 00 = off but LEDS on
    // bit 1-0: Valve B status: 01 = opened, 10 = closed, 11 = in between

    uint8_t output = switchState;

    if(channel == 'A'){
        act = (switchState >> 6) & 0x3;
        sense = (switchState >> 4) & 0x3;
        if (act == sense) {
            // fully opened/closed. Stop driving the valve
            output |= (uint8_t(ValveActions::OFF) << 6);
        }
    }
    else if(channel == 'B'){
        act = (switchState >> 2) & 0x3;
        sense = switchState & 0x3;
        if (act == sense) {
            // fully opened/closed. Stop driving the valve
            output |= (uint8_t(ValveActions::OFF) << 2);
        }
    }
    if (output != switchState) {
        accessWrite(maskSenseBits(output)); // write new state, but ensure keep sense bits as inputs
    }
}

uint8_t ValveController::read(bool doUpdate) {
    if (doUpdate) {
        update();
    }
    return sense;
}

void ValveController::write(ValveActions action) {
    update();
    uint8_t action_ = uint8_t(action);
    uint8_t output = maskSenseBits(switchState);
    if(channel == 'A'){
        output = maskWriteBitsA(output);
        output |= action_ << 6;
        accessWrite(output);
    }
    else if(channel == 'B'){
        output = maskWriteBitsB(output);
        output |= action_ << 2;
        accessWrite(output);
    }
}
