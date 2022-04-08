/*
 * THIS FILE IS AUTOMATICALLY GENERATED
 *
 * Generator:    sensirion-driver-generator 0.9.0
 * Product:      scd30
 * Version:      None
 */
/*
 * Copyright (c) 2022, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "scd30_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"
#include <stdio.h>  // printf

#define sensirion_hal_sleep_us sensirion_i2c_hal_sleep_usec

int main(void) {
    int16_t error = NO_ERROR;
    sensirion_i2c_hal_init();
    init_driver(SCD30_I2C_ADDR_61);

    scd30_soft_reset();
    sensirion_hal_sleep_us(2000000);
    uint8_t major = 0;
    uint8_t minor = 0;
    error = scd30_read_firmware_version(&major, &minor);
    if (error != NO_ERROR) {
        printf("error executing read_firmware_version(): %i\n", error);
        return error;
    }
    printf("major: %u ", major);
    printf("minor: %u\n", minor);
    error = scd30_start_periodic_measurement(0);
    if (error != NO_ERROR) {
        printf("error executing start_periodic_measurement(): %i\n", error);
        return error;
    }
    float co2_concentration = 0.0;
    float temperature = 0.0;
    float humidiy = 0.0;
    uint16_t repetition = 0;
    for (repetition = 0; repetition < 30; repetition++) {
        sensirion_hal_sleep_us(1500000);
        error = scd30_blocking_read_measurement_data(&co2_concentration,
                                                     &temperature, &humidiy);
        if (error != NO_ERROR) {
            printf("error executing blocking_read_measurement_data(): %i\n",
                   error);
            continue;
        }
        printf("co2_concentration: %.2f ", co2_concentration);
        printf("temperature: %.2f ", temperature);
        printf("humidiy: %.2f\n", humidiy);
    }

    error = scd30_soft_reset();
    if (error != NO_ERROR) {
        return error;
    }
    return NO_ERROR;
}
