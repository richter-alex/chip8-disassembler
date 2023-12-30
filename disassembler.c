#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
    FILE *rom_file = fopen("./pong.rom", "rb");
    fseek(rom_file, 0, SEEK_END);

    unsigned int size = ftell(rom_file);
    uint8_t rom_bytes[size];

    rewind(rom_file);

    for (int i = 0; i < size; i++) {
        fread(&rom_bytes[i], 1, 1, rom_file);
    }

    fclose(rom_file);

    for (int i = 0; i < size;) {
        for (int j = 0; j < 8; j++) {
            if (i >= size)
                break;
            printf("%02X%02X ", rom_bytes[i], rom_bytes[i+1]);
            i += 2;
        }
        if (i >= size)
            break;

        printf("\n");
    }
}
