/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
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

#pragma once

#include "TempSensor.h"
#include "ControllerMixins.h"

class TempSensorDisconnected final : public TempSensor, public TempSensorDisconnectedMixin {
	
public:
    void accept(VisitorBase & v) final{
    	v.visit(*this);
    }

	bool isConnected() const final { return false; }

	bool init() final {
		return read()!=TEMP_SENSOR_DISCONNECTED;
	}
	
	temp_t read() const final {
		return TEMP_SENSOR_DISCONNECTED;
	}

    void update() final {
        // nop for this mock sensor
    }
	
    friend class TempSensorDisconnectedMixin;
};

