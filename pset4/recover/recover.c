#include <stdio.h>
#include <stdint.h>
#define BUFFER_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure proper use
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    char *input = argv[1];

    // open the file and check for validity
    FILE *inptr = fopen(input, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open the file.\n");
        return 1;
    }

    char filename[8];  //char arrays need an extra '\0' in the end

    // create buffer
    BYTE buffer[BUFFER_SIZE];

    // filename counter
    int filecount = 0;

    // output file pointer
    FILE* outptr = NULL;

    // go through cardfile until there aren't any blocks left
    while (fread(buffer, BUFFER_SIZE, 1, inptr) == 1)
    {
        // checking for the start of a jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xe0) == 0xe0)
        {
            if (outptr != NULL)
            {
                fclose(outptr);
            }

            sprintf(filename, "%03d.jpg", filecount);
            outptr = fopen(filename, "w");
            filecount++;
        }

        if (outptr != NULL)
        {
            // write 512 bytes to file once we start finding jpegs
            fwrite(&buffer, BUFFER_SIZE, 1, outptr);
        }

    }

    // close files
    fclose(inptr);
    fclose(outptr);
    return 0;
}