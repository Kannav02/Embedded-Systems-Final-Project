#include "Timer.h"
#include "mbed.h"
#include "MFRC522.h"
#include "DS1302.h"
#include "LCD.h"
#include "ThisThread.h"
#include <chrono>
#include <string>

#define BUZZER_PIN PTC10
// for communication with ESP32
BufferedSerial esp32Serial(PTC4, PTC3); 

// for getting the buzzer
DigitalOut buzzer(BUZZER_PIN);

// for getting the clock ready
DS1302 clk(PTB2, PTB3, PTB4);

// for getting the RFID reader ready
MFRC522 RfChip(PTD2, PTD3, PTD1, PTC11, PTD0);

// for getting the LCD ready
LCD lcd(D9, D8, D7, D6, D5, D4, LCD16x2); 


void clearSerialBuffer(BufferedSerial& serial) {
    char dummy;
    while (serial.readable()) {
        serial.read(&dummy, 1); // Read and discard the byte
    }
}

void setup(){
    // for rfid reader
    RfChip.PCD_Init();
    // ==========================================================

    // for clock 
    struct tm datetime;
    datetime.tm_sec = 0;        // Seconds
    datetime.tm_min = 00;       // Minutes
    datetime.tm_hour = 15;      // Hour (24-hour format)
    datetime.tm_mday = 7;      // Day of the month
    datetime.tm_mon = 4;        // Month (0-11)
    datetime.tm_year = 124;     // Year offset from 1900 (2024 - 1900)
    time_t timestamp = mktime(&datetime); 
    clk.set_time(timestamp); 
    // ==========================================================

    // for esp32
    esp32Serial.set_baud(9600); // Set baud rate to match ESP32 settings
    clearSerialBuffer(esp32Serial);
    // ==========================================================

    // for lcd
    lcd.cls(); // clear display
    lcd.locate(0, 0); // set cursor location
    lcd.printf("START\n"); // display text
    ThisThread::sleep_for(2s);
    lcd.cls(); // clear display
    // ==========================================================

    // for buzzer
    buzzer=0;
    // ==========================================================



}

int main()
{
    setup();
    while (true) {
        lcd.locate(0, 0); // set cursor location
        lcd.printf("SCAN YOUR CARD\n");
        if (!RfChip.PICC_IsNewCardPresent()) {
            ThisThread::sleep_for(100ms);
            continue;
        }
        if (!RfChip.PICC_ReadCardSerial()) {
            ThisThread::sleep_for(100ms);
            continue;
        }
        lcd.cls();
        ThisThread::sleep_for(100ms);
        lcd.locate(0, 0); // set cursor location
        lcd.printf("SCANNING\n");
        char uidString[32]; 
        int idx = 0; 
        time_t timeStamp = clk.time(NULL);
        for (uint8_t i = 0; i < RfChip.uid.size; i++) {

            idx += sprintf(&uidString[idx], "%02X", RfChip.uid.uidByte[i]);
        }
        uidString[idx] = '\0'; 
        printf("Card UID: %s\n", uidString);
        char timestampStr[32];
        struct tm *timeinfo = localtime(&timeStamp);
        strftime(timestampStr, sizeof(timestampStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        char dataToSend[64];
        sprintf(dataToSend, "(%s,%s)", uidString, timestampStr);
        
        esp32Serial.write(dataToSend, strlen(dataToSend));
        const int bufferSize = 256; 
        char buffer[bufferSize];
        memset(buffer, 0, bufferSize); 
        int bufferIndex = 0;
        ThisThread::sleep_for(1000ms); 
        while (!esp32Serial.readable()) {
            ThisThread::sleep_for(100ms); 
        }
        Timer time;
        time.start();
        while(time.elapsed_time()<=5s){
        if (esp32Serial.readable()) {
            char receivedChar;
            if (esp32Serial.read(&receivedChar, 1)) {
                if (receivedChar == '\n') { 
                    buffer[bufferIndex] = '\0'; 
                    break; 
                }
                buffer[bufferIndex++] = receivedChar;
            }
        }
        ThisThread::sleep_for(100ms);
    }
     clearSerialBuffer(esp32Serial); 


    
        
        if (buffer[0] == 'A') {
            printf("Acknowledgment received from ESP32\n");
            printf("%s\n",buffer);
            lcd.cls();
            ThisThread::sleep_for(100ms);
            lcd.locate(0,0);
            lcd.printf("Greetings!");
            lcd.locate(0, 1); // set cursor location
            lcd.printf("%s\n",&buffer[2]);
            buzzer=1;

            ThisThread::sleep_for(2s);

            lcd.cls();
            ThisThread::sleep_for(100ms);
            lcd.locate(0, 0); // set cursor location
            lcd.printf("SCAN YOUR CARD\n");
            buzzer=0;

            continue;
        } else {
            printf("Unexpected response received: %c\n", buffer[0]);


            lcd.cls();
            ThisThread::sleep_for(100ms);
            lcd.locate(0, 0); // set cursor location
            lcd.printf("Not Validated\n");

            buzzer=1;
            ThisThread::sleep_for(1s);
            buzzer=0;
            ThisThread::sleep_for(1s);
            buzzer=1;
            ThisThread::sleep_for(1s);
            buzzer=0;

            lcd.cls();
            ThisThread::sleep_for(100ms);
            lcd.locate(0, 0); // set cursor location
            lcd.printf("SCAN YOUR CARD\n");
            
        }

    }
}

