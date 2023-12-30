#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
    uint8_t rom_bytes[512]; // TODO: Make this more precise
    FILE *rom_file = fopen("./pong.rom", "rb");

    for (int i = 0; i <= 256; i++) {
        fread(&rom_bytes[i], 1, 1, rom_file);
    }

    for (int i = 0; i < 256/2; i+=2) {
        printf("%02X%02X\n", rom_bytes[i], rom_bytes[i+1]);
    }
}
