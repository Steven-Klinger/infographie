class Vertex{
  
public:
  float x,y,z;
  
 Vertex(float x, float y, float z):x(x),y(y),z(z){}
  Vertex(){}

  void normalisation(){
    float norme = sqrt(x*x + y*y +z*z);

    x /= norme;
    y /= norme;
    z /= norme;
    
  }
  
};

Vertex produit_vectoriel(Vertex v1, Vertex v2){
    Vertex produit;
    produit.x = (v1.y*v2.z-v1.z*v2.y);
    produit.y = -(v1.x*v2.z-v1.z*v2.x);
    produit.z = (v1.x*v2.y-v1.y*v2.x);

    return produit;
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

Vertex operator*(const Vertex &v1,const Vertex &v2){

    Vertex res;
    res.x = v1.x * v2.x;
    res.y = v1.y * v2.y;
    res.z = v1.z * v2.z;

    return res;

}


Vertex operator+(const Vertex &v1,const Vertex &v2){

    Vertex res;
    res.x = v1.x + v2.x;
    res.y = v1.y + v2.y;
    res.z = v1.z + v2.z;

    return res;

}


Vertex operator+(const Vertex &v1,const float &f){

    Vertex res;
    res.x = v1.x + f;
    res.y = v1.y + f;
    res.z = v1.z + f;

    return res;

}

Vertex operator-(const Vertex &v1,const Vertex &v2){

    Vertex res;
    res.x = v1.x - v2.x;
    res.y = v1.y - v2.y;
    res.z = v1.z - v2.z;

    return res;

}

Vertex operator*(const float &f,const Vertex &v1){

    Vertex res;
    res.x = v1.x * f;
    res.y = v1.y * f;
    res.z = v1.z * f;

    return res;

}

Vertex operator*(const Vertex &v1, const float &f){

    Vertex res;
    res.x = v1.x * f;
    res.y = v1.y * f;
    res.z = v1.z * f;

    return res;

}