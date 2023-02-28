/*
 * Copyright (C) 2023 Humberto Martínez Barberá
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
 */
 
#include <SPI.h>
#include <FlashStorage.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <MIDI.h>
#include "Encoder.h"

/*-------------------------------------------
    HARDWARE MIDIURATION
 --------------------------------------------*/

/* This program is intended to be used with:
 
 - Adafruit Feather M0 Express (Cortex M0)
 - Fasizi OLED 128x64 (SSD1306)
 - Adafruit FeatherWing MIDI

 */

 /*---------------------------------------------
    Board and options configuration 
 ----------------------------------------------*/
// Uncomment the line appropiate for you board
#define M0_EXPRESS      1       // Adafruit Feather M0 express

//#define DEBUG           1

/*-------------------------------------------
    INLINE BITMAPS
 --------------------------------------------*/
// Generated online with:
// https://javl.github.io/image2cpp/
// 
// 'ArduScope-intro', 128x64px
const uint8_t PROGMEM init_bmp[] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x63, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x01, 0xb0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x03, 0xb8, 0x0c, 0x0c, 0x1e, 0x63, 0x03, 0xe0, 0x03, 
  0xc0, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x03, 0x18, 0x0c, 0x0c, 0x3f, 0x63, 0x07, 0xf0, 0x03, 
  0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x03, 0x18, 0x0c, 0x0c, 0x71, 0xe3, 0x0e, 0x38, 0x03, 
  0xc0, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x06, 0x0c, 0x0c, 0x0c, 0x60, 0xe3, 0x1c, 0x1c, 0x03, 
  0xc0, 0x77, 0xff, 0xff, 0xff, 0xff, 0x00, 0x06, 0x0c, 0x0c, 0x0c, 0xc0, 0x63, 0x18, 0x0c, 0x03, 
  0xc0, 0xf7, 0x7f, 0xfd, 0xff, 0xff, 0x80, 0x06, 0x0c, 0x0c, 0x0c, 0xc0, 0x63, 0x18, 0x0c, 0x03, 
  0xc0, 0xf7, 0x3f, 0xf8, 0xff, 0xff, 0x80, 0x0f, 0xfe, 0x0c, 0x0c, 0xc0, 0x63, 0x18, 0x0c, 0x03, 
  0xc1, 0xf6, 0x9f, 0xfa, 0xff, 0xff, 0xc0, 0x0f, 0xfe, 0x0c, 0x0c, 0xc0, 0x63, 0x18, 0x0c, 0x03, 
  0xc1, 0xf6, 0xdf, 0xf2, 0xff, 0xff, 0xc0, 0x1c, 0x07, 0x0c, 0x0c, 0xc0, 0x63, 0x18, 0x0c, 0x03, 
  0xc3, 0xf6, 0xdf, 0xf6, 0xff, 0xff, 0xe0, 0x18, 0x03, 0x0c, 0x1c, 0x60, 0xe3, 0x1c, 0x1c, 0x03, 
  0xc3, 0xf6, 0xcf, 0xf6, 0xff, 0xff, 0xe0, 0x18, 0x03, 0x0e, 0x3c, 0x71, 0xe3, 0x0e, 0x38, 0x03, 
  0xc3, 0xf6, 0xef, 0xe7, 0x7c, 0x7f, 0xe0, 0x30, 0x01, 0x87, 0xec, 0x3f, 0x63, 0x07, 0xf0, 0x03, 
  0xc7, 0xf4, 0xef, 0xef, 0x7d, 0x3f, 0xf0, 0x30, 0x01, 0x83, 0xcc, 0x1e, 0x63, 0x03, 0xe0, 0x03, 
  0xc7, 0xf5, 0xef, 0xef, 0x79, 0x9f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc7, 0xf5, 0xef, 0xef, 0x7b, 0xcf, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc7, 0xf5, 0xef, 0xef, 0x7b, 0xef, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc7, 0xf7, 0xf7, 0xcf, 0xbb, 0xf7, 0xf0, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc7, 0xf7, 0xf7, 0xdf, 0xb7, 0xf3, 0xf0, 0x60, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc7, 0xf7, 0xf7, 0x9f, 0xb7, 0xf9, 0xf0, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc7, 0xf7, 0xf7, 0xbf, 0xb7, 0xfc, 0xf0, 0xc0, 0x18, 0x3e, 0x03, 0xe0, 0x67, 0x80, 0xf8, 0x03, 
  0xc3, 0xf7, 0xf3, 0x3f, 0xb7, 0xfe, 0x60, 0xc0, 0x00, 0x7f, 0x07, 0xf0, 0x6f, 0xc1, 0xfc, 0x03, 
  0xc3, 0xf7, 0xfb, 0x7f, 0x97, 0xff, 0xe0, 0x70, 0x00, 0xe3, 0x8e, 0x38, 0x78, 0xe3, 0x8e, 0x03, 
  0xc3, 0xf7, 0xfb, 0x7f, 0xc7, 0xff, 0xe0, 0x7f, 0x81, 0xc1, 0x9c, 0x1c, 0x70, 0x63, 0x07, 0x03, 
  0xc1, 0xf7, 0xfb, 0x7f, 0xef, 0xff, 0xc0, 0x1f, 0xe1, 0x80, 0x18, 0x0c, 0x60, 0x36, 0x03, 0x03, 
  0xc1, 0xf7, 0xfb, 0x7f, 0xff, 0xff, 0xc0, 0x00, 0xf1, 0x80, 0x18, 0x0c, 0x60, 0x37, 0xff, 0x03, 
  0xc0, 0xf7, 0xfb, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x39, 0x80, 0x18, 0x0c, 0x60, 0x37, 0xff, 0x03, 
  0xc0, 0xf7, 0xf9, 0x7f, 0xff, 0xff, 0x81, 0x80, 0x19, 0x80, 0x18, 0x0c, 0x60, 0x36, 0x00, 0x03, 
  0xc0, 0x77, 0xfc, 0xff, 0xff, 0xff, 0x01, 0x80, 0x19, 0x80, 0x18, 0x0c, 0x60, 0x36, 0x00, 0x03, 
  0xc0, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x00, 0xc0, 0x19, 0xc1, 0x9c, 0x1c, 0x70, 0x67, 0x03, 0x03, 
  0xc0, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0xf0, 0x70, 0xe3, 0x8e, 0x38, 0x78, 0xe3, 0x8e, 0x03, 
  0xc0, 0x0f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xe0, 0x7f, 0x07, 0xf0, 0x6f, 0xc1, 0xfc, 0x03, 
  0xc0, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x1f, 0x80, 0x3e, 0x03, 0xe0, 0x67, 0x80, 0xf8, 0x03, 
  0xc0, 0x03, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 
  0xce, 0x71, 0xce, 0x04, 0x40, 0x00, 0x20, 0x00, 0x80, 0x10, 0x40, 0x0a, 0x00, 0x00, 0x00, 0x03, 
  0xd1, 0x8a, 0x31, 0x04, 0x40, 0x00, 0x20, 0x00, 0x80, 0x18, 0xc0, 0x08, 0x00, 0x00, 0x00, 0x03, 
  0xc1, 0x88, 0x21, 0x04, 0x49, 0x7e, 0x38, 0xc7, 0xcc, 0x18, 0xc7, 0x7d, 0x38, 0x6f, 0x80, 0x03, 
  0xc2, 0x88, 0x42, 0x07, 0xc9, 0x49, 0x25, 0x24, 0x92, 0x15, 0x41, 0x49, 0x24, 0x91, 0x00, 0x03, 
  0xc4, 0x88, 0x84, 0x04, 0x49, 0x49, 0x25, 0xe4, 0x92, 0x15, 0x47, 0x49, 0x24, 0xf2, 0x00, 0x03, 
  0xc8, 0x89, 0x08, 0x04, 0x49, 0x49, 0x25, 0x04, 0x92, 0x15, 0x49, 0x49, 0x24, 0x84, 0x00, 0x03, 
  0xdf, 0x73, 0xff, 0x04, 0x47, 0x49, 0x38, 0xe4, 0xcc, 0x12, 0x4f, 0x4d, 0x24, 0x7f, 0x80, 0x03, 
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/*---------------------------------------------
    OLED Initialisation 
 ----------------------------------------------*/
