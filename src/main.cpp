#include <Arduino.h>
#include <FastLED.h>
#include <avr/sleep.h>
#include <OneButton.h>

#define BUTTON_PIN 3
#define POWER_PIN 2
#define LED_PIN 1
#define NUM_LEDS 5
#define STICK_PIN 0

enum pattern { NONE, FIRE, RAINBOW, FADE_RED, FADE_BLUE, FADE_GREEN, FADE_ORANGE, FADE_WHITE };


static const uint8_t redPx = 255;
static const uint8_t grnHigh = 155;
static const uint8_t bluePx = 20;
static const uint8_t min_flicker = 40;
static const uint8_t max_flicker = 80;

OneButton button(BUTTON_PIN, true);


void setup();
void loop();
void longPressStart();
void sleep();
void click();
void sleep2();
void doubleclick();
void StickComplete();
// Information about the LED strip itself

#define CHIPSET     WS2812B
#define COLOR_ORDER GRB



class Lights
{
    public:
        CRGB leds[NUM_LEDS];
        //CRGB leds[5];
         //fire2config Fire2Config;
        // Member Variables:  
        pattern  ActivePattern;  // which pattern is running
        //direction Direction;     // direction to run the pattern
        
        unsigned long Interval;   // milliseconds between updates
        uint8_t High_Interval;
        unsigned long lastUpdate; // last update of position
        
        CRGB Color1, Color2;  // What colors are in use
        uint8_t TotalSteps;  // total number of steps in the pattern
        uint16_t Index;  // current step within the pattern
        //uint8_t max_flicker = 80;
        uint8_t Flicker = 60;
        uint8_t target_flicker;
        //uint8_t min_flicker = 40;
        CRGB fire_color   = CRGB( 255,  85,  10);
        
        bool _updatingPaused = false;
        
        uint8_t led_state = LOW;
        void (*OnComplete)();  // Callback on completion of pattern


    Lights( void (*callback)())
    {
        OnComplete = callback;
    }
    
    void Initialize()
    {
        FastLED.addLeds<CHIPSET, STICK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
        //FastLED.setMaxPowerInVoltsAndMilliamps(4.2, 800);
        FastLED.setBrightness( 255 );
    }
     // Update the pattern
    void Update()
    {

        uint8_t _interval = Interval;
        if(ActivePattern == FIRE){
            //_interval = random(Interval, High_Interval);
        // } else if(ActivePattern ==  FIRE2){
        //     _interval = Fire2Config.fDelay;
        }

        if((millis() - lastUpdate) > _interval) // time to update
        {
            lastUpdate = millis();
            
            switch(ActivePattern)
            {
                case FIRE:
                    FireUpdate();
                    break;
                // case FIRE2:
                //     Fire2Update();
                //     break;
                case RAINBOW:
                    RainbowCycleUpdate();
                    break;
                case FADE_RED:
                case FADE_GREEN:
                case FADE_BLUE:
                case FADE_WHITE:
                case FADE_ORANGE:
                    if(_updatingPaused == false) {
                        FadeUpdate();
                    }
                    break;
              
                default:
                    break;
            }
        }
    }
  
    void PauseUpdate() 
    {
        _updatingPaused = true;
    }
    // Increment the Index and reset at the end
    void Increment()
    {
        
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
       
    }
    
    // Reverse pattern direction
    
    // Initialize for a RainbowCycle
    void RainbowCycle(uint8_t interval) 
    {
        ActivePattern = RAINBOW;
        Interval = interval;
        TotalSteps = 255;
        Index = 0;
        
    }
    
    // Update the Rainbow Cycle Pattern
    void RainbowCycleUpdate()
    {
        for(uint8_t i=0; i< NUM_LEDS; i++)
        {
            CRGB newcolor = Wheel(Index & 255);
            //setCorrectedPixelColor(i, newcolor);
            leds[i] = newcolor;
        }
        show();
        Increment();
    }

    void show()
    {
        FastLED.show();
    }
    
    uint8_t getBrightness()
    {
        return FastLED.getBrightness();
    }

    void setBrightness(uint8_t brightness) 
    {
        FastLED.setBrightness(brightness);
    }

