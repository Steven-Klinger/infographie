#include <iostream>
#include <cmath>
#include "tgaimage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "vertex.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor pink= TGAColor(180, 15, 178, 147);

using namespace std;

vector<int> vectF;
vector<Vertex> vectV;

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
	
        fp = fopen("african_head.obj", "r"); //deuxième arg : Droit
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
				    x = res3;
				  }
				  if(cnt == 1){
				    y = res3;
				  }
				  if(cnt == 2){			    
				    z = res3;
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

void remplir_Triangle(int x1, int y1, int x2, int y2, int x3, int y3){
  
  TGAImage image(1000, 1000, TGAImage::RGB);
  
  line(x1,y1,x2,y2,image,pink);
  line(x2,y2,x3,y3,image,pink);
  line(x3,y3,x1,y1,image,pink);
  int i;
  int j;
  for(i =0; i<1000; i++){
    
    for(j=0; j<1000; j++){
	
      if(((x1-i)*(y2-j))-((y1-j)*(x2-i)) >= 0 && (((x2-i)*(y3-j))-((y2-j)*(x3-i))) >= 0 && (((x3-i)*(y1-j))-((y3-j)*(x1-i))) >= 0 
	|| (((x1-i)*(y2-j))-((y1-j)*(x2-i)) <= 0 && (((x2-i)*(y3-j))-((y2-j)*(x3-i))) <= 0 && (((x3-i)*(y1-j))-((y3-j)*(x1-i))) <= 0)){
	
	image.set(i,j,pink);
      
    }

  }
  
  }
	image.write_tga_file("triangle_rempli.tga");
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
}

void write(TGAImage &image){
  

  for(int i = 0; i<vectF.size(); i+=3){
  
      line(vectV[vectF[i+1]].x*450+500, vectV[vectF[i+1]].y*450+500, vectV[vectF[i]].x*450+500,vectV[vectF[i]].y*450+500,image,pink);
      line(vectV[vectF[i]].x*450+500, vectV[vectF[i]].y*450+500, vectV[vectF[i+2]].x*450+500,vectV[vectF[i+2]].y*450+500,image,pink);
      line(vectV[vectF[i+2]].x*450+500, vectV[vectF[i+2]].y*450+500, vectV[vectF[i+1]].x*450+500,vectV[vectF[i+1]].y*450+500,image,pink);
      
      remplir_Triangle(vectV[vectF[i+1]].x*450+500,vectV[vectF[i+1]].y*450+500,vectV[vectF[i]].x*450+500,vectV[vectF[i]].y*450+500,vectV[vectF[i+2]].x*450+500,vectV[vectF[i+1]].y*450+500);
  }

}

int main(int argc, char** argv) {
  
    TGAImage image(1000, 1000, TGAImage::RGB);
  
    read();
    write(image);
    
    //image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    //image.write_tga_file("wireframe.tga");
    return 0;
}