#define OLED_RESET   -1 
#define OLED_ADDR    0x3C     // WARNING: Silkscreeen address is 0x78 (8-bit address). 
                              // The actual used by the library is 0x3C (7-bit address)

Adafruit_SSD1306 display = Adafruit_SSD1306 (128, 64, &Wire, OLED_RESET);

/*---------------------------------------------
    Encoder Initialisation 
 ----------------------------------------------*/
#define ENC_A  A3
#define ENC_B  A4
#define ENC_S  A5

Encoder rotary = Encoder (ENC_A, ENC_B, ENC_S, true);

/*---------------------------------------------
    NeoPixel/LED Initialisation 
 ----------------------------------------------*/
#ifdef M0_EXPRESS
#define PIXEL_PIN     8
#define PIXEL_COUNT   1
#define PIXEL_ON      255

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
#endif

/*---------------------------------------------
    Potentiometer mux Initialisation 
 ----------------------------------------------*/
#define POT_I    A0
#define POT_A    5
#define POT_B    6
#define POT_C    9
#define POT_D    10

#define POT_NUM  11
#define POT_THR  25

const bool selectA[]      = {  true, false,  true, false,  true, false,  true, false,  true, false,  true };
const bool selectB[]      = { false,  true,  true, false, false,  true,  true, false, false,  true,  true };
const bool selectC[]      = { false, false,  false, true,  true,  true,  true, false, false, false, false };
const bool selectD[]      = { false, false, false, false, false, false, false,  true,  true,  true,  true };

uint16_t potLast[]        = { 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000 };
uint16_t potCur[]         = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const bool potSingle[]    = { false, true, false, false, true, true, true, false, true, true, true };
const uint8_t potCC[]     = { 92, 84, 79, 76, 95, 89, 74, 82, 94, 73, 72 };
const uint8_t potMin[]    = { 0, 0, 0, 0, 0, 57, 14, 0, 0, 40, 40 };                // Minimum potentiometer value
const uint8_t potRng[]    = { 255, 31, 255, 255, 31, 14, 100, 255, 15, 48, 48 };    // Potentiometer range (not used in code, just for reference)
const uint16_t potScal[]  = { 15, 131, 15, 15, 131, 292, 40, 15, 272, 84, 84 };     // ADC scaling for potentiometer range (scal = 4096 / range[i]) 

/*---------------------------------------------
    MIDI Initialisation 
 ----------------------------------------------*/
MIDI_CREATE_DEFAULT_INSTANCE();

/*---------------------------------------------
    Data structures 
 ----------------------------------------------*/

// Distorsion effects (boost, clip)
#define DIST_BOOST    0
#define DIST_CLIP     1

// Non volatile configuration
typedef struct
{
  bool        valid;

  // MIDI Configuration
  uint8_t     midiChan;                 // MIDI channel [1-16]
  uint8_t     midiVol;                  // MIDI volume [0-15]
  uint8_t     midiPatch;                // MIDI patch [1-8]

  // VCO
  uint8_t     vcoWaveA;                 // Oscillator A waveform [0-9]
  bool        vcoRingA;                 // Osicllator A ring modulator [+8]
  bool        vcoSyncA;                 // Osicllator A sync [+16]
  uint8_t     vcoWaveB;                 // Oscillator B waveform [0-9]

  // LFO
  uint8_t     lfoControl;               // LFO control type [0-3]
  uint8_t     lfoWave;                  // LFO waveform [0-4]
  uint8_t     lfoDstFrq;                // LFO destination frequency [0-3]
  uint8_t     lfoDstFlt;                // LFO destination pulse+filter [0-7]

  // VCA
  uint8_t     vcaAttack;                // VCA attack [0-15]
  uint8_t     vcaDecay;                 // VCA decay [0-15]
  uint8_t     vcaSustain;               // VCA sustain [0-15]
  uint8_t     vcaRelease;               // VCA release [0-15]

  // Envelope
  uint8_t     envMod;                   // Envelope modulation [0-31]
  uint8_t     envFrq;                   // Envelope modulation frequency [0-3]
  uint8_t     envPwm;                   // Envelope modulation pulse width [0-3]
 
  // Filter
  uint8_t     fltType;                  // Filter type [0-15]
  uint8_t     fltAttack;                // Filter envelope attack [0-15]
  uint8_t     fltDecay;                 // Filter envelope decay [0-15]
  uint8_t     fltSustain;               // Filter envelope sustain [0-15]
  uint8_t     fltRelease;               // Filter envelope release [0-15]
  
} Configuration;

// Flash storage for non volatile information persistence
Configuration conf;
FlashStorage(flash, Configuration);

