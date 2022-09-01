// DimmerModes.cpp

#include "LEDController.h"

void dimmerRightToLeft( ) {
  for(CRGB & pixel : aledsSet(s[0][0], s[0][1])) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
  for(CRGB & pixel : aledsSet(s[1][0], s[1][1])) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
  for(CRGB & pixel : aledsSet(s[2][0], s[2][1])) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
  for(CRGB & pixel : aledsSet(s[3][0], s[3][1])) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
  for(CRGB & pixel : aledsSet(s[4][0], s[4][1])) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
  for(CRGB & pixel : aledsSet(s[5][0], s[5][1])) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }

        // Primary Method
//      for(CRGB & pixel : aledsSet(s[0][0], s[0][1])) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
//      for(CRGB & pixel : aledsSet(s[1][0], s[1][1])) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
//      for(CRGB & pixel : aledsSet(s[2][0], s[2][1])) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
//      for(CRGB & pixel : aledsSet(s[3][0], s[3][1])) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
//      for(CRGB & pixel : aledsSet(s[4][0], s[4][1])) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
//      for(CRGB & pixel : aledsSet(s[5][0], s[5][1])) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }

        // Alternate Method
//      aledsSet(s[0][0], s[0][1]) = CRGB(scale8(dmx.r1, dmx.d1), scale8(dmx.g1, dmx.d1), scale8(dmx.b1, dmx.d1));
//      aledsSet(s[1][0], s[1][1]) = CRGB(scale8(dmx.r2, dmx.d2), scale8(dmx.g2, dmx.d2), scale8(dmx.b2, dmx.d2));
//      aledsSet(s[2][0], s[2][1]) = CRGB(scale8(dmx.r3, dmx.d3), scale8(dmx.g3, dmx.d3), scale8(dmx.b3, dmx.d3));
//      aledsSet(s[3][0], s[3][1]) = CRGB(scale8(dmx.r4, dmx.d4), scale8(dmx.g4, dmx.d4), scale8(dmx.b4, dmx.d4));
//      aledsSet(s[4][0], s[4][1]) = CRGB(scale8(dmx.r5, dmx.d5), scale8(dmx.g5, dmx.d5), scale8(dmx.b5, dmx.d5));
//      aledsSet(s[5][0], s[5][1]) = CRGB(scale8(dmx.r6, dmx.d6), scale8(dmx.g6, dmx.d6), scale8(dmx.b6, dmx.d6));
}

