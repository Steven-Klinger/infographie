#include <iostream>
#include <cmath>
#include "tgaimage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "vertex.h"
#include <cstdlib>
#include <limits>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor pink= TGAColor(180, 15, 178, 147);

using namespace std;

vector<int> vectF;
vector<Vertex> vectV;
float tabZ[1000*1000]; //Tableau de profondeur 
const Vertex lampe = Vertex(0,0,1); //Lampe en pleine face

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
	float x,y,z;
	
        fp = fopen("diablo3_pose.obj", "r"); //deuxième arg : Droit
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
			cnt++;
			  
			}
			
		
		}
		
			
		if(strchr(line, 'v') && !strchr(line, '#') && !strchr(line,'t')){
				
			char *res = strtok(line, " ");

			int cnt = 0;
			while (res = strtok(NULL, " ")) {
				  res3 = atof(res);

				  if(cnt == 0){
				    x = (res3+1)*500;
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
	
	/*for (int i : vectF){
	  
	  cout << i << endl;
	}
	
	for(Vertex x: vectV){
	  
	  cout << x.x << " " << x.y << " " << x.z << endl;
	}*/

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

void remplir_Triangle(Vertex v1, Vertex v2, Vertex v3, TGAImage &image){
  
  int i;
  int j;
  i = min(min(v1.x,v2.x),v3.x);
  //j = min(min(v1.y,v2.y),v3.y); Ne marche pas pour des raisons inconnues. La vérité est ailleurs.
  int maxI = max(max(v1.x,v2.x),v3.x);
  int maxJ = max(max(v1.y,v2.y),v3.y);
  
  TGAColor random = TGAColor(rand()% 256, rand()% 256, rand()% 256, rand()% 256);
  
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
  TGAColor couleur = TGAColor(255*lumiere, 255*lumiere, 255*lumiere, 255*lumiere);
  
  for(; i<=maxI; i++){
    
    for(j = 0; j<=maxJ; j++){

	Vertex bary = barycentre(v1,v2,v3,i,j);
	
	if(bary.x >= 0 && bary.y >=0 && bary.z >=0){
		
		if(bary.x*v1.z+bary.y*v2.z+bary.z*v3.z > tabZ[i+(1000*j)]){ //Vérifie si le pixel est devant l'ancien dessiné
		  
		image.set(i,j,couleur);
		tabZ[i+(1000*j)] = bary.x*v1.z+bary.y*v2.z+bary.z*v3.z; //Enregistrer la nouvelle profondeur
		  
		}
      
    }

  }
  
  }
}

void write(TGAImage &image){
  
  for(int i = 0; i<vectF.size(); i+=3){
  
      remplir_Triangle(vectV[vectF[i+1]], vectV[vectF[i]] ,vectV[vectF[i+2]],image);
  }

}

int main(int argc, char** argv) {
  
    TGAImage image(1000, 1000, TGAImage::RGB);
    
    for(int i = 0; i<1000*1000; i++){ //Initialisation du tableau profondeur
      tabZ[i] = -numeric_limits<float>::max();
    }
      
    read();
    write(image);
	  
    image.flip_vertically();
    image.write_tga_file("rempli.tga");
    return 0;
}
