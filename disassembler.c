#include <stdio.h>
#include <stdint.h>

int main(int argc, char **argv) {
    if (argv[1] == NULL) {
        printf("ROM file required as argument.\n");
        return -1;
    }

    char *filename = argv[1];
    FILE *rom_file = fopen(filename, "rb");
    fseek(rom_file, 0, SEEK_END);

    unsigned int size = ftell(rom_file);
    uint8_t rom_bytes[size];

    uint16_t address = 0x0000;

    rewind(rom_file);

    for (int i = 0; i < size; i++) {
        fread(&rom_bytes[i], 1, 1, rom_file);
    }

    fclose(rom_file);

    for (int i = 0; i < size; i+= 2) {
        uint16_t instruction = (uint16_t)rom_bytes[i] << 8 | rom_bytes[i+1];
        printf("%04X\t", address);
        printf("%04X\t", instruction);
        switch ((instruction & 0xF000) >> 12) {
            case 0x0:
                switch (instruction & 0x0FFF) {
                    case 0x0E0:
                        printf("Clear screen\n");
                        break;
                    case 0x0EE:
                        printf("Return\n");
                        break;
                    case 0x000:
                        printf("NO-OP\n");
                        break;
                    default:
                        printf("COSMAC Call %03X\n", instruction & 0x0FFF);
                        break;
                }
                break;
            case 0x1:
                printf("JMP %03X\n", instruction & 0x0FFF);
                break;
            case 0x2:
                printf("Call %03X\n", instruction & 0x0FFF);
                break;
            case 0x3:
                printf("SE V[%X], %02X\n", (instruction & 0x0F00) >> 8, instruction & 0x00FF);
                break;
            case 0x4:
                printf("SNE V[%X], %02X\n", (instruction & 0x0F00) >> 8, instruction & 0x00FF);
                break;
            case 0x5:
                printf("SE V[%X], V[%X]\n", instruction & 0x0F00, instruction & 0x0F00);
                break;
            case 0x6:
                printf("MOV V[%X], %02X\n", (instruction & 0x0F00) >> 8, instruction & 0x00FF);
                break;
            case 0x7:
                printf("ADD V[%X] %02X\n", instruction & 0x0F00, instruction & 0x00FF);
                break;
            case 0x8:
                switch (instruction & 0x000F) {
                    case 0x0:
                        printf("MOV V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0x1:
                        printf("OREQ V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0x2:
                        printf("ANDEQ V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0x3:
                        printf("XOREQ V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0x4:
                        printf("ADD V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0x5:
                        printf("SUB V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0x6:
                        printf("RSHIFT V[%X]\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x7:
                        printf("SUBEQ V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                        break;
                    case 0xE:
                        printf("LSHIFT V[%X]\n", (instruction & 0x0F00) >> 8);
                        break;
                }
                break;
            case 0x9:
                printf("SNE V[%X], V[%X]\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4);
                break;
            case 0xA:
                printf("MOV I, %03X\n", instruction & 0x0FFF);
                break;
            case 0xB:
                printf("JMP %03X; Plus V[0]\n", instruction & 0x0FFF);
                break;
            case 0xC:
                printf("ANDRAND V[%X], %02X\n", (instruction & 0x0F00) >> 8, instruction & 0x00FF);
                break;
            case 0xD:
                printf("DRAW V[%X], V[%X], %X\n", (instruction & 0x0F00) >> 8, (instruction & 0x00F0) >> 4, instruction & 0x000F);
                break;
            case 0xE:
                switch (instruction & 0x00FF) {
                    case 0x9E:
                        printf("JKEQ V[%X], KEY\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0xA1:
                        printf("JKNEQ V[%X], KEY\n", (instruction & 0x0F00) >> 8);
                        break;
                }
                break;
            case 0xF:
                switch (instruction & 0x00FF) {
                    case 0x07:
                        printf("MOV V[%X], DTIMER\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x0A:
                        printf("GETKEY V[%X], KEY\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x15:
                        printf("MOV DTIMER, V[%X]\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x18:
                        printf("MOV STIMER, V[%X]\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x1E:
                        printf("ADD I, V[%X]\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x29:
                        printf("MOV I, SPRITELOC(V[%X])\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x33:
                        printf("MOV I, BCD(V[%X])\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x55:
                        printf("REGDMP V[%X])\n", (instruction & 0x0F00) >> 8);
                        break;
                    case 0x65:
                        printf("REGFILL V[%X]\n", (instruction & 0x0F00) >> 8);
                        break;
                }
                break;
        }
        address += 2;
    }
}
