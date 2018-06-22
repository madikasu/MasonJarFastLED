// #include <Arduino.h>
// #include <FastLED.h>

// enum pattern { NONE, FIRE, RAINBOW, FADE_RED, FADE_BLUE, FADE_GREEN, FADE_ORANGE, FADE_WHITE };

// static const uint8_t redPx = 255;
// static const uint8_t grnHigh = 215;
// static const uint8_t bluePx = 100;
// static const uint8_t min_flicker = 40;
// static const uint8_t max_flicker = 80;
// #define CHIPSET     SK6812
// #define COLOR_ORDER GRB
// #define LED_PIN 1
// #define NUM_LEDS 5
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
        
//         CRGB Color1, Color2;  // What colors are in use
//         uint8_t TotalSteps;  // total number of steps in the pattern
//         uint16_t Index;  // current step within the pattern
//         //uint8_t max_flicker = 80;
//         uint8_t Flicker = 60;
//         uint8_t target_flicker;
//         //uint8_t min_flicker = 40;
//         CRGB fire_color   = CRGB( 255,  85,  10);
//         //uint32_t off_color    = Color (  0,  0,  0);
//         bool _updatingPaused = false;

//         void (*OnComplete)();  // Callback on completion of pattern


//     Lights( void (*callback)())
//     {
//         FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
//         //FastLED.setBrightness( BRIGHTNESS );
//     }
    
//      // Update the pattern
//     void Update()
//     {

//         uint8_t _interval = Interval;
//         if(ActivePattern == FIRE){
//             //_interval = random(Interval, High_Interval);
//         // } else if(ActivePattern ==  FIRE2){
//         //     _interval = Fire2Config.fDelay;
//         }

//         if((millis() - lastUpdate) > _interval) // time to update
//         {
//             lastUpdate = millis();
//             switch(ActivePattern)
//             {
//                 case FIRE:
//                     FireUpdate();
//                     break;
//                 // case FIRE2:
//                 //     Fire2Update();
//                 //     break;
//                 case RAINBOW:
//                     RainbowCycleUpdate();
//                     break;
//                 case FADE_RED:
//                 case FADE_GREEN:
//                 case FADE_BLUE:
//                 case FADE_WHITE:
//                 case FADE_ORANGE:
//                     if(_updatingPaused == false) {
//                         FadeUpdate();
//                     }
//                     break;
              
//                 default:
//                     break;
//             }
//         }
//     }
  
//     void PauseUpdate() {
//         _updatingPaused = true;
//     }
//     // Increment the Index and reset at the end
//     void Increment()
//     {
        
//            Index++;
//            if (Index >= TotalSteps)
//             {
//                 Index = 0;
//                 if (OnComplete != NULL)
//                 {
//                     OnComplete(); // call the comlpetion callback
//                 }
//             }
       
//     }
    
//     // Reverse pattern direction
    
//     // Initialize for a RainbowCycle
//     void RainbowCycle(uint8_t interval) 
//     {
//         ActivePattern = RAINBOW;
//         Interval = interval;
//         TotalSteps = 255;
//         Index = 0;
        
//     }
    
//     // Update the Rainbow Cycle Pattern
//     void RainbowCycleUpdate()
//     {
//         for(uint8_t i=0; i< NUM_LEDS; i++)
//         {
//             uint32_t newcolor = Wheel(Index & 255);
//             //setCorrectedPixelColor(i, newcolor);
//             leds[i] = CRGB(newcolor);
//         }
//         show();
//         Increment();
//     }

//     void show()
//     {
//         FastLED.show();
//     }
 
//     // void setFireFlicker()
//     // {
//     //     if(Flicker == 100) {
//     //         Flicker = 80;
//     //     } else if(Flicker == 80){
//     //         Flicker = 60;

//     //     } else if(Flicker == 60){
//     //         Flicker = 40;
//     //     } else if(Flicker == 40) {
//     //         Flicker = 20;
//     //     } else {
//     //         Flicker = 100;
//     //     }
//     // }

