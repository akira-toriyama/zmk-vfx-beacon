/*
 * Copyright (c) 2026 akira-toriyama
 *
 * SPDX-License-Identifier: MIT
 *
 * The status payload: 9 bytes of BLE manufacturer data, written by
 * status_broadcaster.c on the device that runs the keyboard's keymap and read
 * by status_observer.c on the Prospector Dongle. Every byte has a writer and
 * a reader. Any change to a byte's place or meaning, or to the length, is a
 * new version byte, both ends in one commit: the observer takes only its own
 * version at its own length.
 */

#pragma once

#define BEACON_PAYLOAD_LEN 9

/* The broadcaster advertises the payload this often and refreshes it right
 * after a key press; a refresh goes on air at the next advertising event, so a
 * press adds no payload. The observer's KEYS_DELTA_MAX and the sprite's
 * KEY_GRACE_MS are chosen against it. */
#define BEACON_PAYLOAD_INTERVAL_MS 200

/* [0..3]: company id 0xFFFF (no company's), then the magic "VB". Never
 * prospector-zmk-module's AB CD: its scanner takes any manufacturer data of 26
 * bytes or more under that magic for its own layout and checks no version
 * (src/status_scanner.c there, v2.2.3). */
#define BEACON_PAYLOAD_PREFIX_INIT 0xff, 0xff, 0x56, 0x42
#define BEACON_PAYLOAD_PREFIX_LEN 4

/* [4]: the layout's number. */
#define BEACON_PAYLOAD_OFFSET_VERSION 4
#define BEACON_PAYLOAD_VERSION 2

/* [5], [6]: battery of each half in percent, 0 = no reading: the
 * broadcaster's own level in the byte BEACON_OWN_BATTERY names (by default
 * the left, and none on a split central with more than one peripheral, a
 * dongle), and ZMK's split slots in the others, slot 0 first
 * (status_broadcaster.c). A dongle sends none of its own: left = slot 0,
 * right = slot 1. A slot is the order the central first heard each half in,
 * before pairing, kept in its settings (ZMK's zmk_ble_put_peripheral_addr()).
 * A central half with one peripheral has it in slot 0 whichever half it is. */
#define BEACON_PAYLOAD_OFFSET_LEFT 5
#define BEACON_PAYLOAD_OFFSET_RIGHT 6

/* [7]: keystrokes: key presses, any key, either half's on a split, counted
 * modulo 256. The observer takes the difference between consecutive payloads,
 * so a lost payload loses no press and a payload that replaced one not yet on
 * air carries both. */
#define BEACON_PAYLOAD_OFFSET_KEYSTROKES 7

/* [8]: modifiers held, as the HID modifier byte (bit 0 LCTRL .. bit 7 RGUI):
 * only those confirmed as held, a modifier tapped on a mod-tap thumb never
 * shows (status_broadcaster.c, "Modifiers"). The sprite shows its back
 * animation while this is not 0 (sprite.c). */
#define BEACON_PAYLOAD_OFFSET_MODIFIERS 8

/* Not carried, for want of a reader or a source: the active layer and its
 * name (no screen shows them), and a charging state (ZMK knows none: a
 * peripheral's battery event carries its level alone, and a device knows only
 * whether USB powers it). */
