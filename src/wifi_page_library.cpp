#include "wifi_page_library.h"

const char **wifi_button_texts = nullptr;
int WIFI_BUTTON_COUNT = 0;
String *wifi_networks = nullptr;
NetworkInfo targetNetwork;
int wifi_selected_index = -1;