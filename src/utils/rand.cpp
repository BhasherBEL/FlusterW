//
// Created by Bhasher on 01/05/2020.
//

#include <random>
#include "rand.h"

std::string Rand::randString(int len) {
    std::string rv{};
    while (len > 0){
        int i = rand()%51+65;
        if(i>90) i+=7;
        rv.push_back((char) i);
        len--;
    }
    return rv;
}