/* Copyright 2021 OpenAnnePro community
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
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

#include "aw20216s.h"

/*
 * AW20216S LED channel map for the Anne Pro 2D direct-drive RGB.
 *
 * Reversed from the official AP2D KEY 3.08 firmware: the per-key channel
 * table at 0x123C4 maps each matrix (row, col) to a base PWM channel, and
 * each key uses three consecutive channels R=base, G=base+1, B=base+2.
 * The official matrix pin order (rows C3 C14 C15 C1 C2, cols C4 C5 C8 C0
 * A10 B1 A8 C13 C12 A15 A14 A11 D1 D2) matches keyboard.json, so the LED
 * index order here follows the rgb_matrix layout in keyboard.json.
 */

#if defined(RGB_MATRIX_AW20216S)

aw20216s_led_t PROGMEM g_aw20216s_leds[AW20216S_LED_COUNT] = {
    /* driver   R             G             B */
    {0, SW1_CS1,   SW1_CS2,   SW1_CS3},   //  0  matrix[0][0]
    {0, SW2_CS1,   SW2_CS2,   SW2_CS3},   //  1  matrix[0][1]
    {0, SW3_CS1,   SW3_CS2,   SW3_CS3},   //  2  matrix[0][2]
    {0, SW4_CS1,   SW4_CS2,   SW4_CS3},   //  3  matrix[0][3]
    {0, SW5_CS1,   SW5_CS2,   SW5_CS3},   //  4  matrix[0][4]
    {0, SW6_CS1,   SW6_CS2,   SW6_CS3},   //  5  matrix[0][5]
    {0, SW7_CS1,   SW7_CS2,   SW7_CS3},   //  6  matrix[0][6]
    {0, SW8_CS1,   SW8_CS2,   SW8_CS3},   //  7  matrix[0][7]
    {0, SW9_CS1,   SW9_CS2,   SW9_CS3},   //  8  matrix[0][8]
    {0, SW10_CS1,  SW10_CS2,  SW10_CS3},  //  9  matrix[0][9]
    {0, SW11_CS1,  SW11_CS2,  SW11_CS3},  // 10  matrix[0][10]
    {0, SW12_CS1,  SW12_CS2,  SW12_CS3},  // 11  matrix[0][11]
    {0, SW1_CS16,  SW1_CS17,  SW1_CS18},  // 12  matrix[0][12]
    {0, SW2_CS16,  SW2_CS17,  SW2_CS18},  // 13  matrix[0][13]
    {0, SW1_CS4,   SW1_CS5,   SW1_CS6},   // 14  matrix[1][0]
    {0, SW2_CS4,   SW2_CS5,   SW2_CS6},   // 15  matrix[1][1]
    {0, SW3_CS4,   SW3_CS5,   SW3_CS6},   // 16  matrix[1][2]
    {0, SW4_CS4,   SW4_CS5,   SW4_CS6},   // 17  matrix[1][3]
    {0, SW5_CS4,   SW5_CS5,   SW5_CS6},   // 18  matrix[1][4]
    {0, SW6_CS4,   SW6_CS5,   SW6_CS6},   // 19  matrix[1][5]
    {0, SW7_CS4,   SW7_CS5,   SW7_CS6},   // 20  matrix[1][6]
    {0, SW8_CS4,   SW8_CS5,   SW8_CS6},   // 21  matrix[1][7]
    {0, SW9_CS4,   SW9_CS5,   SW9_CS6},   // 22  matrix[1][8]
    {0, SW10_CS4,  SW10_CS5,  SW10_CS6},  // 23  matrix[1][9]
    {0, SW11_CS4,  SW11_CS5,  SW11_CS6},  // 24  matrix[1][10]
    {0, SW12_CS4,  SW12_CS5,  SW12_CS6},  // 25  matrix[1][11]
    {0, SW4_CS16,  SW4_CS17,  SW4_CS18},  // 26  matrix[1][12]
    {0, SW5_CS16,  SW5_CS17,  SW5_CS18},  // 27  matrix[1][13]
    {0, SW1_CS7,   SW1_CS8,   SW1_CS9},   // 28  matrix[2][0]
    {0, SW2_CS7,   SW2_CS8,   SW2_CS9},   // 29  matrix[2][1]
    {0, SW3_CS7,   SW3_CS8,   SW3_CS9},   // 30  matrix[2][2]
    {0, SW4_CS7,   SW4_CS8,   SW4_CS9},   // 31  matrix[2][3]
    {0, SW5_CS7,   SW5_CS8,   SW5_CS9},   // 32  matrix[2][4]
    {0, SW6_CS7,   SW6_CS8,   SW6_CS9},   // 33  matrix[2][5]
    {0, SW7_CS7,   SW7_CS8,   SW7_CS9},   // 34  matrix[2][6]
    {0, SW8_CS7,   SW8_CS8,   SW8_CS9},   // 35  matrix[2][7]
    {0, SW9_CS7,   SW9_CS8,   SW9_CS9},   // 36  matrix[2][8]
    {0, SW10_CS7,  SW10_CS8,  SW10_CS9},  // 37  matrix[2][9]
    {0, SW11_CS7,  SW11_CS8,  SW11_CS9},  // 38  matrix[2][10]
    {0, SW12_CS7,  SW12_CS8,  SW12_CS9},  // 39  matrix[2][11]
    {0, SW7_CS16,  SW7_CS17,  SW7_CS18},  // 40  matrix[2][12]
    {0, SW1_CS10,  SW1_CS11,  SW1_CS12},  // 41  matrix[3][0]
    {0, SW3_CS10,  SW3_CS11,  SW3_CS12},  // 42  matrix[3][2]
    {0, SW4_CS10,  SW4_CS11,  SW4_CS12},  // 43  matrix[3][3]
    {0, SW5_CS10,  SW5_CS11,  SW5_CS12},  // 44  matrix[3][4]
    {0, SW6_CS10,  SW6_CS11,  SW6_CS12},  // 45  matrix[3][5]
    {0, SW7_CS10,  SW7_CS11,  SW7_CS12},  // 46  matrix[3][6]
    {0, SW8_CS10,  SW8_CS11,  SW8_CS12},  // 47  matrix[3][7]
    {0, SW9_CS10,  SW9_CS11,  SW9_CS12},  // 48  matrix[3][8]
    {0, SW10_CS10, SW10_CS11, SW10_CS12}, // 49  matrix[3][9]
    {0, SW11_CS10, SW11_CS11, SW11_CS12}, // 50  matrix[3][10]
    {0, SW12_CS10, SW12_CS11, SW12_CS12}, // 51  matrix[3][11]
    {0, SW8_CS16,  SW8_CS17,  SW8_CS18},  // 52  matrix[3][12]
    {0, SW1_CS13,  SW1_CS14,  SW1_CS15},  // 53  matrix[4][0]
    {0, SW3_CS13,  SW3_CS14,  SW3_CS15},  // 54  matrix[4][2]
    {0, SW4_CS13,  SW4_CS14,  SW4_CS15},  // 55  matrix[4][3]
    {0, SW7_CS13,  SW7_CS14,  SW7_CS15},  // 56  matrix[4][6]
    {0, SW10_CS13, SW10_CS14, SW10_CS15}, // 57  matrix[4][9]
    {0, SW11_CS13, SW11_CS14, SW11_CS15}, // 58  matrix[4][10]
    {0, SW12_CS13, SW12_CS14, SW12_CS15}, // 59  matrix[4][11]
    {0, SW8_CS13,  SW8_CS14,  SW8_CS15},  // 60  matrix[4][12]
};

#endif
