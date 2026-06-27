#include <pb_encode.h>
#include <zmk/studio/rpc.h>
#include <zmk/battery.h>
#include <zmk/keymap.h>
#include <zmk/split/central.h>

#define CORE_RESPONSE(type, ...) ZMK_RPC_RESPONSE(core, type, __VA_ARGS__)

zmk_studio_Response get_battery(const zmk_studio_Request *req) {
    zmk_core_GetBatteryResponse resp = zmk_core_GetBatteryResponse_init_zero;

    resp.level = zmk_battery_state_of_charge();

    uint8_t batt;
    if (zmk_split_central_get_peripheral_battery_level(0, &batt) == 0) {
        resp.peripheral_0_level = batt;
    }
    if (zmk_split_central_get_peripheral_battery_level(1, &batt) == 0) {
        resp.peripheral_1_level = batt;
    }

    return CORE_RESPONSE(get_battery, resp);
}

zmk_studio_Response get_active_layer(const zmk_studio_Request *req) {
    zmk_keymap_layer_index_t layer = zmk_keymap_highest_layer_active();
    return CORE_RESPONSE(get_active_layer, layer);
}

ZMK_RPC_SUBSYSTEM_HANDLER(core, get_battery, ZMK_STUDIO_RPC_HANDLER_UNSECURED);
ZMK_RPC_SUBSYSTEM_HANDLER(core, get_active_layer, ZMK_STUDIO_RPC_HANDLER_UNSECURED);
