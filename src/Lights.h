// #include <Arduino.h>
// #include <FastLED.h>

// enum pattern { NONE, FIRE, RAINBOW, FADE_RED, FADE_BLUE, FADE_GREEN, FADE_ORANGE, FADE_WHITE };

// static const uint8_t redPx = 255;
// static const uint8_t grnHigh = 215;
// static const uint8_t bluePx = 100;
// static const uint8_t min_flicker = 40;
// static const uint8_t max_flicker = 80;


// class Lights
// {
//     public:
//         CRGB leds[5];
//          //fire2config Fire2Config;
//         // Member Variables:  
//         pattern  ActivePattern;  // which pattern is running
//         //direction Direction;     // direction to run the pattern
        
//         unsigned long Interval;   // milliseconds between updates
//         uint8_t High_Interval;
//         unsigned long lastUpdate; // last update of position
        
//         uint32_t Color1, Color2;  // What colors are in use
//         uint8_t TotalSteps;  // total number of steps in the pattern
//         uint16_t Index;  // current step within the pattern
//         //uint8_t max_flicker = 80;
//         uint8_t Flicker = 60;
//         uint8_t target_flicker;
//         //uint8_t min_flicker = 40;
//         CRGB fire_color   = CRGB( 255,  85,  10);
//         //uint32_t off_color    = Color (  0,  0,  0);

//     // Lights()
//     // {
//     //     FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
//     //     //FastLED.setBrightness( BRIGHTNESS );
//     // }
    
// };
