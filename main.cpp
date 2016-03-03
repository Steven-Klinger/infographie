#include <iostream>
#include <cmath>
#include "tgaimage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <limits>
#include "Matrice.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor pink  = TGAColor(180, 15, 178,  147);

using namespace std;

vector<int> vectF;
vector<Vertex> vectV;
vector<Vertex> vectT;
vector<int> vectF2;
vector<Vertex> vectVN;

float* tabZ; //Tableau de profondeur
const Vertex lampe = Vertex(0,0,1); //Lampe en pleine face
const Vertex camera = Vertex(0,0,1);
TGAImage texture;

//Algorithme de Brensenham
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
  bool steep = false;
  if (std::abs(x0-x1)<std::abs(y0-y1)) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }
  if (x0>x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  int dx = x1-x0;
  int dy = y1-y0;
  int derror2 = std::abs(dy)*2;
  int error2 = 0;
  int y = y0;
  for (int x=x0; x<=x1; x++) {
    if (steep) {
      image.set(y, x, color);
    } else {
      image.set(x, y, color);
    }
    error2 += derror2;
    if (error2 > dx) {
      y += (y1>y0?1:-1);
      error2 -= dx*2;
    }
  }
}

void read(){

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int res2;
  float res3;
  float res4;
  float x,y,z,xt,yt,zt;

  fp = fopen("body.obj", "r"); //deuxième arg : Droit
  if (fp == NULL)
  exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {

    if(strchr(line, 'f') && !strchr(line, '#')){

      char *res = strtok(line+2, "/");
      int cnt = 1;
      res2 = atoi(res);
      vectF.push_back(res2-1);
      while (res = strtok(NULL, "/ ")) {
        if (0==cnt%3)
        {
          res2 = atoi(res);
          vectF.push_back(res2-1);
        }
        if(1==cnt%3){
          res2 = atoi(res);
          vectF2.push_back(res2-1);
	}
        cnt++;
      }
    }

    if(strchr(line, 'v') && !strchr(line, '#') && !strchr(line,'t')){
           if(!strchr(line, 'n')){
      char *res = strtok(line, " ");

      int cnt = 0;
      while (res = strtok(NULL, " ")) {
        res3 = atof(res);

        if(cnt == 0){
          x = (res3+1)*500; // +1 pour ramener à 0
        }
        if(cnt == 1){
          y = (res3+1)*500;
        }
        if(cnt == 2){
          z = (res3+1)*500;
        }

        cnt++;

      }
      vectV.push_back(Vertex(x,y,z));
    }
    }

    if(strchr(line, 'v') && !strchr(line, '#') && !strchr(line,'t')){
           if(strchr(line, 'n')){
      char *res = strtok(line, " ");

      int cnt = 0;
      while (res = strtok(NULL, " ")) {
        res3 = atof(res);

        if(cnt == 0){
          x = (res3+1)*500; // +1 pour ramener à 0
        }
        if(cnt == 1){
          y = (res3+1)*500;
        }
        if(cnt == 2){
          z = (res3+1)*500;
        }

        cnt++;

      }
      vectVN.push_back(Vertex(x,y,z));
    }
    }
    
    if(!strchr(line, '#') && strchr(line,'t')){
           if(!strchr(line, 'n')){

      char *res = strtok(line, " ");

      int cnt = 0;
      while (res = strtok(NULL, " ")) {
        res4 = atof(res);

        if(cnt == 0){
          xt = (res4);
        }
        if(cnt == 1){
          yt = (res4);
        }
        if(cnt == 2){
          zt = (res4);
        }

        cnt++;
      }
      vectT.push_back(Vertex(xt,yt,zt));
    }
    
  }
  }

  fclose(fp);
  if (line)
    free(line);

}

Vertex barycentre(Vertex v1, Vertex v2, Vertex v3, int pointX, int pointY){

  Vertex vt1;
  vt1.x = v3.x-v1.x;
  vt1.y = v2.x-v1.x;
  vt1.z = v1.x-pointX;

  Vertex vt2;
  vt2.x = v3.y-v1.y;
  vt2.y = v2.y-v1.y;
  vt2.z = v1.y-pointY;

  Vertex produit = produit_vectoriel(vt1,vt2);
  float xt,yt,zt;
  xt = 1. - (produit.x+produit.y)/produit.z;
  yt = produit.y/produit.z;
  zt = produit.x/produit.z;

  produit.x = xt;
  produit.y = yt;
  produit.z = zt;

  return produit;


}

