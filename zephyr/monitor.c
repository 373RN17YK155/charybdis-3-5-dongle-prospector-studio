#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/keymap.h>

LOG_MODULE_REGISTER(monitor, CONFIG_ZMK_LOG_LEVEL_DBG);

static int monitor_battery_listener(const zmk_event_t *eh) {
    const struct zmk_peripheral_battery_state_changed *ev =
        as_zmk_peripheral_battery_state_changed(eh);
    if (ev) {
        printk("MONITOR:BAT:%u:%u\n", ev->source, ev->state_of_charge);
        return 0;
    }

    const struct zmk_battery_state_changed *local =
        as_zmk_battery_state_changed(eh);
    if (local) {
        printk("MONITOR:BAT:LOCAL:%u\n", local->state_of_charge);
        return 0;
    }

    return -ENOTSUP;
}

static int monitor_layer_listener(const zmk_event_t *eh) {
    const struct zmk_layer_state_changed *ev =
        as_zmk_layer_state_changed(eh);
    if (ev) {
        uint8_t highest = zmk_keymap_highest_layer_active();
        printk("MONITOR:LAY:%u:%u:%u\n", ev->layer, ev->state, highest);
        return 0;
    }
    return -ENOTSUP;
}

ZMK_LISTENER(monitor_battery, monitor_battery_listener);
ZMK_LISTENER(monitor_layer, monitor_layer_listener);

ZMK_SUBSCRIPTION(monitor_battery, zmk_peripheral_battery_state_changed);
ZMK_SUBSCRIPTION(monitor_battery, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(monitor_layer, zmk_layer_state_changed);
