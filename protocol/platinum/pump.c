/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017-2019  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "device.h"
#include "driver.h"
#include "lowlevel/platinum.h"
#include "print.h"
#include "protocol/platinum.h"

#include <errno.h>
#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
corsairlink_platinum_pump_mode(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[0x00] = CommandId++;
    commands[0x00] = WriteOneByte;
    commands[0x00] = FAN_Select;
    commands[0x00] = ctrl->channel;

    commands[0x00] = CommandId++;
    commands[0x00] = ReadOneByte;
    commands[0x00] = FAN_Mode;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = response[4];

    return rr;
}

int
corsairlink_platinum_pump_mode_read(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];

    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[0x00] = CommandId++;
    commands[0x00] = WriteOneByte;
    commands[0x00] = FAN_Select;
    commands[0x00] = ctrl->channel;

    commands[0x00] = CommandId++;
    commands[0x00] = ReadOneByte;
    commands[0x00] = FAN_Mode;

    commands[0] = ii;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    ctrl->mode = response[4];

    return rr;
}

int
corsairlink_platinum_pump_mode_quiet(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    // PLATINUM_FAN_TABLE_QUIET(*ctrl);
    return corsairlink_platinum_pump_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_pump_mode_balanced(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    // PLATINUM_FAN_TABLE_BALANCED(*ctrl);
    return corsairlink_platinum_pump_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_pump_mode_performance(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    // PLATINUM_FAN_TABLE_EXTREME(*ctrl);
    return corsairlink_platinum_pump_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_pump_mode_custom(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    return corsairlink_platinum_pump_curve(dev, handle, ctrl);
}

int
corsairlink_platinum_pump_mode_default(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    return corsairlink_platinum_pump_mode_balanced(dev, handle, ctrl);
}

int
corsairlink_platinum_pump_curve(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    // commands[0] = FanCurve;
    // commands[1] = UnknownFanCurve;

    commands[0x00] = CommandId++;
    commands[0x00] = WriteThreeBytes;
    commands[0x00] = FAN_TempTable;
    commands[0x00] = 0x0A;

    commands[0x00] = ctrl->table[0].temperature;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[1].temperature;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[2].temperature;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[3].temperature;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[4].temperature;
    commands[0x00] = 0x00;

    commands[0x00] = CommandId++;
    commands[0x00] = WriteThreeBytes;
    commands[0x00] = FAN_RPMTable;
    commands[0x00] = 0x0A;

    commands[0x00] = ctrl->table[0].speed;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[1].speed;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[2].speed;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[3].speed;
    commands[0x00] = 0x00;
    commands[0x00] = ctrl->table[4].speed;
    commands[0x00] = 0x00;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    return rr;
}

int
corsairlink_platinum_pump_speed(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    struct pump_control* ctrl )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    commands[0x00] = 0x3F;
    commands[0x01] = CommandId++;

    commands[0x00] = 0xFF;

    commands[0x3F] = 0x00;
    commands[0x40] = 0xFF;

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    msg_debug2( "Speed: %02X %02X\n", response[0x1D], response[0x1E] );
    msg_debug2( "Max Speed: %02X %02X\n", response[9], response[8] );

    ctrl->speed = ( response[0x1E] << 8 ) + response[0x1D];
    ctrl->max_speed = 0xFFFF;

    return rr;
}
