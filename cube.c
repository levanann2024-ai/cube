#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h> // dùng Sleep() nếu là Windows

float A = 0, B = 0;
float cubeWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
float distanceFromCam = 100;
float K1 = 40;

float sinA, cosA, sinB, cosB;

void calculatePoint(float x, float y, float z, char ch) {
    float x1 = x * cosB - z * sinB;
    float z1 = x * sinB + z * cosB;

    float y1 = y * cosA - z1 * sinA;
    float z2 = y * sinA + z1 * cosA;

    z2 += distanceFromCam;

    float ooz = 1 / z2;

    int xp = (int)(width / 2 + K1 * ooz * x1);
    int yp = (int)(height / 2 - K1 * ooz * y1);

    int idx = xp + yp * width;
    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {
    printf("\x1b[2J");
    while (1) {
        memset(buffer, ' ', width * height);
        memset(zBuffer, 0, width * height * sizeof(float));

        sinA = sin(A); cosA = cos(A);
        sinB = sin(B); cosB = cos(B);

        for (float x = -cubeWidth; x < cubeWidth; x += 2)
        for (float y = -cubeWidth; y < cubeWidth; y += 2) {
            calculatePoint(x, y, -cubeWidth, '#');
            calculatePoint(cubeWidth, y, x, '$');
            calculatePoint(-cubeWidth, y, -x, '~');
            calculatePoint(-x, y, cubeWidth, '@');
            calculatePoint(x, -cubeWidth, -y, '+');
            calculatePoint(x, cubeWidth, y, '*');
        }

        printf("\x1b[H");
        for (int i = 0; i < width * height; i++) {
            putchar(i % width ? buffer[i] : '\n');
        }
///this is useless
        A += 0.04;
        B += 0.02;

        usleep(16000); // Linux / macOS
        // Sleep(16);   // Windows
    }
    return 0;
}
