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
