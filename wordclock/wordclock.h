#include "esphome.h"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 114
#define DATA_PIN 3

/*
FÜNF = 1,2,3,4
IST = 6,7,8
ES = 10,11
ZEHN = 12,13,14,15
ZWANZIG = 16,17,18,19,21,21,22
VIERTEL = 23,24,25,26,27,28,29
DREI = 30,31,32,33
VOR = 34,35,36
NACH = 41,42,43,44
FÜNF = 45,46,47,48
ELF = 48,49,50
HALB = 52,53,54,55
EINS = 46,57,58,59
ZWEI = 63,64,65,66
VIER = 67,68,69,70
DREI = 75,75,76,77
SECHS = 78,79,80,81,82
ACHT = 84,85,86,87
ZWÖLF = 88,89,90,91,92
SIEBEN = 93,94,95,96,97,98
ZEHN = 99,101,101,102
NEUN = 102,103,104,105
UHR = 107,108,109

The counting here started at 1, but in reality, it starts at 0, so substract 1 to get the correct LED id.
*/

// esphome dependencies:
// needs: esphome time --> id: current_time
int thinking[][2] = {
    {112, 21}, // T: 12
    {109, 24}, // H: 5
    {2, 36}, // I: 9
    {92, 53}, // N: 8
    {10, 119}, // K: 2
    {46, 95}, // I: 9
    {11, 32}, // N: 8
    {110, 49}  // G: 2
};
int leds_time_it_is[] = {10, 9, 7, 6, 5}; // It Is
int leds_minutes[] = {113, 112, 111, 110};


