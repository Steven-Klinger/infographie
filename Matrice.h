#include "vertex.h"
#include <stdio.h>

using namespace std;

class Matrice44{

public:

    float tab[4][4];

    Matrice44():tab{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}{}

    Vertex operator*(const Vertex &vertex){ //multiplier une matrice par un vertex

        float tab2[4];
        Vertex res;


        for(int i =0; i<4; i++){

           tab2[i] = tab[i][0]*(vertex.x) + tab[i][1]*(vertex.y) + tab[i][2]*(vertex.z) + tab[i][3];
            cout << tab2[i] << endl;
        }

        res.x = tab2[0] / tab2[3];
        res.y = tab2[1] / tab2[3];
        res.z = tab2[2] / tab2[3];

        return res;

    }



};
