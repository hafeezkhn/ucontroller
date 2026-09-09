#include<stdio.h>
unsigned int convert_little_to_big(unsigned int val){
    //extract each byte and shift it to opposite end
    return ((val & 0x000000FF) << 24) | //move byte 0 to byte 3
           ((val & 0x0000FF00) << 8 ) | // move byte 1 to byte 2
           ((val & 0x00FF0000) >> 8 ) |
           ((val & 0xFF000000) >> 24);
    }

int main(){

    unsigned int little_endian_val = 0x12345678;
    unsigned int big_endinan_val = convert_little_to_big(little_endian_val);

    printf("original = 0x %08x\n",little_endian_val);
    printf("converted = 0x %08x\n",big_endinan_val);

    return 0;
}