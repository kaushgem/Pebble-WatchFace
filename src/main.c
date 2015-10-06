/*

Simple WatchFace

*/

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;

//Method to update time
static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  static char buffer[] = "00:00";
  
  if(clock_is_24h_style() == true)
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  else
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  
  text_layer_set_text(s_time_layer, buffer);
}

//Time change handler
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

//Window load
static void main_window_load(Window *window) {
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(s_main_window), text_layer_get_layer(s_time_layer));
}

//Window Unload
static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
}

//Program load
static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(s_main_window, true);
  update_time();
}

//Program terminate - deallocation block
static void deinit() {
  window_destroy(s_main_window);
}

//main method
int main(void) {
  init();
  app_event_loop();
  deinit();
}