/*---------------------------------------------
    GUI constants
 ----------------------------------------------*/
String notes[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

int8_t guiLevel  = 0;
bool firstTime   = true;

// Main menu
// -------------------
#define MENU_NUM    6
#define MENU_VCO    0
#define MENU_LFO    1
#define MENU_VCA    2
#define MENU_ENV    3
#define MENU_FLT    4
#define MENU_MIDI   5

const String mainText[]     = { "VCO", "LFO", "VCA", "ENVEL", "FILT", "MIDI" };
const uint16_t mainPosX[]   = { 6*2, 9*6, 16*6, 6, 8*6, 14*6 };
const uint16_t mainPosY[]   = { 0, 0, 0, 10, 10, 10 };
int16_t mainSel             = MENU_VCO;

// VCO options
// -------------------
#define VCO_NUM   5
#define VCO_WAVA  0
#define VCO_RINA  1
#define VCO_SYNA  2
#define VCO_WAVB  3
#define VCO_END   4

const String vcoText[]     = { "Wave A:", "Ring A:", "Sync A:", "Wave B:", "Finish" };
const String vcoWaveText[] = { "OFF", "SINE", "SAW", "PULSE", "NOISE", "SINE+SAW", "SINE+PULSE", "SAW+PULSE", "ALL" };
const String vcoWaveEmpty  = "          ";
const uint16_t vcoPosX[]   = { 8*6, 8*6, 8*6, 8*6 };
const uint16_t vcoOffX[]   = { 0, 0, 0, 0, 15*6 };
const uint16_t vcoOffY[]   = { 24, 34, 44, 54, 44 };
int16_t vcoSel             = VCO_WAVA;

// LFO options
// -------------------
#define LFO_NUM   5
#define LFO_CTL   0
#define LFO_WAV   1
#define LFO_DFRQ  2
#define LFO_DFLT  3
#define LFO_END   4

const String lfoText[]       = { "Control:", "Wave:", "DstFrq:", "DstFlt:", "Finish" };
const String lfoCtrlText[]   = { "NONE", "KEY", "MIDI", "KEY+MIDI" };
const String lfoCtrlEmpty    = "        ";
const String lfoWaveText[]   = { "NONE", "SINE", "SAW", "PULSE", "NOISE" };
const String lfoWaveEmpty    = "     ";
const String lfoDstFrqText[] = { "NONE", "OSC A", "OSC B", "OSC A+B" };
const String lfoDstFrqEmpty  = "           ";
const String lfoDstFltText[] = { "NONE", "FILTER", "OSC A", "OSC B", "OSC A+B", "FLT/OSC A", "FLT/OSC B", "FLT/OSC A+B" };
const String lfoDstFltEmpty  = "           ";
const uint16_t lfoPosX[]     = { 9*6, 6*6, 8*6, 8*6 };
const uint16_t lfoOffX[]     = { 0, 0, 0, 0, 15*6 };
const uint16_t lfoOffY[]     = { 24, 34, 44, 54, 34 };
int16_t lfoSel               = LFO_CTL;

// VCA options
// -------------------
#define VCA_NUM  5
#define VCA_ATT  0
#define VCA_DEC  1
#define VCA_SUS  2
#define VCA_REL  3
#define VCA_END  4

const String vcaText[]    = { "Attack:", "Decay:", "Sustain:", "Release: ", "Finish" };
const uint16_t vcaOffX[]  = { 0, 0, 0, 0, 15*6 };
const uint16_t vcaOffY[]  = { 24, 34, 44, 54, 54 };
const uint16_t vcaPosX[]  = { 8*6, 7*6, 9*6, 9*6 };
int16_t vcaSel            = VCA_ATT;

// Envelope options
// -------------------
#define ENV_NUM   4
#define ENV_MOD   0
#define ENV_FRQ   1
#define ENV_PWM   2
#define ENV_END   3

const String envText[]      = { "Modulation:", "Frequency:", "Pulse Width:", "Finish" };
const String envModText[]   = { "NONE", "OSC A", "OSC B", "OSC A+B" };
const String envModEmpty    = "        ";
const uint16_t envOffX[]    = { 0, 0, 0, 0 };
const uint16_t envOffY[]    = { 24, 34, 44, 54 };
const uint16_t envPosX[]    = { 12*6, 11*6, 13*6 };
int16_t envSel              = ENV_MOD;

// Filter options
// -------------------
#define FLT_NUM   6
#define FLT_TYP   0
#define FLT_ATT   1
#define FLT_DEC   2
#define FLT_SUS   3
#define FLT_REL   4
#define FLT_END   5

const String fltText[]     = { "Type:", "EnvAtt:", "EnvDec:", "EnvSus:", "EnvRel:", "Finish" };
const String fltTypeText[] = { "OFF", "LOW", "BAND", "HIGH", "LO+BND", "LO+HI", "BND+HI", "LO+BND+HI", "LO+EXT", "BND+EXT", "HI+EXT", "LO+BND+EXT", "LO+HI+EXT", "BND+HI+EXT", "LO+BND+HI+EXT", "EXT" };
const String fltTypeEmpty  = "              ";
const uint16_t fltOffX[]   = { 0, 0, 11*6, 0, 11*6, 0 };
const uint16_t fltOffY[]   = { 24, 34, 34, 44, 44, 54 };
const uint16_t fltPosX[]   = { 6*6, 8*6, 19*6, 8*6, 19*6 };
int16_t fltSel             = FLT_TYP;

// Configuration options
// -------------------
#define MIDI_NUM  4
#define MIDI_PCH  0
#define MIDI_VOL  1
#define MIDI_CHA  2
#define MIDI_END  3

const String midiText[]      = { "Patch:", "Volume: ", "Channel:", "Finish" };
const uint16_t midiOffX[]    = { 0, 0, 0, 0 };
const uint16_t midiOffY[]    = { 24, 34, 44, 54 };
const uint16_t midiPosX[]    = { 8*6, 8*6, 9*6 };
int16_t midiSel              = MIDI_PCH;

/*---------------------------------------------
    Program state
 ----------------------------------------------*/
int     count, lcount = -10;

int     ledCount = 0;
bool    ledToggle = false;

int     noteCount = 0;          // Keep track of concurrent MIDI notes

// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin (115200);

  // ------------------------------------
  // Program non volatile configuration
  // ------------------------------------
  conf = flash.read();
  if (conf.valid == false) 
  {
    conf.valid = true;

    conf.midiChan = 4;
    conf.midiVol = 15;
    conf.midiPatch = 1;

    conf.vcoWaveA = 8;
    conf.vcoRingA = false;
    conf.vcoSyncA = false;
    conf.vcoWaveB = 8;

    conf.lfoControl = 0;
    conf.lfoWave = 0;
    conf.lfoDstFrq = 0;
    conf.lfoDstFlt = 0;

    conf.vcaAttack = 1;
    conf.vcaDecay = 0;
    conf.vcaSustain = 15;
    conf.vcaRelease = 5; 

    conf.envFrq = 0;
    conf.envPwm = 0;

    conf.fltType = 0;
    conf.fltAttack = 1;
    conf.fltDecay = 0;
    conf.fltSustain = 15;
    conf.fltRelease = 5;
  }
  
  // ------------------------------
  // Configure OLED
  // ------------------------------
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setRotation(2);
  display.drawBitmap (0, 0, init_bmp, 128, 64, SSD1306_WHITE);   
  display.display();
  delay (2000); 

  // Clear the buffer and show text
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < MENU_NUM; i++)
  {
    display.setCursor (mainPosX[i], mainPosY[i]);
    display.print (mainText[i]);
  }
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor (mainPosX[mainSel], mainPosY[mainSel]);
  display.print (mainText[mainSel]);
  display.drawLine (0, 22, 128, 22, SSD1306_WHITE);
  display.display();

  // Configure status LED
  pinMode(LED_BUILTIN, OUTPUT);

  // ------------------------------
  // Configure NeoPixels
  // ------------------------------
