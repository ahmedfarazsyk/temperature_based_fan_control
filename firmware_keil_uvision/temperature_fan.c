#include <reg51.h> // Include register definition file for 8051 microcontroller

// Define data and control port aliases
#define MYDATA P3     // Port 3 used to read ADC data
#define ldata P1      // Port 1 connected to LCD data lines

// Define control pins using bit-addressable memory
sbit rd  = P2^5;      // ADC read control pin
sbit wr  = P2^4;      // ADC write control pin
sbit INTR = P2^3;     // ADC interrupt pin (signals conversion complete)
sbit rs  = P2^0;      // LCD Register Select (0: command, 1: data)
sbit rw  = P2^1;      // LCD Read/Write (0: write, 1: read)
sbit en  = P2^2;      // LCD Enable
sbit MTR = P2^6;      // Motor control pin (fan motor)

// Delay function (approx. 1ms per unit)
void msDelay(unsigned int value) {
    unsigned int x, y;
    for(x = 0; x < value; x++)
        for(y = 0; y < 1275; y++);
}

// Send command to LCD
void lcdCmd(unsigned char value) {
    ldata = value; // Put command on data lines
    rs = 0;         // Select command register
    rw = 0;         // Write operation
    en = 1;         // Enable high
    msDelay(5);     // Short delay
    en = 0;         // Enable low (command latched)
}

// Send character data to LCD
void lcdData(char value) {
    ldata = value; // Put data on data lines
    rs = 1;         // Select data register
    rw = 0;         // Write operation
    en = 1;
    msDelay(5);
    en = 0;
}

// Initialize LCD with required commands
void lcdInit() {
    lcdCmd(0x38); // 8-bit, 2-line display
    lcdCmd(0x0C); // Display ON, Cursor OFF
    lcdCmd(0x01); // Clear display
    lcdCmd(0x80); // Move cursor to beginning
}

// Display temperature value (2 digits + 'C') on LCD
void displayTemp(unsigned char value) {
    char d1 = (value / 10) + '0'; // Tens digit
    char d2 = (value % 10) + '0'; // Units digit
    lcdData(d1);
    lcdData(d2);
    lcdData('C');                // Display Celsius
}

// Update only numeric temperature on display (without 'C')
void updateTemp(unsigned char value) {
    char d1 = (value / 10) + '0';
    char d2 = (value % 10) + '0';
    lcdData(d1);
    lcdData(d2);
}

// Read value from ADC
unsigned char adcRead() {
    unsigned char value;
    wr = 0;        // Start ADC conversion
    wr = 1;
    while(INTR == 1); // Wait for conversion to complete (INTR low)
    rd = 0;        // Enable ADC data output
    value = MYDATA; // Read converted value
    rd = 1;        // Disable data output
    return value;
}

// Main control loop: read temperature, display, and control fan
void motorcontrol() {
    unsigned char i, value;
    unsigned char tempMsg[] = "TEMP: ";
    unsigned char fanOn[]   = "FAN ON ";
    unsigned char fanOff[]  = "FAN OFF";

    while(1) {
        value = adcRead(); // Read temperature value from ADC
        
        // Display "TEMP: " on first LCD line
        lcdCmd(0x80);      // Move cursor to first line
        for(i = 0; i < 6; i++) {
            lcdData(tempMsg[i]);
        }
        displayTemp(value); // Show temperature reading

        // Fan OFF: if temperature < 25°C
        if(value < 25) {
            MTR = 1;            // Fan OFF (assuming active-low control)
            lcdCmd(0xC0);       // Move to second line
            for(i = 0; i < 7; i++) {
                lcdData(fanOff[i]);
            }
        }

        // Fan ON: temperature = 25°C
        else {
            MTR = 0;            // Fan ON
            lcdCmd(0xC0);       // Move to second line
            for(i = 0; i < 7; i++) {
                lcdData(fanOn[i]);
            }

            // Fan speed modulation: low speed
            if(value >= 25 && value < 30) {
                while(value >= 25 && value < 30) {
                    MTR = 0; msDelay(50); // Fan ON
                    MTR = 1; msDelay(50); // Fan OFF
                    value = adcRead();
                    lcdCmd(0x86);         // Update temperature digits only
                    updateTemp(value);
                }
            }

            // Medium speed
            else if(value >= 30 && value < 35) {
                while(value >= 30 && value < 35) {
                    MTR = 0; msDelay(75); // Longer ON
                    MTR = 1; msDelay(25); // Shorter OFF
                    value = adcRead();
                    lcdCmd(0x86);
                    updateTemp(value);
                }
            }

            // Full speed
            else if(value >= 35) {
                while(value >= 35) {
                    MTR = 0;             // Constant ON
                    value = adcRead();
                    lcdCmd(0x86);
                    updateTemp(value);
                }
            }
        }

        msDelay(300); // Delay before next temperature check
    }
}

// Main function
void main() {
    P0 = 0xFF;        // Set Port 0 as input (if needed)
    MYDATA = 0xFF;    // Set Port 3 as input for ADC
    INTR = 1;         // Prepare for ADC interrupt
    rd = 1; wr = 1;   // Default high for ADC control
    MTR = 1;          // Fan OFF initially

    lcdInit();        // Initialize LCD

    // Optional welcome message
    lcdCmd(0x80);
    lcdData('W'); lcdData('E'); lcdData('L'); lcdData('C'); lcdData('O'); lcdData('M'); lcdData('E');
    msDelay(100);
    lcdCmd(0x01);     // Clear screen

    motorcontrol();   // Start fan and temperature control loop
}