    void Fire(uint8_t low_interval, uint8_t high_interval) 
    {
        FastLED.setBrightness(255);
        ActivePattern = pattern::FIRE;
        Interval = low_interval;
        High_Interval = high_interval;
        Color1 = fire_color;
        //Color2 = fire_color;
        Index = 0;
    }

    unsigned long lastmillis;
    void FireUpdate() 
    {
       
        uint8_t grnPx = grnHigh-5;
        uint8_t halfGrn = grnHigh - ((grnHigh - grnPx) / 2);

        leds[0] = CRGB(redPx-55, grnPx-40, bluePx-16);
        leds[1] = CRGB(redPx-22, grnPx-27, bluePx-5);
        leds[2] = CRGB(redPx, grnPx, bluePx);
        leds[3] = CRGB(redPx, halfGrn, bluePx);
        leds[4] = CRGB(redPx, grnHigh, bluePx);

        unsigned long thismillis = millis();
        
        if(thismillis - lastmillis > 500) {

            uint8_t seed = random(0, 100);

            //30% calm
            //55% mild - 85
            //15% windy

            if(Flicker == target_flicker && (thismillis - lastmillis) > 2000) {
                if(seed < 60) 
                {
                    target_flicker = min_flicker;
                } 
                else if(seed < 85)
                {
                    target_flicker = (max_flicker + min_flicker)/2;
                } 
                else  
                {
                    target_flicker = max_flicker;
                }
            }

            if(Flicker < target_flicker) {
                Flicker+=5;
            } else if(Flicker<target_flicker){
                Flicker-=5;
            }

            lastmillis = thismillis;
        }

        


        for (uint8_t i = NUM_LEDS - 1; i > 0; i--)
        {
            uint8_t r = random(Flicker);
            leds[i] -= CRGB(r,r/2,r/2);
            //SubtractColor(i, CRGB(r,r/2,r/2));
        }
        show();
        //Increment();
    }

    void FadeRed()
    {
        ActivePattern = FADE_RED;
        Fade(255,0,0, 100, 5); 
    }
    void FadeGreen()
    {
        ActivePattern = FADE_GREEN;
        Fade(0,255,0, 100, 5); 
    }
    void FadeBlue()
    {
        ActivePattern = FADE_BLUE;
        Fade( 0,0,255, 100, 5); 

    }
    void FadeOrange() 
    {
        ActivePattern = FADE_ORANGE;
        Fade(255, 128, 0, 100, 5);
    }
    void FadeWhite()
    {
        ActivePattern = FADE_WHITE;
        Fade(255,255,255, 100, 5); 

    }


    // Initialize for a Fade
    void Fade(uint8_t red, uint8_t green, uint8_t blue, uint16_t steps, uint8_t interval)
    {

        //ActivePattern = FADE;
        Interval = interval;
        TotalSteps = steps;
        Color1 = leds[1];
        Color2 = CRGB(red, green, blue);
        Index = 0;
        
    }
    
    
    // Update the Fade Pattern
    void FadeUpdate()
    {
        // Calculate linear interpolation between Color1 and Color2
        // Optimise order of operations to minimize truncation error
        uint8_t red = ((Color1.red * (TotalSteps - Index)) + (Color2.red * Index)) / TotalSteps;
        uint8_t green = ((Color1.green * (TotalSteps - Index)) + (Color2.green * Index)) / TotalSteps;
        uint8_t blue = ((Color1.blue * (TotalSteps - Index)) + (Color2.blue * Index)) / TotalSteps;
      
        
        ColorSet(CRGB(red, green, blue));
        Increment();
    }
   
   

    void FadeOff(uint8_t interval)
    {
        bool stillLit = true;
        while(stillLit){
            stillLit = false;
            for(uint8_t i = 0;i<NUM_LEDS;i++)
            {
                leds[i].fadeToBlackBy( 64 );
                if(leds[i])
                {
                  stillLit = true;  
                } 
               
            }
            show();
            delay(interval);
        }


    }

