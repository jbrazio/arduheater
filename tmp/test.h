void draw_test_page(UI_LCD_TYPE* painter, const uint8_t& t) {
  ui::single::instance().font();

  painter->setColorIndex(1);

  painter->drawStr(0, 1, "Hello");
  painter->drawHLine(0, 1+14, 40);
  painter->setScale2x2();            // Scale up all draw procedures
  painter->drawStr(0, 12, "Hello");      // actual display position is (0,24)
  painter->drawHLine(0, 12+14, 40);      // All other procedures are also affected
  painter->undoScale();          // IMPORTANT: Switch back to normal mode
}
