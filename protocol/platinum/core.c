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
corsairlink_platinum_device_id(
    struct corsair_device_info* dev, struct libusb_device_handle* handle, uint8_t* device_id )
{
    ( *device_id ) = 0xFF;
    return 0;
}

int
corsairlink_platinum_name(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size )
{
    snprintf( name, name_size, "%s", dev->name );
    return 0;
}

int
corsairlink_platinum_vendor(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size )
{
    snprintf( name, name_size, "Corsair" );
    return 0;
}

int
corsairlink_platinum_product(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* name,
    uint8_t name_size )
{
    snprintf( name, name_size, "%s", dev->name );
    return 0;
}

int
corsairlink_platinum_firmware_id(
    struct corsair_device_info* dev,
    struct libusb_device_handle* handle,
    char* firmware,
    uint8_t firmware_size )
{
    int rr;
    uint8_t response[64];
    uint8_t commands[64];
    memset( response, 0, sizeof( response ) );
    memset( commands, 0, sizeof( commands ) );

    uint8_t ii = 0;

    commands[0x00] = 0x3F;
    commands[0x01] = CommandId++; // Command ID
    commands[0x02] = 0xFF; // Command Opcode
    commands[0x40] = 0xD8; // CRC ??
    // commands[0] = ii; // Length

    rr = dev->driver->write( handle, dev->write_endpoint, commands, 64 );
    rr = dev->driver->read( handle, dev->read_endpoint, response, 64 );

    snprintf(
        firmware, firmware_size, "%d.%d.%d", ( response[3] & 240 ) >> 4, ( response[3] & 15 ),
        response[2] );

    return 0;
}
