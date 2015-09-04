#include "post3D/Color.h"
#include <algorithm>

using namespace std;
namespace math3D{
Color::Color(float r , float g , float b , float a ){
  this->r = max(min(r,1.0f),0.0f);
  this->g = max(min(g,1.0f),0.0f);
  this->b = max(min(b,1.0f),0.0f);
  this->a = max(min(a,1.0f),0.0f);
}

Color& Color::setRGBA255(int r , int g , int b , int a ){
  this->r = max(min(r,255),0)/255.0f;
  this->g = max(min(g,255),0)/255.0f;
  this->b = max(min(b,255),0)/255.0f;
  this->a = max(min(a,255),0)/255.0f;
  return *this;
}

Color& Color::setRGBA(float r , float g , float b , float a ){
  //Looks the same as the constructor, maybe it can be removed
  this->r = max(min(r,1.0f),0.0f);
  this->g = max(min(g,1.0f),0.0f);
  this->b = max(min(b,1.0f),0.0f);
  this->a = max(min(a,1.0f),0.0f);
  return *this;
}

Color& Color::setComponent(const char component, float value){
  switch(component){
      case 'r':
          this->r = value;
          break;
      case 'g':
          this->g = value;
          break;
      case 'b':
          this->b = value;
          break;
      case 'a':
          this->a = value;
          break; 
  }
  return *this;
}

Color& Color::operator+=(const Color& color){
  this->r = fmin(this->r + color.r,1.0f);
  this->g = fmin(this->g + color.g,1.0f);
  this->b = fmin(this->b + color.b,1.0f);
  this->a = fmin(this->a + color.a,1.0f);
  return *this;
}

Color& Color::operator-=(const Color& color){
  this->r = fmax(this->r - color.r,0.0f);
  this->g = fmax(this->g - color.g,0.0f);
  this->b = fmax(this->b - color.b,0.0f);
  this->a = fmax(this->a - color.a,0.0f);
  return *this;
}

bool Color::operator==(const Color& color)const{
  bool result = 
      this->r == color.r && 
      this->g == color.g && 
      this->b == color.b && 
      this->a == color.a;
  return result;
}

std::unique_ptr<float[]> Color::getAsArray() const{
  //float col[4];
  std::unique_ptr<float[]> col{new float[4]};
  col[0] = this->r;
  col[1] = this->g;
  col[2] = this->b;
  col[3] = this->a;
  return col;
}

float Color::getComponent(const char component) const{
  switch(component){
      case 'r':
          return this->r;
      case 'g':
          return this->g;
      case 'b':
          return this->b;
  }
  return 0;//should throw an exception
}

Color operator+(const Color& c1,const Color& c2){
  Color result {c1};
  return result += c2;
}

Color operator-(const Color& c1,const Color& c2){
  Color result {c1};
  return result += c2;
}

}