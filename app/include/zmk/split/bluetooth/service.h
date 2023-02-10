/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <drivers/sensor.h>

#define ZMK_SPLIT_RUN_BEHAVIOR_DEV_LEN 9
#define ZMK_SPLIT_POS_STATE_LEN 16

struct zmk_split_run_behavior_data {
    uint8_t position;
    uint8_t state;
    uint32_t param1;
    uint32_t param2;
} __packed;

struct zmk_split_run_behavior_payload {
    struct zmk_split_run_behavior_data data;
    char behavior_dev[ZMK_SPLIT_RUN_BEHAVIOR_DEV_LEN];
} __packed;

typedef enum zmk_split_notify_data_type {
    ZMK_SPLIT_NOTIFIY_DATA_POS,
    ZMK_SPLIT_NOTIFIY_DATA_SENSOR,
} zmk_split_notify_data_t;

struct zmk_split_notify_data_position {
    const zmk_split_notify_data_t type;
    uint8_t position_state[ZMK_SPLIT_POS_STATE_LEN];
} __packed;

struct zmk_split_notify_data_sensor {
    const zmk_split_notify_data_t type;
    uint8_t sensor_number;
    struct sensor_value value;
} __packed;

int zmk_split_bt_position_pressed(uint8_t position);
int zmk_split_bt_position_released(uint8_t position);
int zmk_split_bt_sensor_triggered(uint8_t sensor_number, const struct sensor_value value, int64_t timestamp);