    void TurnOff()
    {
        FastLED.clear(true);
        //show();
    }
    void ColorSet(CRGB color, boolean should_show)
    {
        for (uint16_t i = 0; i < NUM_LEDS; i++)
        {
            //setCorrectedPixelColor(i, color);
            leds[i] = color;
        }
        if(should_show) {
            show();
        }
    }

    // Set all pixels to a color (synchronously)
    void ColorSet(CRGB color)
    {
        ColorSet(color, true);
    }
 

    
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    CRGB Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }

    void Resume() 
    {
        _updatingPaused = false;
        switch(ActivePattern)
        {
            case FIRE:
                Fire(50, 150);
                break;
            case RAINBOW:
                RainbowCycle(250);
                break;
            case FADE_RED:
                FadeRed();
                break;
            case FADE_GREEN:
                FadeGreen();
                break;
            case FADE_BLUE:
                FadeBlue();
                break;
            case FADE_ORANGE:
                FadeOrange();
                break;
            case FADE_WHITE:
                FadeWhite();
                break;
            case NONE:
                break;
        }
    }
    void NextPattern()
    {
        _updatingPaused = false;
        switch(ActivePattern)
        {
            case FIRE:
                FadeRed();
                break;   
            case FADE_RED:
                FadeGreen();
                break;
            case FADE_GREEN:
                FadeBlue();
                break;
            case FADE_BLUE:
                FadeOrange();
                break;
            case FADE_ORANGE:
                FadeWhite();
                break;
            case FADE_WHITE:
                RainbowCycle(250);
                break;
            case NONE:
            case RAINBOW:
                Fire(50, 150);
                //Fire2();
                break;
        }


    }
};


Lights Stick(&StickComplete);

unsigned long start_time;
void setup() 
{
   
    
    pinMode(POWER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    digitalWrite(POWER_PIN, HIGH);
    
    button.setClickTicks(650);
    button.setPressTicks(2500);

    Stick.Initialize();
    
    //Stick.setBrightness(255);
    //Stick.begin();
    
    button.attachLongPressStart(longPressStart);
    button.attachDoubleClick(doubleclick);
    button.attachClick(click);
    start_time = millis();
}

ISR(PCINT0_vect) 
{
    //button.tick();
}

void loop() 
{
    // put your main code here, to run repeatedly:
    Stick.Update();
    button.tick();
    
    if(Stick.ActivePattern == NONE) {
        unsigned long now = millis();

        if(start_time + 5000 < now) {
            sleep();
        }
    }
}

void click() 
{
    Stick.NextPattern();
    
}

void doubleclick() 
{
        byte brightness = Stick.getBrightness();
    
        if(brightness == 255) {
            brightness = 150;
        } else if (brightness == 150) {
            brightness = 50;
        } else if(brightness == 50) {
            brightness = 255;
        }

        Stick.setBrightness(brightness);
        Stick.show();
    // if(Stick.ActivePattern == FIRE)
    // {
    //     Stick.setFireFlicker();
    // }
    // else
    // {
        // byte brightness = Stick.getBrightness();
    
        // if(brightness == 255) {
        //     brightness = 150;
        // } else if (brightness == 150) {
        //     brightness = 50;
        // } else if(brightness == 50) {
        //     brightness = 255;
        // }

        // Stick.setBrightness(brightness);
        // Stick.show();
    //}
    
}

void longPressStart() 
{
    Stick.FadeOff(50);
    delay(1000);
    sleep();
}

void sleep() 
{
    Stick.TurnOff();
    //Stick.show();
    digitalWrite(POWER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    //digitalWrite(BUTTON_PIN, LOW);
    
    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts

    // for(uint8_t i = 0;i<10;i++){
    //     veryshortLEDFlash();
    // }

    //displayVcc();

    
    sleep_cpu();                            // sleep
    digitalWrite(LED_PIN, LOW);
    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts


    digitalWrite(POWER_PIN, HIGH);
    start_time = millis();
    Stick.Resume();
    //digitalWrite(BUTTON_PIN, HIGH);
    //sei();                                  // Enable interrupts
}

void StickComplete()
{

    Stick.PauseUpdate();
    // Random color change for next scan
    //Stick.Color1 = Stick.Wheel(random(255));
}