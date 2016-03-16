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