//     void Fire(uint8_t low_interval, uint8_t high_interval) 
//     {
//         FastLED.setBrightness(255);
//         ActivePattern = pattern::FIRE;
//         Interval = low_interval;
//         High_Interval = high_interval;
//         Color1 = fire_color;
//         //Color2 = fire_color;
//         Index = 0;
//     }

//     unsigned long lastmillis;
//     void FireUpdate() 
//     {
       
//         uint8_t grnPx = grnHigh-5;
//         uint8_t halfGrn = grnHigh - ((grnHigh - grnPx) / 2);

//         leds[0] = CRGB(redPx-55, grnPx-40, bluePx-16);
//         leds[1] = CRGB(redPx-22, grnPx-27, bluePx-5);
//         leds[2] = CRGB(redPx, grnPx, bluePx);
//         leds[3] = CRGB(redPx, halfGrn, bluePx);
//         leds[4] = CRGB(redPx, grnHigh, bluePx);

//         unsigned long thismillis = millis();
        
//         if(thismillis - lastmillis > 500) {

//             uint8_t seed = random(0, 100);

//             //30% calm
//             //55% mild - 85
//             //15% windy

//             if(Flicker == target_flicker && (thismillis - lastmillis) > 2000) {
//                 if(seed < 60) 
//                 {
//                     target_flicker = min_flicker;
//                 } 
//                 else if(seed < 85)
//                 {
//                     target_flicker = (max_flicker + min_flicker)/2;
//                 } 
//                 else  
//                 {
//                     target_flicker = max_flicker;
//                 }
//             }

//             if(Flicker < target_flicker) {
//                 Flicker+=5;
//             } else if(Flicker<target_flicker){
//                 Flicker-=5;
//             }

//             lastmillis = thismillis;
//         }

        


//         for(uint8_t i = 0;i<NUM_LEDS;i++){
//             uint8_t r = random(Flicker);
//             SubtractColor(i, CRGB(r,r/2,r/2));
//         }
//         show();
//         //Increment();
//     }

//     void FadeRed()
//     {
//         ActivePattern = FADE_RED;
//         Fade(255,0,0,0, 100, 5); 
//     }
//     void FadeGreen()
//     {
//         ActivePattern = FADE_GREEN;
//         Fade(0,255,0,0, 100, 5); 
//     }
//     void FadeBlue()
//     {
//         ActivePattern = FADE_BLUE;
//         Fade( 0,0,255,0, 100, 5); 

//     }
//     void FadeOrange() 
//     {
//         ActivePattern = FADE_ORANGE;
//         Fade(255, 128, 0, 0, 100, 5);
//     }
//     void FadeWhite()
//     {
//         ActivePattern = FADE_WHITE;
//         Fade(0,0,0, 255, 100, 5); 

//     }


//     // Initialize for a Fade
//     void Fade(uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint16_t steps, uint8_t interval)
//     {

//         //ActivePattern = FADE;
//         Interval = interval;
//         TotalSteps = steps;
//         Color1 = leds[1];
//         Color2 = CRGB(red, green, blue, white);
//         Index = 0;
        
//     }
    
    
//     // Update the Fade Pattern
//     void FadeUpdate()
//     {
//         // Calculate linear interpolation between Color1 and Color2
//         // Optimise order of operations to minimize truncation error
//         uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
//         uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
//         uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;
//         uint8_t white = ((White(Color1) * (TotalSteps - Index)) + (White(Color2) * Index)) / TotalSteps;
        
//         ColorSet(Color(red, green, blue, white));
//         show();
//         Increment();
//     }
   
//     // Calculate 50% dimmed version of a color (used by ScannerUpdate)
//     uint32_t DimColor(uint32_t color)
//     {
//         // Shift R, G and B components one bit to the right
//         uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1, White(color) >> 1);
//         return dimColor;
//     }
 

//     void FadeOff(uint8_t interval)
//     {
//         bool stillLit = true;
//         while(stillLit){
//             stillLit = false;
//             for(uint8_t i = 0;i<NUM_LEDS;i++)
//             {

//                 uint32_t newcolor = DimColor(getPixelColor(i));
//                 setCorrectedPixelColor(i, newcolor);
//                 if(newcolor != 0) {
//                     stillLit = true;
//                 }
//             }
//             show();
//             delay(interval);
//         }


