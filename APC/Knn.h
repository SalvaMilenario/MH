/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Knn.h
 * Author: salva
 *
 * Created on April 4, 2017, 11:55 AM
 */

#ifndef KNN_H
#define KNN_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

class Knn {
public:
    Knn(char* archivo);
    Knn(const Knn& orig);
    virtual ~Knn();
private:
    vector<vector<float> > datos;
    int posClase;

};

#endif /* KNN_H */

