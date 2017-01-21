#include "pebble.h"

#define REPEAT_INTERVAL_MS 50

static Window *s_main_window;

static ActionBarLayer *s_action_bar;
static TextLayer *s_header_layer, *s_body_layer, *s_label_layer;
static GBitmap *s_icon_minus;

char buffer[32];

static void send_request(void) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  if (!iter) {
    // Error creating outbound message
    return;
  }
  dict_write_end(iter);
  app_message_outbox_send();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
   APP_LOG(APP_LOG_LEVEL_DEBUG, "sending request to JS");
  //TODO call js
  send_request();
  
}

static void click_config_provider(void *context) {
  window_single_repeating_click_subscribe(BUTTON_ID_SELECT, REPEAT_INTERVAL_MS, select_click_handler);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  s_action_bar = action_bar_layer_create();
  action_bar_layer_add_to_window(s_action_bar, window);
  action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);

  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_icon_minus);

  int width = layer_get_frame(window_layer).size.w - ACTION_BAR_WIDTH - 3;

  s_header_layer = text_layer_create(GRect(4, PBL_IF_RECT_ELSE(0, 30), width, 60));
  text_layer_set_font(s_header_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_background_color(s_header_layer, GColorClear);
  text_layer_set_text(s_header_layer, "Garage remote");
  text_layer_set_text_alignment(s_header_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
  layer_add_child(window_layer, text_layer_get_layer(s_header_layer));

}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_header_layer);
  text_layer_destroy(s_body_layer);
  text_layer_destroy(s_label_layer);

  action_bar_layer_destroy(s_action_bar);
}

static void init() {
  s_icon_minus = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ACTION_ICON_MINUS);
 
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
 
  window_stack_push(s_main_window, true);
  app_message_open(128, 128);
}

static void deinit() {
  window_destroy(s_main_window);
  gbitmap_destroy(s_icon_minus);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
