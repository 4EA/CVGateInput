/* Control Voltage (0-5V) Input with Gate (For Calibration)
 * Language:  C
 * Author:  Cole Jepson
 * Date:  8/04/2017
 * References: http://bytenoise.co.uk/CV-gate,
 * http://embeddedgurus.com/stack-overflow/2010/01/a-tutorial-on-lookup-tables-in-c/,
 * https://developer.mbed.org/forum/mbed/topic/2104/
 */

#include "mbed.h"

Serial pc(USBTX, USBRX);// setup serial connection for testing
AnalogIn ADC16(p16);    // CV input from CV keyboard/drum machine)
InterruptIn gate(p30);  // dynamic gate from CV keyboard/drum machine

int note_count = 0;     // declare note counter
float CV_in;            // declare ADC variable

// 0-5V in CV values for 5V logic level devices (arduino etc.)
/*static const float CV[61] =
{
    0, 0.083, 0.16, 0.25, 0.3, 0.416, 0.5, 0.583, 0.6, 0.75, 0.83, 0.916,
    1, 1.083, 1.16, 1.25, 1.3, 1.416, 1.5, 1.583, 1.6, 1.75, 1.83, 1.916,
    2, 2.083, 2.16, 2.25, 2.3, 2.416, 2.5, 2.583, 2.6, 2.75, 2.83, 2.916,
    3, 3.083, 3.16, 3.25, 3.3, 3.416, 3.5, 3.583, 3.6, 3.75, 3.83, 3.916,
    4, 4.083, 4.16, 4.25, 4.3, 4.416, 4.5, 4.583, 4.6, 4.75, 4.83, 4.916,
    5
}; */

// 0-3.3V in CV Values (divided from 0-5V) for 3.3V logic level devices (mbed)
static const float CV[61] =
{
    0, 0.054, 0.104, 0.165, 0.2, 0.2704, 0.325, 0.379, 0.39, 0.4875, 0.5395, 0.5954,
    0.65, 0.7039, 0.754, 0.82, 0.86, 0.9204, 0.975, 1.04, 1.055, 1.1375, 1.1895, 1.2454,
    1.3, 1.354, 1.404, 1.47, 1.5, 1.5704, 1.625, 1.689, 1.72, 1.7875, 1.8395, 1.905,
    1.96, 2.004, 2.054, 2.12, 2.17, 2.2204, 2.275, 2.35, 2.39, 2.4375, 2.4895, 2.5454,
    2.61, 2.654, 2.704, 2.78, 2.839, 2.89, 2.925, 2.99, 3.05, 3.0875, 3.1395, 3.1954,
    3.204
}; // B4 and C5 can't be reached with current hardware setup

// Note strings for calibration/testing
const char * const note[61] =
{
    "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
    "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
    "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",  
    "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3", 
    "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
    "C5" 
};

void open_gate()            // note on interrupt
{
    CV_in = ADC16.read();   // read CV on DIP16
    CV_in = CV_in*3.3;      // normalize input to 3.3V logic level
    note_count = 0;         // initialize counter to zero
    
    if(CV_in >= 3.2)        // with the current voltage divider setup, only 
    {                       // voltages less than 3.2V are supported 
        pc.printf("Unsupported CV range (>3.2V)\r\n");  // B4 and C5 unsupported
    }
    else
    {
        while(CV_in >= CV[note_count])  // check CV_in against CV lookup table
        {
            note_count++;   // increment CV lookup table index until the 
        }                   // the correct CV note is reached
        
        pc.printf("CV_in: %f\r\n", CV_in);  // display CV input
        pc.printf("note num: %d, note: %s\r\n", note_count-1, note[note_count-1]);  
    }   // display the note_count value (index) and the corresponding note string
}

void closed_gate()          // note off interrupt
{
    pc.printf("note off\r\n");  
}

int main() 
{
    gate.mode(PullNone);    // turn off pull up and pull down resistors
    gate.rise(&open_gate);  // setup interrupt for when the gate voltage is high
    gate.fall(&closed_gate);// setup interrupt for when the gate voltage is low
    
    while(1)
    {
        // wait indefinitely for a gate signal to trigger the interrupts
    }
}
