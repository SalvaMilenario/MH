/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Knn.cpp
 * Author: salva
 * 
 * Created on April 4, 2017, 11:55 AM
 */

#include "Knn.h"
#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

Knn::Knn(char* archivo) {
    ifstream fichero;
    fichero.open(archivo);
    fichero.close();
}

Knn::Knn(const Knn& orig) {
}

Knn::~Knn() {
}