void remplir_Triangle(Vertex v1, Vertex v2, Vertex v3, TGAImage &image, TGAImage &texture, Vertex vtex1, Vertex vtex2, Vertex vtex3){

  int i;
  int j;
  i = min(min(v1.x,v2.x),v3.x);
  //j = min(min(v1.y,v2.y),v3.y); //Ne marche pas pour des raisons inconnues. La vérité est ailleurs.
  int maxI = max(max(v1.x,v2.x),v3.x);
  int maxJ = max(max(v1.y,v2.y),v3.y);

  //cout << v1.y << " " << v2.y << " " << v3.y << "  J: " << j << " MAXJ: " << maxJ << endl;
  //Calcul pour la lumière

  Vertex vt1;
  vt1.x = v1.x-v3.x;
  vt1.y = v1.y-v3.y;
  vt1.z = v1.z-v3.z;

  Vertex vt2;
  vt2.x = v2.x-v3.x;
  vt2.y = v2.y-v3.y;
  vt2.z = v2.z-v3.z;

  Vertex vecteur_normal = produit_vectoriel(vt1,vt2);
  vecteur_normal.normalisation(); //Pour avoir les valeurs entre 0 et 1 pour ne pas dépasser 255 par rapport à la couleur

  float lumiere = abs(lampe.x*vecteur_normal.x + lampe.y*vecteur_normal.y + lampe.z*vecteur_normal.z);
  TGAColor couleur;

  for(; i<=maxI; i++){

    for(j=0; j<=maxJ; j++){  //Ne marche pas pour des raisons inconnues. La vérité est ailleurs. Donc j=0

      Vertex bary = barycentre(v1,v2,v3,i,j);

      if(bary.x >= 0 && bary.y >=0 && bary.z >=0){

        if(bary.x*v1.z+bary.y*v2.z+bary.z*v3.z > tabZ[i+(image.get_width()*j)]){ //Vérifie si le pixel est devant l'ancien dessiné

            int pix_x = ((vtex1.x*bary.x) + (vtex2.x*bary.y) + (vtex3.x*bary.z)) *texture.get_width();
            int pix_y = ((vtex1.y*bary.x) + (vtex2.y*bary.y) + (vtex3.y*bary.z)) *texture.get_height();

            couleur = texture.get(pix_x,pix_y);
	
            couleur.r *= lumiere; /*Luminosité sur les textures*/
            couleur.g *= lumiere;
            couleur.b *= lumiere;

            tabZ[i+(image.get_width()*j)] = bary.x*v1.z+bary.y*v2.z+bary.z*v3.z; //Enregistrer la nouvelle profondeur
            image.set(i,j,couleur);

      }

    }

  }

}
}

void write(TGAImage &image){

  for(int i = 0; i<vectF.size(); i+=3){
      remplir_Triangle(vectV[vectF[i]], vectV[vectF[i+1]] ,vectV[vectF[i+2]],image,texture,vectT[vectF2[i]], vectT[vectF2[i+1]], vectT[vectF2[i+2]]);
  }

}

int main(int argc, char** argv) {

  TGAImage image(1000, 1000, TGAImage::RGB);

  tabZ = (float*)malloc(image.get_height()*image.get_width()*sizeof(float));

  for(int i = 0; i<image.get_height()*image.get_width(); i++){ //Initialisation du tableau profondeur
    tabZ[i] = -numeric_limits<float>::max();
  }

  read();
  texture.read_tga_file("obj/body_diffuse.tga"); //lire texture
  texture.flip_vertically();

  write(image);

  image.flip_vertically();
  image.write_tga_file("rempli.tga");

  /*Matrice44 lol;
  Vertex lol2;
  lol2.x = 4;
  lol2.y = 7;
  lol2.z = 98;

  Vertex swag = lol*lol2;

  cout << swag.x << " " << swag.y << " " << swag.z << " " << endl;*/



  return 0;
}
