//
// Created by luca on 05/04/24.
//

#include "utils.h"

int findChar(char *src, char toFind, unsigned int src_len){
    for(int i = 0; i < src_len; ++i){
        if (src[i] == toFind){
            return i;
        }
    }
    return -1;
}