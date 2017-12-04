// This program reads utf-8 characters from stdio, tries to convert
// them to code points and prints resulting code points to stdout,
// CR-separated.

#include <stdio.h>
#include <stdlib.h>

enum
{
    EXIT_UNEXPECTED_END_OF_INPUT = 1,
    EXIT_BAD_ENCODING = 2,
};

typedef struct
{
    int length;
    int bitmask;
    int bits;
} byte_meta_t;

// Global byte counter
int counter = 0;

int getc_or_die (int error_code)
{
    int byte = getc(stdin);
    counter++;
    if (byte == EOF)
    {
        exit(error_code);
    }
    else
    {
        return byte;
    }
}

byte_meta_t byte_meta (int byte)
// Calculates number of bytes of a character with first byte BYTE and
// a bitmask for extracting meaningful bits
{
    if ((0x80 & byte) == 0)         /* 0xxxxxxx */
    {
        return (byte_meta_t) { .length = 1, .bitmask = 0x7F, .bits = 7 };
    }
    else if ((0xE0 & byte) == 0xC0) /* 110xxxxx */
    {
        return (byte_meta_t) { .length = 2, .bitmask = 0x1F, .bits = 5 };
    }
    else if ((0xF0 & byte) == 0xE0) /* 1110xxxx */
    {
        return (byte_meta_t) { .length = 3, .bitmask = 0x0F, .bits = 4 };
    }
    else if ((0xF8 & byte) == 0xF0) /* 11110xxx */
    {
        return (byte_meta_t) { .length = 4, .bitmask = 0x07, .bits = 3 };
    }
    else if ((0xFC & byte) == 0xF8) /* 111110xx */
    {
        return (byte_meta_t) { .length = 5, .bitmask = 0x03, .bits = 2 };
    }
    else if ((0xFE & byte) == 0xFC) /* 1111110x */
    {
        return (byte_meta_t) { .length = 6, .bitmask = 0x01, .bits = 1 };
    }
}

int main (int argc, char *argv[])
{
    while (1)
    {
        int byte = getc_or_die(EXIT_SUCCESS);

        byte_meta_t meta = byte_meta(byte);

        // Now constructing code point from bytes
        unsigned int code_point = (meta.bitmask & byte);

        for (int i = 0; i < meta.length - 1; i++)
        {
            byte = getc_or_die(EXIT_UNEXPECTED_END_OF_INPUT);
            // Check if byte is well-coded
            if (0x80 != (0xC0 & byte))
            {
                fprintf(stderr, "Error: bad encoding at %d\n", counter);
                exit(EXIT_BAD_ENCODING);
            }
            // Only 6 lowest bits matter
            code_point <<= 6;
            code_point |= (byte & 0x3F);
        }

        printf("%u\n", code_point);
    }
}