#ifdef M0_EXPRESS
  strip.begin();
  strip.show();
#endif

  // ------------------------------
  // Configure MIDI
  // ------------------------------
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  // MIDI.setHandleProgramChange(handleProgramChange);
  // MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
  // MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
  // MIDI.setHandleControlChange(handleControlChange);
  // MIDI.setHandlePitchBend(handlePitchBend);

  MIDI.begin(conf.midiChan);
  MIDI.turnThruOn();

  // -----------------------
  // ADC & mux Configuration
  // -----------------------
  analogReference (AR_DEFAULT);
  analogReadResolution (12);
  pinMode(POT_A, OUTPUT);
  pinMode(POT_B, OUTPUT);
  pinMode(POT_C, OUTPUT);
  pinMode(POT_D, OUTPUT);
}

void loop()
{
  // Scan encoder
  rotary.read ();
  count = rotary.readCount();
  if (count != lcount)
  {
    gui_rotaryAction (count - lcount);
    lcount = count;
 }

  // Scan switch
  if (rotary.check (HIGH))
  {
    if (firstTime)
      firstTime = false;
    else
      gui_switchAction ();  
  }

  // Scan MIDI port
  MIDI.read();

  // Poll ADC mux
  int     val;

  for (int i = 0; i < POT_NUM; i++)
  {
    digitalWrite (POT_A, selectA[i]);
    digitalWrite (POT_B, selectB[i]);
    digitalWrite (POT_C, selectC[i]);
    digitalWrite (POT_D, selectD[i]);

    delay (1);

    potCur[i] = analogRead (POT_I);
    if (abs (potCur[i]-potLast[i]) > POT_THR)
    {
      val = potMin[i] + potCur[i] / potScal[i];
      if (potSingle[i])
        MIDI.sendControlChange(potCC[i], val, conf.midiChan);
      else
      {
        if (val < 128)
          MIDI.sendControlChange(potCC[i], val, conf.midiChan);
        else
          MIDI.sendControlChange(potCC[i]+1, val-127, conf.midiChan);
      }

      Serial.print(potCC[i]) ;
      Serial.print (" ");
      Serial.println (val);

      potLast[i] = potCur[i];
    }   
  }
}

//*****************************************************************************
/*-------------------------------------------
    MIDI routines
 --------------------------------------------*/

inline void changePixel ()
{
  strip.setPixelColor (0, strip.Color (0, (noteCount == 0 ? 0 : PIXEL_ON), 0));
  strip.show ();
}

inline uint8_t selectWaveform (uint8_t wave, bool ring, bool sync)
{
  if (wave == 0)    return 0;

  return wave + (ring && !sync ? 8 : 0) + (!ring && sync ? 16 : 0) + (ring && sync ? 24 : 0);
}

// -----------------------------------------------------------------------------

void handleNoteOn(byte channel, byte pitch, byte velocity)
{  
#ifdef DEBUG
  Serial.println ("MIDI ON");
#endif

  MIDI.sendNoteOn(pitch,velocity,channel);

  // Check NoteOn messages with 0 velocity. Should be interpreted as NoteOffs.
  if (velocity == 0)
  {
    noteCount --;
    if (noteCount == 0)
      changePixel ();
    return;
  }
  
  noteCount ++;
  if (noteCount == 1)
    changePixel ();
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
#ifdef DEBUG
  Serial.println ("MIDI OFF");
#endif

  MIDI.sendNoteOff(pitch,velocity,channel);

  noteCount --;
  if (noteCount == 0)
     changePixel ();
}

/*-------------------------------------------
    PROGRAM LOGIC
 --------------------------------------------*/

