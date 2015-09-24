#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <string>

namespace math3D{

const std::string CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
//Adapted from https://gist.github.com/ne-sachirou/882192
std::string generateUUID();
}
#endif