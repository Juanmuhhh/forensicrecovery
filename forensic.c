#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //Prompts user to put in one line argument
    if (argc > 2)
    {
        printf("Incorrect usage. Must look like: ./recover card.raw\n");
        return 1;
    }


    char filename[9];
    int indx = 0;
    int block_size = 512;
    BYTE buffer[block_size];

    //Opens memory card which is typically just used as "./recover card.raw" in Read mode
    FILE *file = fopen(argv[1], "r");
    //Will let user know they cannot open image
    if (file == NULL)
    {
        printf("Cannot open this image\n");
        return 1;
    }

    //Read 512 bytes into BYTE buffer. Repeats until until end of card
    while (fread(buffer, sizeof(BYTE), block_size, file) == block_size)
    {
        //Looks through beginning 4 BYTES of the data to see if it matches the beginning of a new JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0))
        {
            sprintf(filename, "%03i.jpg", indx);
            FILE *img = fopen(filename, "w");
            fwrite(buffer, sizeof(BYTE), block_size, img);
            indx++;
            fclose(img);
        }
        else if (indx > 0)
        {
            FILE *img = fopen(filename, "a");
            fwrite(buffer, sizeof(BYTE), block_size, img);
            fclose(img);
        }

    }
    //Close remaining files
    fclose(file);
}