inline void gui_rotaryAction (int diff)
{
  switch (guiLevel)
  {
    case 0:                   gui_selectMenu (diff); break;
      
    case 1:
      switch (mainSel)
      {
        case MENU_VCO:        gui_selectVco (diff); break;
        case MENU_LFO:        gui_selectLfo (diff); break;
        case MENU_VCA:        gui_selectVca (diff); break;
        case MENU_ENV:        gui_selectEnv (diff); break;
        case MENU_FLT:        gui_selectFlt (diff); break;
        case MENU_MIDI:       gui_selectMidi (diff); break;
      }
      break;

    case 2:
      switch (mainSel)
      {
        case MENU_VCO:
          switch (vcoSel)
          {
            case VCO_WAVA:    conf.vcoWaveA = gui_selectLabel (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoWaveA, diff, vcoWaveText, vcoWaveEmpty, 9); MIDI.sendControlChange(70, selectWaveform (conf.vcoWaveA, conf.vcoRingA, conf.vcoSyncA), conf.midiChan); break;
            case VCO_RINA:    conf.vcoRingA = gui_selectBool (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoRingA); MIDI.sendControlChange(70, selectWaveform (conf.vcoWaveA, conf.vcoRingA, conf.vcoSyncA), conf.midiChan); break;
            case VCO_SYNA:    conf.vcoSyncA = gui_selectBool (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoSyncA); MIDI.sendControlChange(70, selectWaveform (conf.vcoWaveA, conf.vcoRingA, conf.vcoSyncA), conf.midiChan); break;
            case VCO_WAVB:    conf.vcoWaveB = gui_selectLabel (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoWaveB, diff, vcoWaveText, vcoWaveEmpty, 9); MIDI.sendControlChange(71, conf.vcoWaveB, conf.midiChan); break;
          }
          break;
          
        case MENU_LFO:
          switch (lfoSel)
          {
            case LFO_CTL:     conf.lfoControl = gui_selectLabel (lfoPosX[lfoSel], lfoOffY[lfoSel], conf.lfoControl, diff, lfoCtrlText, lfoCtrlEmpty, 4); MIDI.sendControlChange(85, conf.lfoControl, conf.midiChan); break;
            case LFO_WAV:     conf.lfoWave = gui_selectLabel (lfoPosX[lfoSel], lfoOffY[lfoSel], conf.lfoWave, diff, lfoWaveText, lfoWaveEmpty, 5); MIDI.sendControlChange(86, conf.lfoWave, conf.midiChan); break;
            case LFO_DFRQ:    conf.lfoDstFrq = gui_selectLabel (lfoPosX[lfoSel], lfoOffY[lfoSel], conf.lfoDstFrq, diff, lfoDstFrqText, lfoDstFrqEmpty, 4); MIDI.sendControlChange(87, conf.lfoDstFrq, conf.midiChan); break;
            case LFO_DFLT:    conf.lfoDstFlt = gui_selectLabel (lfoPosX[lfoSel], lfoOffY[lfoSel], conf.lfoDstFlt, diff, lfoDstFltText, lfoDstFltEmpty, 8); MIDI.sendControlChange(88, conf.lfoDstFlt, conf.midiChan); break;
          }
          break;
          
       case MENU_VCA:
          switch (vcaSel)
          {
            case VCA_ATT:     conf.vcaAttack = gui_selectInt (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaAttack, diff, 0, 15); MIDI.sendControlChange(100, conf.vcaAttack, conf.midiChan); break;
            case VCA_DEC:     conf.vcaDecay = gui_selectInt (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaDecay, diff, 0, 15); MIDI.sendControlChange(101, conf.vcaDecay, conf.midiChan); break;
            case VCA_SUS:     conf.vcaSustain = gui_selectInt (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaSustain, diff, 0, 15); MIDI.sendControlChange(102, conf.vcaSustain, conf.midiChan); break;
            case VCA_REL:     conf.vcaRelease = gui_selectInt (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaRelease, diff, 0, 15); MIDI.sendControlChange(103, conf.vcaRelease, conf.midiChan); break;
          }
          break;
          
        case MENU_ENV:
          switch (envSel)
          {
            case ENV_MOD:     conf.envMod = gui_selectInt (envPosX[envSel], envOffY[envSel], conf.envMod, diff, 0, 31); MIDI.sendControlChange(67, conf.envMod, conf.midiChan); break;
            case ENV_FRQ:     conf.envFrq = gui_selectLabel (envPosX[envSel], envOffY[envSel], conf.envFrq, diff, envModText, envModEmpty, 4); MIDI.sendControlChange(68, conf.envFrq, conf.midiChan); break;
            case ENV_PWM:     conf.envPwm = gui_selectLabel (envPosX[envSel], envOffY[envSel], conf.envPwm, diff, envModText, envModEmpty, 4); MIDI.sendControlChange(69, conf.envPwm, conf.midiChan); break;
          }
          break;
          
        case MENU_FLT:
          switch (fltSel)
          {
            case FLT_TYP:     conf.fltType = gui_selectLabel (fltPosX[fltSel], fltOffY[fltSel], conf.fltType, diff, fltTypeText, fltTypeEmpty, 16); MIDI.sendControlChange(90, conf.fltType, conf.midiChan); break;
            case FLT_ATT:     conf.fltAttack = gui_selectInt (fltPosX[fltSel], fltOffY[fltSel], conf.fltAttack, diff, 0, 15); MIDI.sendControlChange(96, conf.fltAttack, conf.midiChan); break;
            case FLT_DEC:     conf.fltDecay = gui_selectInt (fltPosX[fltSel], fltOffY[fltSel], conf.fltDecay, diff, 0, 15); MIDI.sendControlChange(97, conf.fltDecay, conf.midiChan); break;
            case FLT_SUS:     conf.fltSustain = gui_selectInt (fltPosX[fltSel], fltOffY[fltSel], conf.fltSustain, diff, 0, 15); MIDI.sendControlChange(98, conf.fltSustain, conf.midiChan); break;
            case FLT_REL:     conf.vcaRelease = gui_selectInt (fltPosX[fltSel], fltOffY[fltSel], conf.fltRelease, diff, 0, 15); MIDI.sendControlChange(99, conf.fltRelease, conf.midiChan); break;
          }
          break;
          
        case MENU_MIDI:
          switch (midiSel)
          {
            case MIDI_PCH:   conf.midiPatch = gui_selectInt (midiPosX[midiSel], midiOffY[midiSel], conf.midiPatch, diff, 1, 8); MIDI.sendProgramChange (conf.midiPatch, conf.midiChan); break;
            case MIDI_VOL:   conf.midiVol = gui_selectInt (midiPosX[midiSel], midiOffY[midiSel], conf.midiVol, diff, 0, 15); MIDI.sendControlChange(7, conf.midiVol, conf.midiChan); break;
            case MIDI_CHA:   conf.midiChan = gui_selectInt (midiPosX[midiSel], midiOffY[midiSel], conf.midiChan, diff, 1, 16); break;
           }
          break;
      }
    flash.write(conf);
    break;
  }    
}

