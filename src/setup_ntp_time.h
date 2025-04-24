#pragma once
#include "Arduino.h"
#include "esp_sntp.h"
// ntp time realated functions

void setup_ntp_time(){
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Europe/Ljubljana
    esp_sntp_setservername(0, "nether.mojvegovc.si");
    sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
    // sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_init();
}