int leds_time_minutes[][15] = {
    {107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // UHR
    { 0,    1,   2,   3,  40,  41,  42,  43,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // FÜNF, NACH
    { 11,  12,  13,  14,  40,  41,  42,  43,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ZEHN, NACH
    { 22,  23,  24,  25,  26,  27,  28,  40,  41,  42,  43,  -1,  -1,  -1,  -1}, // VIERTEL, NACH
    { 15,  16,  17,  18,  19,  20,  21,  40,  41,  42,  43,  -1,  -1,  -1,  -1}, // ZWANZIG, NACH
    { 0,   1,   2,    3,  33,  34,  35,  51,  52,  53,  54,  -1,  -1,  -1,  -1}, // FÜNF, VOR, HALB
    { 51,  52,  53,  54,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // HALB
    { 0,    1,   2,    3,  40,  41,  42,  43,  51,  52,  53,  54,  -1,  -1,  -1}, // FÜNF, NACH, HALB
    { 15,  16,  17,  18,  19,  20,  21,  33,  34,  35,  -1,  -1,  -1,  -1,  -1}, // ZWANZIG, VOR
    { 22,  23,  24,  25,  26,  27,  28,  33,  34,  35,  -1,  -1,  -1,  -1,  -1}, // VIERTEL, VOR
    { 11,  12,  13,  14,  33,  34,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // ZEHN, VOR
    { 0,   1,   2,    3,  33,  34,  35,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}  // FÜNF, VOR
};

int leds_time_hours[][6] = {
    { 87,  88,  89,  90,  91,  -1}, // twelve
    { 45,  56,  57,  58,  -1,  -1}, // one
    { 62,  63,  64,  65,  -1,  -1}, // two
    { 74,  75,  76,  77,  -1,  -1}, // three
    { 66,  67,  68,  69,  -1,  -1}, // four
    { 44,  45,  46,  47,  -1,  -1}, // five
    { 77,  78,  79,  80,  81,  -1}, // six
    { 91,  92,  93,  94,  95,  96}, // seven
    { 84,  85,  86,  87,  -1,  -1}, // eight
    {102, 103, 104, 105,  -1,  -1}, // nine
    { 99, 100, 101, 102,  -1,  -1}, // ten
    { 47,  48,  49,  -1,  -1,  -1}  // eleven
};

int hour = -1;
int minute = -1;
int red = 124;
int green = 124;
int blue = 124;

int brightness = 50; // half brightness
int prevLightReading = 0;

Adafruit_NeoPixel pixels(NUM_LEDS, DATA_PIN, NEO_RGB + NEO_KHZ800);

void setPixelColor( uint16_t i, uint8_t r, uint8_t g, uint8_t b, uint16_t uiBrightness) {
    pixels.setPixelColor(i, pixels.Color((uiBrightness*r/255) , (uiBrightness*g/255), (uiBrightness*b/255)));
}
void show() {
    pixels.show();
}

void startStrip() {
    pixels.begin();
}

void clearStrip() {
    pixels.clear();
}

class Wordclock : public Component, public CustomAPIDevice {
    public:
        void setup() override {
            startStrip();
            clearStrip();
            brightness = 50; // half brightness

            // Start all LED with on and default color and brightness to check if everything is working...
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, red, 0, 0, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, green, 0, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, blue, brightness); show(); delay(10); }
            for(int i = 0; i < NUM_LEDS; i++) { setPixelColor(i, 0, 0, 0, brightness); }
            show();
            randomSeed(analogRead(0));
            register_service(&Wordclock::on_setled, "setled", {"number","red", "blue", "green"});
            Serial.println("WordClock Setup Complete!");
        }
        void on_setled(int number, int red, int blue, int green) {

            if (number < NUM_LEDS || number > 0) {
                ESP_LOGD("setled", "Setting led number %d to color %i %i %i", number, red, green, blue );
                setPixelColor(number, red, green, blue, brightness);
                show();
            }
            else {
                ESP_LOGE("setled", "Not a valid LED Number - out of range");
            }
        }
        void loop() override {
            auto time = id(current_time).now();
            int h = time.hour;
            int m = time.minute;
            bool isChanged = false;
            auto fastledlight2 = id(clockface).current_values;


            

            // Brightness set by the light sensor

            // brightness = (int)(fastledlight2.get_brightness()*255);
            brightness = 255;
            float temp_r;
            float temp_g;
            float temp_b;
            fastledlight2.as_rgb(&temp_r, &temp_g, &temp_b, 2.8);
            



            //convert float 0.0 till 1.0 into int 0 till 255
            int newRed = (int)(temp_r*255);
            int newGreen = (int)(temp_g*255);
            int newBlue = (int)(temp_b*255);
            if (red != newRed || blue != newBlue || green != newGreen) {
                red = newRed;
                blue = newBlue;
                green = newGreen;
                // brightness = newBrightness;
                isChanged = true;
            }



            //check if valid time. Blink red,green,blue until valid time is present
            if (time.is_valid() == false) {
                ESP_LOGE("loop", "Got invalid time from current_time Time: %i:%i", h, m );
                for (int i = 0; i < 8; i++) {
                    int random_number = random(1023);
                    int rRed = random(255);
                    int rGreen = random(255);
                    int rBlue = random(255);
                    if (random_number %2 == 1) {
                        setPixelColor(thinking[i][0], rRed, rGreen, rBlue, brightness); show();
                        delay(250);
                        setPixelColor(thinking[i][0], 0, 0, 0, brightness);   show();
                    } else {
                        setPixelColor(thinking[i][1], rRed, rGreen, rBlue, brightness); show();
                        delay(250);
                        setPixelColor(thinking[i][1], 0, 0, 0, brightness);   show();
                    }
                }
                // setPixelColor(0, 255, 0, 0, brightness); show(); delay(250);
                // setPixelColor(0, 0, 255, 0, brightness); show(); delay(250);
                // setPixelColor(0, 0, 0, 255, brightness); show(); delay(250);
                
                // cycle through the letters in "thinking"

            }
            // happy tit day
            // twenty four seven c ock
            // flashing between c ock and oclock
            // it is c ock
            //
            else {
                // only update once in a Minute
                if(h != hour || m != minute) {
                   //ESP_LOGD("loop", "Using b: %i rgb %i %i %i", brightness, red, green, blue);
                    hour = h;
                    minute = m;
                    if (hour >= 0 && time.is_valid() == true){
                        int tmp_hour = hour;
                        int tmp_minute = (minute - (minute % 5));
                        if(tmp_minute >= 25) { tmp_hour += 1; }
                        tmp_minute = tmp_minute / 5;
                        tmp_hour = tmp_hour % 12;
                        int minutessum = minute % 5;
                        // Reset all LED, but skip LED 110 till 120
                        for(int i = 0; i < NUM_LEDS; i++) {  setPixelColor(i, 0, 0, 0, brightness); }
                        for(int i = 0; i < 5; i++) {            setPixelColor(leds_time_it_is[i], red, green, blue, brightness); }
                        for(int i = 0; i < 15; i++) {           if(leds_time_minutes[tmp_minute][i] >= 0) { setPixelColor(leds_time_minutes[tmp_minute][i], red, green, blue, brightness); } }
                        for(int i = 0; i < 6; i++) {            if(leds_time_hours[tmp_hour][i] >= 0) { setPixelColor(leds_time_hours[tmp_hour][i], red, green, blue, brightness); } }
                        for(int i = 0; i < minutessum; i++) {   setPixelColor(leds_minutes[i], red, green, blue, brightness);}
                        show();
                        ESP_LOGD("loop", "Update Time: %i:%i  Brightness: %i RGB: %i-%i-%i", hour, minute, brightness, red, green, blue);
                        ESP_LOGD("loop", "Using tmp_hour: %i tmp_minute: %i minutessum: %i", tmp_hour, tmp_minute, minutessum);
                    }
                }
            }
        }
};