inline void gui_switchAction ()
{
  switch (guiLevel)
  {
    case 0:
      guiLevel ++;
      switch (mainSel)
      {
        case MENU_VCO:          vcoSel = VCO_WAVA;      gui_writeTextInverse (vcoOffX[vcoSel], vcoOffY[vcoSel], vcoText[vcoSel]); break;        
        case MENU_LFO:          lfoSel = LFO_CTL;       gui_writeTextInverse (lfoOffX[lfoSel], lfoOffY[lfoSel], lfoText[lfoSel]); break;        
        case MENU_VCA:          vcaSel = VCA_ATT;       gui_writeTextInverse (vcaOffX[vcaSel], vcaOffY[vcaSel], vcaText[vcaSel]); break;        
        case MENU_ENV:          envSel = ENV_MOD;       gui_writeTextInverse (envOffX[envSel], envOffY[envSel], envText[envSel]); break;         
        case MENU_FLT:          fltSel = FLT_TYP;       gui_writeTextInverse (fltOffX[fltSel], fltOffY[fltSel], fltText[fltSel]); break;      
        case MENU_MIDI:         midiSel = MIDI_PCH;     gui_writeTextInverse (midiOffX[midiSel], midiOffY[midiSel], midiText[midiSel]); break;
      }
      break;

    case 1:
      guiLevel ++;
      switch (mainSel)
      {
        case MENU_VCO:
          gui_writeTextNormal (vcoOffX[vcoSel], vcoOffY[vcoSel], vcoText[vcoSel]);
          switch (vcoSel)
          {
            case VCO_WAVA:     gui_writeTextInverse (vcoPosX[vcoSel], vcoOffY[vcoSel], vcoWaveText[conf.vcoWaveA]); break;
            case VCO_RINA:     gui_writeTextInverse (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoRingA ? "ON " : "OFF"); break;
            case VCO_SYNA:     gui_writeTextInverse (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoSyncA ? "ON " : "OFF"); break;
            case VCO_WAVB:     gui_writeTextInverse (vcoPosX[vcoSel], vcoOffY[vcoSel], vcoWaveText[conf.vcoWaveB]); break;
            case VCO_END:      guiLevel -= 2; break;
          }
          break;
          
        case MENU_LFO:
          gui_writeTextNormal (lfoOffX[lfoSel], lfoOffY[lfoSel], lfoText[lfoSel]);
          switch (lfoSel)
          {
            case LFO_CTL:      gui_writeTextInverse (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoCtrlText[conf.lfoControl]); break;
            case LFO_WAV:      gui_writeTextInverse (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoWaveText[conf.lfoWave]); break;
            case LFO_DFRQ:     gui_writeTextInverse (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoDstFrqText[conf.lfoDstFrq]); break;
            case LFO_DFLT:     gui_writeTextInverse (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoDstFltText[conf.lfoDstFlt]); break;
            case LFO_END:      guiLevel -= 2; break;
          }
          break;
          
        case MENU_VCA:
          gui_writeTextNormal (vcaOffX[vcaSel], vcaOffY[vcaSel], vcaText[vcaSel]);
          switch (vcaSel)
          {
            case VCA_ATT:      gui_writeIntInverse (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaAttack); break;
            case VCA_DEC:      gui_writeIntInverse (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaDecay); break;
            case VCA_SUS:      gui_writeIntInverse (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaSustain); break;
            case VCA_REL:      gui_writeIntInverse (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaRelease); break;
            case VCA_END:      guiLevel -= 2; break;
          }
          break;
          
        case MENU_ENV:
          gui_writeTextNormal (envOffX[envSel], envOffY[envSel], envText[envSel]);
          switch (envSel)
          {
            case ENV_MOD:     gui_writeIntInverse (envPosX[envSel], envOffY[envSel], conf.envMod); break;
            case ENV_FRQ:     gui_writeTextInverse (envPosX[envSel], envOffY[envSel], envModText[conf.envFrq]); break;
            case ENV_PWM:     gui_writeTextInverse (envPosX[envSel], envOffY[envSel], envModText[conf.envPwm]); break;
            case ENV_END:     guiLevel -= 2; break;
          }
          break;
          
        case MENU_FLT:
          gui_writeTextNormal (fltOffX[fltSel], fltOffY[fltSel], fltText[fltSel]);
          switch (fltSel)
          {
            case FLT_TYP:      gui_writeTextInverse (fltPosX[fltSel], fltOffY[fltSel], fltTypeText[conf.fltType]); break;
            case FLT_ATT:      gui_writeIntInverse (fltPosX[fltSel], fltOffY[fltSel], conf.fltAttack); break;
            case FLT_DEC:      gui_writeIntInverse (fltPosX[fltSel], fltOffY[fltSel], conf.fltDecay); break;
            case FLT_SUS:      gui_writeIntInverse (fltPosX[fltSel], fltOffY[fltSel], conf.fltSustain); break;
            case FLT_REL:      gui_writeIntInverse (fltPosX[fltSel], fltOffY[fltSel], conf.fltRelease); break;
            case FLT_END:      guiLevel -= 2; break;
          }
          break;
          
        case MENU_MIDI:
          gui_writeTextNormal (midiOffX[midiSel], midiOffY[midiSel], midiText[midiSel]);
          switch (midiSel)
          {
            case MIDI_PCH:    gui_writeIntInverse (midiPosX[midiSel], midiOffY[midiSel], conf.midiPatch); break;
            case MIDI_CHA:    gui_writeIntInverse (midiPosX[midiSel], midiOffY[midiSel], conf.midiChan); break;
            case MIDI_VOL:    gui_writeIntInverse (midiPosX[midiSel], midiOffY[midiSel], conf.midiVol); break;
            case MIDI_END:    guiLevel -= 2; break;
          }
          break;
      }
      break;

    case 2:
      guiLevel --;
      switch (mainSel)
      {
        case MENU_VCO:
          gui_writeTextInverse (vcoOffX[vcoSel], vcoOffY[vcoSel], vcoText[vcoSel]);
          switch (vcoSel)
          {
            case VCO_WAVA:     gui_writeTextNormal (vcoPosX[vcoSel], vcoOffY[vcoSel], vcoWaveText[conf.vcoWaveA]); break;
            case VCO_RINA:     gui_writeTextNormal (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoRingA ? "ON " : "OFF"); break;
            case VCO_SYNA:     gui_writeTextNormal (vcoPosX[vcoSel], vcoOffY[vcoSel], conf.vcoSyncA ? "ON " : "OFF"); break;
            case VCO_WAVB:     gui_writeTextNormal (vcoPosX[vcoSel], vcoOffY[vcoSel], vcoWaveText[conf.vcoWaveB]); break;
          }
          break;
          
        case MENU_LFO:
          gui_writeTextInverse (lfoOffX[lfoSel], lfoOffY[lfoSel], lfoText[lfoSel]);
          switch (lfoSel)
          {
            case LFO_CTL:      gui_writeTextNormal (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoCtrlText[conf.lfoControl]); break;
            case LFO_WAV:      gui_writeTextNormal (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoWaveText[conf.lfoWave]); break;
            case LFO_DFRQ:     gui_writeTextNormal (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoDstFrqText[conf.lfoDstFrq]); break;
            case LFO_DFLT:     gui_writeTextNormal (lfoPosX[lfoSel], lfoOffY[lfoSel], lfoDstFltText[conf.lfoDstFlt]); break;
          }
          break;
          
        case MENU_VCA:
          gui_writeTextInverse (vcaOffX[vcaSel], vcaOffY[vcaSel], vcaText[vcaSel]);
          switch (vcaSel)
          {
            case VCA_ATT:      gui_writeIntNormal (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaAttack); break;
            case VCA_DEC:      gui_writeIntNormal (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaDecay); break;
            case VCA_SUS:      gui_writeIntNormal (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaSustain); break;
            case VCA_REL:      gui_writeIntNormal (vcaPosX[vcaSel], vcaOffY[vcaSel], conf.vcaRelease); break;
           }
          break;
          
        case MENU_ENV:
          gui_writeTextInverse (envOffX[envSel], envOffY[envSel], envText[envSel]);
          switch (envSel)
          {
            case ENV_MOD:     gui_writeIntNormal (envPosX[envSel], envOffY[envSel], conf.envMod); break;
            case ENV_FRQ:     gui_writeTextNormal (envPosX[envSel], envOffY[envSel], envModText[conf.envFrq]); break;
            case ENV_PWM:     gui_writeTextNormal (envPosX[envSel], envOffY[envSel], envModText[conf.envPwm]); break;
          }
          break;
          
        case MENU_FLT:
          gui_writeTextInverse (fltOffX[fltSel], fltOffY[fltSel], fltText[fltSel]);
          switch (fltSel)
          {
            case FLT_TYP:      gui_writeTextNormal (fltPosX[fltSel], fltOffY[fltSel], fltTypeText[conf.fltType]); break;
            case FLT_ATT:      gui_writeIntNormal (fltPosX[fltSel], fltOffY[fltSel], conf.fltAttack); break;
            case FLT_DEC:      gui_writeIntNormal (fltPosX[fltSel], fltOffY[fltSel], conf.fltDecay); break;
            case FLT_SUS:      gui_writeIntNormal (fltPosX[fltSel], fltOffY[fltSel], conf.fltSustain); break;
            case FLT_REL:      gui_writeIntNormal (fltPosX[fltSel], fltOffY[fltSel], conf.fltRelease); break;
          }
          break;
          
        case MENU_MIDI:
          gui_writeTextInverse (midiOffX[midiSel], midiOffY[midiSel], midiText[midiSel]);
          switch (midiSel)
          {
            case MIDI_PCH:    gui_writeIntNormal (midiPosX[midiSel], midiOffY[midiSel], conf.midiPatch); break;
            case MIDI_CHA:    gui_writeIntNormal (midiPosX[midiSel], midiOffY[midiSel], conf.midiChan); break;
            case MIDI_VOL:    gui_writeIntNormal (midiPosX[midiSel], midiOffY[midiSel], conf.midiVol); break;
          }
          break;
      }
      break;
  }
  display.display();
}

