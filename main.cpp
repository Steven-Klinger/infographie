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
#include <cassert>
#include "Bresenham.h"

using namespace std;

vector<int> vectF;
vector<Vertex> vectV;
vector<Vertex> vectT;
vector<int> vectF2;
vector<int> vectF3;
vector<Vertex> vectVN;

float* tabZ; //Tableau de profondeur
const Vertex lampe = Vertex(0,1,1); //Lampe en pleine face
Matrice44 viewport;
TGAImage texture;
TGAImage nm; //normal mapping
TGAImage spec; //specular map
void read(){ //Lecture des .obj

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  int res2;
  float res3;
  float res4;  
  Matrice44 viewport;
  float x,y,z,xt,yt,zt;

  fp = fopen("diablo3_pose.obj", "r"); //deuxième arg : Droits
  if (fp == NULL)
  exit(EXIT_FAILURE);

  while ((read = getline(&line, &len, fp)) != -1) {

    if(strchr(line, 'f') && !strchr(line, '#')){ //Récupérer les faces

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
        if(2==cnt%3){
          res2 = atoi(res);
          vectF3.push_back(res2-1);
        }
        cnt++;
      }
    }

    if(strchr(line, 'v') && !strchr(line, '#') && !strchr(line,'t')){ //Récupérer les points (vertex)
           if(!strchr(line, 'n')){
      char *res = strtok(line, " ");

      int cnt = 0;
      while (res = strtok(NULL, " ")) {
        res3 = atof(res);

        if(cnt == 0){
          x = (res3); // +1 pour ramener à 0
        }
        if(cnt == 1){
          y = (res3);
        }
        if(cnt == 2){
          z = (res3);
        }

        cnt++;

      }
      vectV.push_back(Vertex(x,y,z));
    }
    }

    if(strchr(line, 'v') && !strchr(line, '#') && !strchr(line,'t')){ //Récupérer les coordonnées normales
           if(strchr(line, 'n')){
      char *res = strtok(line, " ");

      int cnt = 0;
      while (res = strtok(NULL, " ")) {
        res3 = atof(res);

        if(cnt == 0){
          x = (res3);
        }
        if(cnt == 1){
          y = (res3);
        }
        if(cnt == 2){
          z = (res3);
        }

        cnt++;

      }
      vectVN.push_back(Vertex(x,y,z));
    }
    }
    
    if(!strchr(line, '#') && strchr(line,'t')){ //Récupérer les coordonnées de texture
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
          zt = (0);
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

void remplir_Triangle(Vertex v1, Vertex v2, Vertex v3, TGAImage &image, TGAImage &texture, 
  Vertex vtex1, Vertex vtex2, Vertex vtex3, Vertex vn1, Vertex vn2, Vertex vn3, TGAImage &nm,TGAImage &spec,int angle){

  Vertex vt1;
  vt1 = v1 - v3;

  Vertex vt2;
  vt2 = v2 - v3;  

  double alpha = angle * M_PI/180; // angle de rotation

  Matrice44 rotation;
  rotation.identity();

  rotation.setM(0,0,cos(alpha));
  rotation.setM(0,2,-sin(alpha));
  rotation.setM(2,0,sin(alpha));
  rotation.setM(2,2,cos(alpha));

  v1 = viewport*rotation*v1;
  v2 = viewport*rotation*v2;
  v3 = viewport*rotation*v3;

  int i;
  int j;
  i = min(min(v1.x,v2.x),v3.x);
  //j = min(min(v1.y,v2.y),v3.y); //Ne marche pas pour des raisons inconnues. La vérité est ailleurs.
  int maxI = max(max(v1.x,v2.x),v3.x);
  int maxJ = max(max(v1.y,v2.y),v3.y);

  TGAColor couleur;
  TGAColor couleur_base;

  for(; i<=maxI; i++){

    for(j=0; j<=maxJ; j++){  // (; ...) Ne marche pas pour des raisons inconnues. La vérité est ailleurs. Donc j=0.

      Vertex bary = barycentre(v1,v2,v3,i,j);

      if(bary.x >= 0 && bary.y >=0 && bary.z >=0){

        if(bary.x*v1.z+bary.y*v2.z+bary.z*v3.z > tabZ[i+(image.get_width()*j)]){ //Vérifie si le pixel est devant l'ancien dessiné

            Vertex gouraud = bary.x * vn1 + bary.y * vn2 + bary.z * vn3; //gouraud

            int pix_x = ((vtex1.x*bary.x) + (vtex2.x*bary.y) + (vtex3.x*bary.z)) *nm.get_width();
            int pix_y = ((vtex1.y*bary.x) + (vtex2.y*bary.y) + (vtex3.y*bary.z)) *nm.get_height();

            couleur = (nm.get(pix_x,pix_y));

            Vertex normal_map; //normal map
            normal_map.x = couleur.b;
            normal_map.y = couleur.r;
            normal_map.z = couleur.g;

            normal_map = normal_map * gouraud;
            normal_map.normalisation();

            float lumiere = max((double)(.2 + lampe.x*normal_map.x + lampe.y*normal_map.y + lampe.z*normal_map.z),0.);

            Vertex specular = (normal_map*lampe)*2.f;
            specular = specular - lampe;

            TGAColor specC = spec.get(pix_x, pix_y);
            float specu = pow(max(specular.z,0.f), 10 + specC.b);

            lumiere += specu;

            couleur_base = (texture.get(pix_x, pix_y));
            couleur_base.r = min((double)couleur_base.r*lumiere, 255.);
            couleur_base.g = min((double)couleur_base.g*lumiere, 255.);
            couleur_base.b = min((double)couleur_base.b*lumiere, 255.);

            tabZ[i+(image.get_width()*j)] = bary.x*v1.z+bary.y*v2.z+bary.z*v3.z; //Enregistrer la nouvelle profondeur
            image.set(i,j,couleur_base);

      }

    }

  }

}
}

void write(TGAImage &image, int angle){

  for(int i = 0; i<vectF.size(); i+=3){
      remplir_Triangle(vectV[vectF[i]], vectV[vectF[i+1]] ,vectV[vectF[i+2]],image,texture,
        vectT[vectF2[i]], vectT[vectF2[i+1]], vectT[vectF2[i+2]], 
        vectVN[vectF3[i]],vectVN[vectF3[i+1]], vectVN[vectF3[i+2]],nm,spec,angle);
  }
  
}

void reglage(TGAImage &image){

viewport.identity();

  viewport.setM(0,3, image.get_width()/2.f);
  viewport.setM(1,3, image.get_height()/2.f);
  viewport.setM(2,3, 1);

  viewport.setM(0,0, image.get_width()/2.f);
  viewport.setM(1,1, image.get_height()/2.f);
  viewport.setM(2,2, 1);

  tabZ = (float*)malloc(image.get_height()*image.get_width()*sizeof(float)); //Z-Buffer

  for(int i = 0; i<image.get_height()*image.get_width(); i++){ //Initialisation du tableau profondeur
    tabZ[i] = -numeric_limits<float>::max();
  }

}


  int main(int argc, char** argv) {

  TGAImage image(1000, 1000, TGAImage::RGB);
  TGAImage image2(1000, 1000, TGAImage::RGB);
  TGAImage image3(1000, 1000, TGAImage::RGB);

  read();
  texture.read_tga_file("obj/diablo3_pose_diffuse.tga"); //lire texture
  texture.flip_vertically();
  nm.read_tga_file("obj/diablo3_pose_nm.tga"); //lire normal mapping
  nm.flip_vertically();
  spec.read_tga_file("obj/diablo3_pose_spec.tga");
  spec.flip_vertically();

  reglage(image);
  write(image,1);
  image.flip_vertically();
  image.write_tga_file("Rendu_Face.tga");

  reglage(image2);
  write(image2,45);
  image2.flip_vertically();
  image2.write_tga_file("Rendu_45.tga");
  
  reglage(image3);
  write(image3,90);
  image3.flip_vertically();
  image3.write_tga_file("Rendu_90.tga");

  return 0;
}
