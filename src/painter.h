/**
 * Arduheater - Telescope heat controller
 * Copyright (C) 2016-2017 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PAINTER_H__
#define __PAINTER_H__

#include "common.h"

class Painter
{
private:
  Painter() {;}

public:
  static U8GLIB* instance() {
    static UI_LCD_TYPE s_painter(UI_LCD_OPTIONS);
    static bool s_needs_init = true;

    if (s_needs_init) {
      s_painter.begin();

      #ifdef UI_LCD_CONTRAST
      s_painter.setContrast(UI_LCD_CONTRAST);
      #endif

      #ifdef UI_LCD_ROTATED
      s_painter.setRot180();
      #endif

      s_needs_init = false;
    }

    return &s_painter;
  }

  static inline void normalfont() {
    instance()->setFont(UI_LCD_FONT);
    instance()->setFontPosTop();
    instance()->setFontRefHeightExtendedText();
  }

  static inline void bigfont() {
    instance()->setFont(UI_LCD_FONT_BIG);
    instance()->setFontPosTop();
    instance()->setFontRefHeightExtendedText();
  }
};

#endif