//*****************************************************************************
/*-------------------------------------------
    INTERFACE
 --------------------------------------------*/

inline void gui_writeTextNormal (int x, int y, String text)
{
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor (x, y);
  display.print (text);
}

inline void gui_writeTextInverse (int x, int y, String text)
{
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor (x, y);
  display.print (text);  
}

inline void gui_writeIntNormal (int x, int y, uint16_t number)
{
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.setCursor (x, y);
  display.print (number);
}

inline void gui_writeIntInverse (int x, int y, uint16_t number)
{
  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
  display.setCursor (x, y);
  display.print (number);  
}

inline bool gui_selectBool (int x, int y, bool enabled)
{
  enabled = !enabled;
  gui_writeTextNormal (x, y, "   ");
  gui_writeTextInverse (x, y, enabled ? "ON" : "OFF");
  display.display();
  return enabled;
}

inline int8_t gui_selectLabel (int x, int y, int8_t selected, int diff, const String labels[], const String empty, int8_t maxnum)
{
  // Compute new selection
  selected += diff;
  if (selected >= maxnum)
    selected = 0;
  else if (selected < 0)
    selected = maxnum-1;
    
  // Update new selection
  gui_writeTextNormal (x, y, empty);
  gui_writeTextInverse (x, y, labels[selected]);
  display.display();
  return selected;
}

inline int16_t gui_selectInt (int x, int y, int16_t number, int diff, int16_t minimum, int16_t maximum)
{
  // Compute new selection
  number += diff;
  if (number > maximum)
    number = maximum;
  else if (number < minimum)
    number = minimum;
    
  // Update new selection
  gui_writeTextNormal (x, y, "  ");
  gui_writeIntInverse (x, y, number);
  display.display();
  return number;
}

inline void gui_selectMenu (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (mainPosX[mainSel], mainPosY[mainSel], mainText[mainSel]);

  // Compute new selection
  mainSel += diff;
  if (mainSel >= MENU_NUM)
    mainSel = 0;
  else if (mainSel < 0)
    mainSel = MENU_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (mainPosX[mainSel], mainPosY[mainSel], mainText[mainSel]);
  display.display();

  switch (mainSel)
  {
    case MENU_VCO:        gui_optsVco (); break;
    case MENU_LFO:        gui_optsLfo (); break;
    case MENU_VCA:        gui_optsVca (); break;
    case MENU_ENV:        gui_optsEnv (); break;
    case MENU_FLT:        gui_optsFlt (); break;
    case MENU_MIDI:       gui_optsMidi (); break;
  }
}

inline void gui_selectVco (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (vcoOffX[vcoSel], vcoOffY[vcoSel], vcoText[vcoSel]);

  // Compute new selection
  vcoSel += diff;
  if (vcoSel >= VCO_NUM)
    vcoSel = 0;
  else if (vcoSel < 0)
    vcoSel = VCO_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (vcoOffX[vcoSel], vcoOffY[vcoSel], vcoText[vcoSel]);
  display.display();
}

inline void gui_selectLfo (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (lfoOffX[lfoSel], lfoOffY[lfoSel], lfoText[lfoSel]);

  // Compute new selection
  lfoSel += diff;
  if (lfoSel >= LFO_NUM)
    lfoSel = 0;
  else if (lfoSel < 0)
    lfoSel = LFO_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (lfoOffX[lfoSel], lfoOffY[lfoSel], lfoText[lfoSel]);
  display.display();
}

