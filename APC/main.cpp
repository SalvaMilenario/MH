/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: salva
 *
 * Created on April 4, 2017, 1:51 AM
 */

#include <cstdlib>
#include <iostream>
#include "Knn.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) 
{
    
    Knn sonar(argv[1]);
    Knn spambase(argv[2]);
    Knn wdbc(argv[3]);
    return 0;
}

