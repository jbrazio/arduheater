void draw_page_home(UI_LCD_TYPE* painter) {
  int16_t val;
  uint8_t x, y;
  char buffer[4];

  ui::single::instance().font();
  const uint8_t border = 2;
  const uint8_t fh = ui::single::instance().font_height();

  painter->setColorIndex(1);
  painter->drawBox(0, 0, UI_LCD_SIZE_W, fh + 2 * border);

  // Draw the temperature indicator
  y = fh * 0.75;
  painter->setScale2x2();
  itoa(roundf(10.0f), buffer, 10);
  x = (UI_LCD_SIZE_W /8.0) - ((painter->getStrWidth(buffer)
    + painter->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_unit_C)) /2.0);
  x += painter->drawStr(x, y, buffer);
  x += painter->drawStrP(x, y, (u8g_pgm_uint8_t*) string_lcd_unit_C);

  // Draw the humidity indicator
  itoa(roundf(10.0f), buffer, 10);
  x = (UI_LCD_SIZE_W *3 /8.0) - ((painter->getStrWidth(buffer)
    + painter->getStrWidthP((u8g_pgm_uint8_t*) string_percent)) /2.0);
  x += painter->drawStr(x, y, buffer);
  x += painter->drawStrP(x, y, (u8g_pgm_uint8_t*) string_percent);
  painter->undoScale();

  painter->setColorIndex(0);
  painter->drawBox(UI_LCD_SIZE_W /2.0 - border /2.0, border, border, fh);

  painter->setColorIndex(0);
  x = painter->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_ambient);
  painter->drawStrP((UI_LCD_SIZE_W /4.0) - (x /2.0), 2,
    (const u8g_pgm_uint8_t*) string_lcd_ambient);

  x = painter->getStrWidthP((u8g_pgm_uint8_t*) string_lcd_humidity);
  painter->drawStrP((UI_LCD_SIZE_W *3 /4.0) - (x /2.0), 2,
    (const u8g_pgm_uint8_t*) string_lcd_humidity);


  //painter->drawBox(2, fh + 4, UI_LCD_SIZE_W - 4);


  //serial::println::uint8( (UI_LCD_SIZE_W - (x/2)) / ((float) 4/3) );

/*
  x = 0;
  painter->setScale2x2();
  itoa(roundf(-10.0f), buffer, 10);
  x += painter->drawStr(x, 20, buffer);
  x += painter->drawStrP(x, 20, (const u8g_pgm_uint8_t*) string_lcd_unit_C);
  painter->undoScale();

  x *= 2;

  painter->drawStrP(x, 40, (const u8g_pgm_uint8_t*) string_lcd_temperature);

  itoa(roundf(30.0f), buffer, 10);
  x += painter->drawStr(x, 52, buffer);
  x += painter->drawStrP(x, 52, (const u8g_pgm_uint8_t*) string_percent);
  x += painter->drawStrP(x, 52, (const u8g_pgm_uint8_t*) string_space);
  x += painter->drawStrP(x, 52, (const u8g_pgm_uint8_t*) string_lcd_humidity);
  */
}
