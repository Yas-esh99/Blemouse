#include <stdio.h>
#include <BleMouse.h>
#include <cstdlib>

#define X_PIN 34
#define Y_PIN 33

BleMouse blemouse;

int maxSpeed = 1;
int xoffset = -38;
int yoffset = -37;
int xBuffer[20];
int yBuffer[20];
int xvalue;
int yvalue;

void setup() {
    Serial.begin(115200);
    blemouse.begin();
    delay(1000);
}

// Function that returns the most frequently occurring value in an array.
int getMostRepeatedValue(int values[], int size) {
    int maxCount = 0;
    int mostRepeated = values[0];  // Initialize with first value

    for (int i = 0; i < size; i++) {
        int count = 0;
        // Count how many times values[i] occurs in the array
        for (int j = 0; j < size; j++) {
            if (values[i] == values[j])
                count++;
        }
        // Update if the current value has a higher frequency
        if (count > maxCount) {
            maxCount = count;
            mostRepeated = values[i];
        }
    }
    return mostRepeated;
}

// Function to fill buffer with readings and return most repeated value
int getStabilizedValue(int pin, int buffer[], int size) {
    // Fill the buffer with new readings
    for (int i = 0; i < size; i++) {
        buffer[i] = analogRead(pin);
    }
    
    // Get the most repeated value from the buffer
    return getMostRepeatedValue(buffer, size);
}

void loop() {
    if (blemouse.isConnected()) {
        // Get stabilized X value
        int rawXValue = getStabilizedValue(X_PIN, xBuffer, 20);
        xvalue = (int) round(rawXValue / 50) + xoffset;
        
        // Get stabilized Y value
        int rawYValue = getStabilizedValue(Y_PIN, yBuffer, 20);
        yvalue = (int) round(rawYValue / 50) + yoffset;
        
        Serial.print("X: ");
        Serial.print(xvalue);
        Serial.print(" Y: ");
        Serial.println(yvalue);
        
        blemouse.move(xvalue, yvalue);
        delay(50);
    }
}