void dimmerUpToDown( ) {
//        CRGB c = CRGB(scale8(dmx.r1, dmx.d1), scale8(dmx.g1, dmx.d1), scale8(dmx.b1, dmx.d1));
//      aledsSet(s[0][0], s[0][0] + 14) = c;
//      aledsSet(s[1][0], s[1][0] + 14) = c;
//      aledsSet(s[2][0], s[2][0] + 14) = c;
//      aledsSet(s[3][1], s[3][1] - 14) = c;
//      aledsSet(s[4][1], s[4][1] - 14) = c;
//      aledsSet(s[5][1], s[5][1] - 14) = c;    
//      c = CRGB(scale8(dmx.r2, dmx.d2), scale8(dmx.g2, dmx.d2), scale8(dmx.b2, dmx.d2));
//      aledsSet(s[0][0] + 14, s[0][0] + 28) = c;
//      aledsSet(s[1][0] + 14, s[1][0] + 28) = c;
//      aledsSet(s[2][0] + 14, s[2][0] + 28) = c;
//      aledsSet(s[3][1] - 14, s[3][1] - 28) = c;
//      aledsSet(s[4][1] - 14, s[4][1] - 28) = c;
//      aledsSet(s[5][1] - 14, s[5][1] - 28) = c;
//      c = CRGB(scale8(dmx.r3, dmx.d3), scale8(dmx.g3, dmx.d3), scale8(dmx.b3, dmx.d3));
//      aledsSet(s[0][0] + 28, s[0][0] + 42) = c;
//      aledsSet(s[1][0] + 28, s[1][0] + 42) = c;
//      aledsSet(s[2][0] + 28, s[2][0] + 42) = c;
//      aledsSet(s[3][1] - 28, s[3][1] - 42) = c;
//      aledsSet(s[4][1] - 28, s[4][1] - 42) = c;
//      aledsSet(s[5][1] - 28, s[5][1] - 42) = c;
//      c = CRGB(scale8(dmx.r4, dmx.d4), scale8(dmx.g4, dmx.d4), scale8(dmx.b4, dmx.d4));
//      aledsSet(s[0][0] + 42, s[0][0] + 56) = c;
//      aledsSet(s[1][0] + 42, s[1][0] + 56) = c;
//      aledsSet(s[2][0] + 42, s[2][0] + 56) = c;
//      aledsSet(s[3][1] - 42, s[3][1] - 56) = c;
//      aledsSet(s[4][1] - 42, s[4][1] - 56) = c;
//      aledsSet(s[5][1] - 42, s[5][1] - 56) = c;
//      c = CRGB(scale8(dmx.r5, dmx.d5), scale8(dmx.g5, dmx.d5), scale8(dmx.b5, dmx.d5));
//      aledsSet(s[0][0] + 56, s[0][0] + 70) = c;
//      aledsSet(s[1][0] + 56, s[1][0] + 70) = c;
//      aledsSet(s[2][0] + 56, s[2][0] + 70) = c;
//      aledsSet(s[3][1] - 56, s[3][1] - 70) = c;
//      aledsSet(s[4][1] - 56, s[4][1] - 70) = c;
//      aledsSet(s[5][1] - 56, s[5][1] - 70) = c;
//      c = CRGB(scale8(dmx.r6, dmx.d6), scale8(dmx.g6, dmx.d6), scale8(dmx.b6, dmx.d6));
//      aledsSet(s[0][0] + 70, s[0][1]) = c;
//      aledsSet(s[1][0] + 70, s[1][1]) = c;
//      aledsSet(s[2][0] + 70, s[2][1]) = c;
//      aledsSet(s[3][1] - 70, s[3][0]) = c;
//      aledsSet(s[4][1] - 70, s[4][0]) = c;
//      aledsSet(s[5][1] - 70, s[5][0]) = c;

  for(CRGB & pixel : aledsSet(s[0][0], s[0][0] + 14)) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
  for(CRGB & pixel : aledsSet(s[1][0], s[1][0] + 14)) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
  for(CRGB & pixel : aledsSet(s[2][0], s[2][0] + 14)) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
  for(CRGB & pixel : aledsSet(s[3][1], s[3][1] - 14)) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
  for(CRGB & pixel : aledsSet(s[4][1], s[4][1] - 14)) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }
  for(CRGB & pixel : aledsSet(s[5][1], s[5][1] - 14)) { pixel = CRGB(scale8(pixel.r, dmx.d1), scale8(pixel.g, dmx.d1), scale8(pixel.b, dmx.d1)); }

  for(CRGB & pixel : aledsSet(s[0][0] + 15, s[0][0] + 28)) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
  for(CRGB & pixel : aledsSet(s[1][0] + 15, s[1][0] + 28)) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
  for(CRGB & pixel : aledsSet(s[2][0] + 15, s[2][0] + 28)) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
  for(CRGB & pixel : aledsSet(s[3][1] - 15, s[3][1] - 28)) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
  for(CRGB & pixel : aledsSet(s[4][1] - 15, s[4][1] - 28)) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }
  for(CRGB & pixel : aledsSet(s[5][1] - 15, s[5][1] - 28)) { pixel = CRGB(scale8(pixel.r, dmx.d2), scale8(pixel.g, dmx.d2), scale8(pixel.b, dmx.d2)); }

  for(CRGB & pixel : aledsSet(s[0][0] + 29, s[0][0] + 42)) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
  for(CRGB & pixel : aledsSet(s[1][0] + 29, s[1][0] + 42)) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
  for(CRGB & pixel : aledsSet(s[2][0] + 29, s[2][0] + 42)) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
  for(CRGB & pixel : aledsSet(s[3][1] - 29, s[3][1] - 42)) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
  for(CRGB & pixel : aledsSet(s[4][1] - 29, s[4][1] - 42)) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }
  for(CRGB & pixel : aledsSet(s[5][1] - 29, s[5][1] - 42)) { pixel = CRGB(scale8(pixel.r, dmx.d3), scale8(pixel.g, dmx.d3), scale8(pixel.b, dmx.d3)); }

  for(CRGB & pixel : aledsSet(s[0][0] + 43, s[0][0] + 56)) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
  for(CRGB & pixel : aledsSet(s[1][0] + 43, s[1][0] + 56)) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
  for(CRGB & pixel : aledsSet(s[2][0] + 43, s[2][0] + 56)) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
  for(CRGB & pixel : aledsSet(s[3][1] - 43, s[3][1] - 56)) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
  for(CRGB & pixel : aledsSet(s[4][1] - 43, s[4][1] - 56)) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }
  for(CRGB & pixel : aledsSet(s[5][1] - 43, s[5][1] - 56)) { pixel = CRGB(scale8(pixel.r, dmx.d4), scale8(pixel.g, dmx.d4), scale8(pixel.b, dmx.d4)); }

  for(CRGB & pixel : aledsSet(s[0][0] + 57, s[0][0] + 70)) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
  for(CRGB & pixel : aledsSet(s[1][0] + 57, s[1][0] + 70)) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
  for(CRGB & pixel : aledsSet(s[2][0] + 57, s[2][0] + 70)) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
  for(CRGB & pixel : aledsSet(s[3][1] - 57, s[3][1] - 70)) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
  for(CRGB & pixel : aledsSet(s[4][1] - 57, s[4][1] - 70)) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }
  for(CRGB & pixel : aledsSet(s[5][1] - 57, s[5][1] - 70)) { pixel = CRGB(scale8(pixel.r, dmx.d5), scale8(pixel.g, dmx.d5), scale8(pixel.b, dmx.d5)); }

  for(CRGB & pixel : aledsSet(s[0][0] + 71, s[0][1])) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }
  for(CRGB & pixel : aledsSet(s[1][0] + 71, s[1][1])) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }
  for(CRGB & pixel : aledsSet(s[2][0] + 71, s[2][1])) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }
  for(CRGB & pixel : aledsSet(s[3][1] - 71, s[3][0] + 1)) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }
  for(CRGB & pixel : aledsSet(s[4][1] - 71, s[4][0] + 1)) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }
  for(CRGB & pixel : aledsSet(s[5][1] - 71, s[5][0] + 1)) { pixel = CRGB(scale8(pixel.r, dmx.d6), scale8(pixel.g, dmx.d6), scale8(pixel.b, dmx.d6)); }
}
