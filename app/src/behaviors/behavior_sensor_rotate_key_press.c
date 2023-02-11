/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_sensor_rotate_key_press

#include <device.h>
#include <drivers/behavior.h>
#include <logging/log.h>

#include <drivers/sensor.h>
#include <zmk/behavior_queue.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int behavior_sensor_rotate_key_press_init(const struct device *dev) { return 0; };

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    uint32_t keycode;

    switch (event.position) {
    case 1:
        keycode = binding->param1;
        break;
    case 2:
        keycode = binding->param2;
        break;
    default:
        return -ENOTSUP;
    }

    return ZMK_EVENT_RAISE(zmk_keycode_state_changed_from_encoded(keycode, true, event.timestamp));
}

static int on_keymap_binding_released(struct zmk_behavior_binding *binding,
                                      struct zmk_behavior_binding_event event) {
    uint32_t keycode;

    switch (event.position) {
    case 1:
        keycode = binding->param1;
        break;
    case 2:
        keycode = binding->param2;
        break;
    default:
        return -ENOTSUP;
    }

    return ZMK_EVENT_RAISE(zmk_keycode_state_changed_from_encoded(keycode, false, event.timestamp));
}

static int on_sensor_binding_triggered(struct zmk_behavior_binding *binding,
                                       const struct sensor_value value, int64_t timestamp) {
    int err;
    uint32_t keycode_position;
    LOG_DBG("inc keycode 0x%02X dec keycode 0x%02X", binding->param1, binding->param2);

    switch (value.val1) {
    case 1:
        keycode_position = 1;
        break;
    case -1:
        keycode_position = 2;
        break;
    default:
        return -ENOTSUP;
    }

    zmk_behavior_queue_add(keycode_position, *binding, true, 20);
    return zmk_behavior_queue_add(keycode_position, *binding, false, 20);
}

static const struct behavior_driver_api behavior_sensor_rotate_key_press_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
    .binding_released = on_keymap_binding_released,
    .sensor_binding_triggered = on_sensor_binding_triggered};

#define KP_INST(n)                                                                                 \
    DEVICE_DT_INST_DEFINE(n, behavior_sensor_rotate_key_press_init, NULL, NULL, NULL, APPLICATION, \
                          CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                     \
                          &behavior_sensor_rotate_key_press_driver_api);

DT_INST_FOREACH_STATUS_OKAY(KP_INST)

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