inline void gui_selectVca (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (vcaOffX[vcaSel], vcaOffY[vcaSel], vcaText[vcaSel]);

  // Compute new selection
  vcaSel += diff;
  if (vcaSel >= VCA_NUM)
    vcaSel = 0;
  else if (vcaSel < 0)
    vcaSel = VCA_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (vcaOffX[vcaSel], vcaOffY[vcaSel], vcaText[vcaSel]);
  display.display();
}

inline void gui_selectEnv (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (envOffX[envSel], envOffY[envSel], envText[envSel]);

  // Compute new selection
  envSel += diff;
  if (envSel >= ENV_NUM)
    envSel = 0;
  else if (envSel < 0)
    envSel = ENV_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (envOffX[envSel], envOffY[envSel], envText[envSel]);
  display.display();
}

inline void gui_selectFlt (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (fltOffX[fltSel], fltOffY[fltSel], fltText[fltSel]);

  // Compute new selection
  fltSel += diff;
  if (fltSel >= FLT_NUM)
    fltSel = 0;
  else if (fltSel < 0)
    fltSel = FLT_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (fltOffX[fltSel], fltOffY[fltSel], fltText[fltSel]);
  display.display();
}

inline void gui_selectMidi (int diff)
{
  // Restore previous selection
  gui_writeTextNormal (midiOffX[midiSel], midiOffY[midiSel], midiText[midiSel]);

  // Compute new selection
  midiSel += diff;
  if (midiSel >= MIDI_NUM)
    midiSel = 0;
  else if (midiSel < 0)
    midiSel = MIDI_NUM-1;
    
  // Update new selection
  gui_writeTextInverse (midiOffX[midiSel], midiOffY[midiSel], midiText[midiSel]);
  display.display();
}

inline void gui_optsVco ()
{
  display.fillRect (0, 24, 128, 40, SSD1306_BLACK);
  display.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < VCO_NUM; i++)
  {
    display.setCursor (vcoOffX[i], vcoOffY[i]);
    display.print (vcoText[i]);
  }
  display.setCursor (vcoPosX[0], vcoOffY[0]);
  display.print (vcoWaveText[conf.vcoWaveA]);
  display.setCursor (vcoPosX[1], vcoOffY[1]);
  display.print (conf.vcoRingA ? "ON" : "OFF");
  display.setCursor (vcoPosX[2], vcoOffY[2]);
  display.print (conf.vcoSyncA ? "ON" : "OFF");
  display.setCursor (vcoPosX[3], vcoOffY[3]);
  display.print (vcoWaveText[conf.vcoWaveB]);

  display.display();
}

inline void gui_optsLfo ()
{
  display.fillRect (0, 24, 128, 40, SSD1306_BLACK);
  display.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < LFO_NUM; i++)
  {
    display.setCursor (lfoOffX[i], lfoOffY[i]);
    display.print (lfoText[i]);
  }
  display.setCursor (lfoPosX[0], lfoOffY[0]);
  display.print (lfoCtrlText[conf.lfoControl]);
  display.setCursor (lfoPosX[1], lfoOffY[1]);
  display.print (lfoWaveText[conf.lfoWave]);
  display.setCursor (lfoPosX[2], lfoOffY[2]);
  display.print (lfoDstFrqText[conf.lfoDstFrq]);
  display.setCursor (lfoPosX[3], lfoOffY[3]);
  display.print (lfoDstFltText[conf.lfoDstFlt]);

  display.display();
}

inline void gui_optsVca ()
{
  display.fillRect (0, 24, 128, 40, SSD1306_BLACK);
  display.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < VCA_NUM; i++)
  {
    display.setCursor (vcaOffX[i], vcaOffY[i]);
    display.print (vcaText[i]);
  }
  display.setCursor (vcaPosX[0], vcaOffY[0]);
  display.print (conf.vcaAttack);
  display.setCursor (vcaPosX[1], vcaOffY[1]);
  display.print (conf.vcaDecay);
  display.setCursor (vcaPosX[2], vcaOffY[2]);
  display.print (conf.vcaSustain);
  display.setCursor (vcaPosX[3], vcaOffY[3]);
  display.print (conf.vcaRelease);

  display.display();
}

inline void gui_optsEnv ()
{
  display.fillRect (0, 24, 128, 40, SSD1306_BLACK);
  display.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < ENV_NUM; i++)
  {
    display.setCursor (envOffX[i], envOffY[i]);
    display.print (envText[i]);
  }
  display.setCursor (envPosX[0], envOffY[0]);
  display.print (conf.envMod);
  display.setCursor (envPosX[1], envOffY[1]);
  display.print (envModText[conf.envFrq]);
  display.setCursor (envPosX[2], envOffY[2]);
  display.print (envModText[conf.envPwm]);
  
  display.display();
}

inline void gui_optsFlt ()
{
  display.fillRect (0, 24, 128, 40, SSD1306_BLACK);
  display.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < FLT_NUM; i++)
  {
    display.setCursor (fltOffX[i], fltOffY[i]);
    display.print (fltText[i]);
  }
  display.setCursor (fltPosX[0], fltOffY[0]);
  display.print (fltTypeText[conf.fltType]);
  display.setCursor (fltPosX[1], fltOffY[1]);
  display.print (conf.fltAttack);
  display.setCursor (fltPosX[2], fltOffY[2]);
  display.print (conf.fltDecay);
  display.setCursor (fltPosX[3], fltOffY[3]);
  display.print (conf.fltSustain);
  display.setCursor (fltPosX[4], fltOffY[4]);
  display.print (conf.fltRelease);

  display.display();
}

inline void gui_optsMidi ()
{
  display.fillRect (0, 24, 128, 40, SSD1306_BLACK);
  display.setTextColor (SSD1306_WHITE, SSD1306_BLACK);
  for (int i = 0; i < MIDI_NUM; i++)
  {
    display.setCursor (midiOffX[i], midiOffY[i]);
    display.print (midiText[i]);
  }
  display.setCursor (midiPosX[0], midiOffY[0]);
  display.print (conf.midiPatch);
  display.setCursor (midiPosX[1], midiOffY[1]);
  display.print (conf.midiVol);
  display.setCursor (midiPosX[2], midiOffY[2]);
  display.print (conf.midiChan);

  display.display();
}
