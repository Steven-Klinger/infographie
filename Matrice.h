#include "vertex.h"
#include <cstdio>
#include <iostream>

using namespace std;

class Matrice44{

public:

    float tab[4][4];

    Matrice44():tab{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}{}

    Vertex operator*(const Vertex &vertex){ //multiplier une matrice par un vertex

        float tab2[4];
        Vertex res;


        for(int i =0; i<4; i++){

           tab2[i] = tab[i][0]*(vertex.x) + tab[i][1]*(vertex.y) + tab[i][2]*(vertex.z) + tab[i][3];
        }

        res.x = tab2[0] / tab2[3];
        res.y = tab2[1] / tab2[3];
        res.z = tab2[2] / tab2[3];


        return res;

    }

    Matrice44 operator*(Matrice44 m){

         Matrice44 res;

         for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                res.tab[i][j] = 0;
                for(int k = 0; k<4; k++){

                    res.tab[i][j] += tab[i][k] * m.tab[k][j];

                }


            }


         }
            return res;

    }

    void identity(){

        for(int i=0; i<4; i++){

                tab[i][i] = 1;


        }

    }

    void showM(){
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                cout << tab[i][j] << (j<3 ? " | " : "");
            }
            cout << endl;
        }
/*
        cout << tab[0][0] << " | " << tab[0][1] << " | " << tab[0][2] << " | " << tab[0][3] << endl;
        cout << tab[1][0] << " | " << tab[1][1] << " | " << tab[1][2] << " | " << tab[1][3] << endl;
        cout << tab[2][0] << " | " << tab[2][1] << " | " << tab[2][2] << " | " << tab[2][3] << endl;
        cout << tab[3][0] << " | " << tab[3][1] << " | " << tab[3][2] << " | " << tab[3][3] << endl;
*/
    }

    void setM(int ligne, int colonne, float valeur){

        tab[ligne][colonne] = valeur;

    }
};