//     }

//     void TurnOff()
//     {
//         clear();
//         show();
//     }
//     void ColorSet(uint32_t color, boolean should_show)
//     {
//         for (uint16_t i = 0; i < NUM_LEDS; i++)
//         {
//             setCorrectedPixelColor(i, color);
//         }
//         if(should_show) {
//             show();
//         }
//     }

//     // Set all pixels to a color (synchronously)
//     void ColorSet(uint32_t color)
//     {
//         ColorSet(color, true);
//     }
 

//     void SubtractColor(uint8_t position, uint32_t color)
//     {
//         uint32_t current_color = getPixelColor(position);
//         uint32_t blended_color = Color(
//             Subtract(Red(current_color), Red(color)),
//             Subtract(Green(current_color), Green(color)),
//             Subtract(Blue(current_color), Blue(color))
           
//         );
//         setCorrectedPixelColor(position, blended_color);

//         // setCorrectedPixelColor(position, 
//         //     Subtract(Red(current_color), Red(color)),
//         //     Subtract(Green(current_color), Green(color)),
//         //     Subtract(Blue(current_color), Blue(color))
//         // );
//     }



//     uint8_t Subtract(uint8_t colorPart1, uint8_t colorPart2)
//     {
//         int16_t value = (int16_t)colorPart1 - (int16_t)colorPart2;
//         if(value<0) value=0;
//         return (uint8_t)value;
//     }    

    
//     uint8_t White(uint32_t color) 
//     {
//         return (color >> 24) & 0xFF;

//     }

//     // Returns the Red component of a 32-bit color
//     uint8_t Red(uint32_t color)
//     {
//         return (color >> 16) & 0xFF;
//     }
 
//     // Returns the Green component of a 32-bit color
//     uint8_t Green(uint32_t color)
//     {
//         return (color >> 8) & 0xFF;
//     }
 
//     // Returns the Blue component of a 32-bit color
//     uint8_t Blue(uint32_t color)
//     {
//         return color & 0xFF;
//     }
    
//     // Input a value 0 to 255 to get a color value.
//     // The colours are a transition r - g - b - back to r.
//     uint32_t Wheel(byte WheelPos)
//     {
//         WheelPos = 255 - WheelPos;
//         if(WheelPos < 85)
//         {
//             return Color(255 - WheelPos * 3, 0, WheelPos * 3);
//         }
//         else if(WheelPos < 170)
//         {
//             WheelPos -= 85;
//             return Color(0, WheelPos * 3, 255 - WheelPos * 3);
//         }
//         else
//         {
//             WheelPos -= 170;
//             return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//         }
//     }

//     void Resume() 
//     {
//         _updatingPaused = false;
//         switch(ActivePattern)
//         {
//             case FIRE:
//                 Fire(50, 150);
//                 break;
//             case RAINBOW:
//                 RainbowCycle(250);
//                 break;
//             case FADE_RED:
//                 FadeRed();
//                 break;
//             case FADE_GREEN:
//                 FadeGreen();
//                 break;
//             case FADE_BLUE:
//                 FadeBlue();
//                 break;
//             case FADE_ORANGE:
//                 FadeOrange();
//                 break;
//             case FADE_WHITE:
//                 FadeWhite();
//                 break;
//             case NONE:
//                 break;
//         }
//     }
//     void NextPattern()
//     {
//         _updatingPaused = false;
//         switch(ActivePattern)
//         {
//             case FIRE:
//                 FadeRed();
//                 break;   
//             case FADE_RED:
//                 FadeGreen();
//                 break;
//             case FADE_GREEN:
//                 FadeBlue();
//                 break;
//             case FADE_BLUE:
//                 FadeOrange();
//                 break;
//             case FADE_ORANGE:
//                 FadeWhite();
//                 break;
//             case FADE_WHITE:
//                 RainbowCycle(250);
//                 break;
//             case NONE:
//             case RAINBOW:
//                 Fire(50, 150);
//                 //Fire2();
//                 break;
//         }


//     }
// };
