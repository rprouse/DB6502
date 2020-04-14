#include "blink.h"
#include "acia.h"
#include "keyboard.h"
#include "modem.h"
#include "lcd.h"
#include "utils.h"

#define CHAR_MOUTH_OPEN 0x00
#define CHAR_MOUTH_CLOSED 0x01

static char open_mouth_map[] = {0b00001110, 0b00011111, 0b00010101, 0b00011111, 0b00010001, 0b00001110, 0b00000000, 0b00000000};
static char closed_mouth_map[] = {0b00001110, 0b00011111, 0b00010101, 0b00011111, 0b000111111, 0b00000000, 0b00000000, 0b00000000};

extern void __fastcall__ tty_write(const char* string);

static char counter = 0;
static const char blahmsg[] = "Blah blah";

void main(void) {
  unsigned char c;
  blink_init();
  blink_led(BLINK_LED_ON);
  delay_ms(250);
  blink_led(BLINK_LED_OFF);

  acia_write_string(blahmsg);

  while (!acia_is_data_available()) {}

  strobe_led();

  c = acia_read_byte();

  acia_write_byte(c);
  acia_write_byte(0x0a);
  acia_write_byte(0x0d);

  if (keyboard_is_connected() == KEYBOARD_CONNECTED) {
    acia_write_string("Keyboard connected");
  } else {
    acia_write_string("Keyboard not connected");
  }

  acia_write_byte(0x0a);
  acia_write_byte(0x0d);

  while (!keyboard_is_data_available()) {}

  strobe_led();

  c = keyboard_read_char();

  acia_write_byte(c);
  acia_write_byte(0x0a);
  acia_write_byte(0x0d);

  lcd_clear();
  lcd_print("Testing LCD...");
  delay_ms(200);
  lcd_backspace();
  delay_ms(200);
  lcd_backspace();
  delay_ms(200);
  lcd_backspace();
  delay_ms(200);
  lcd_print(" - it works");
  lcd_print_char('!');
  lcd_display_mode(LCD_DM_DISPLAY_ON | LCD_DM_CURSOR_BLINK);
  delay_sec(2);
  lcd_scroll_down();
  delay_sec(2);
  lcd_scroll_down();
  delay_sec(2);
  lcd_scroll_up();
  lcd_newline();
  lcd_display_mode(LCD_DM_DISPLAY_ON | LCD_DM_CURSOR_OFF);
  lcd_print("Works as a charm!");
  
  // lcd_clear();
  // lcd_define_char(CHAR_MOUTH_OPEN, open_mouth_map);
  // lcd_define_char(CHAR_MOUTH_CLOSED, closed_mouth_map);
  // lcd_set_position(0, 5, 1);
  // tty_write(blahmsg);
  // while (counter<10) {
  //   lcd_set_position(0, 3, 1);
  //   lcd_print_char(CHAR_MOUTH_OPEN);
  //   delay_ms(200);
  //   lcd_set_position(0, 3, 1);
  //   lcd_print_char(CHAR_MOUTH_CLOSED);
  //   delay_ms(150);
  //   ++counter;
  // }
  // lcd_clear();
}

        