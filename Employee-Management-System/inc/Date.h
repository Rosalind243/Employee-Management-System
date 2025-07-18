
#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

/* 
   In C programming language, (using gcc compiler), struct is limited to "grouping"
   >= 1 data members of similar / different data types only!
   
   In C++ programming language, (using g++ compiler), struct can, (additionally) posses 
   constructors, destructors, inheritance, method overriding, polymorphism, etc!
   Note : Without using access specifiers, struct members/methods access are PUBLIC by DEFAULT !
   
   Below struct definition must be compiled using g++ compiler !
*/

struct Date 
{
    int day;    // range from 1-31
    int month;  // range from 1-12
    int year;   

    // constructor for Date struct obj
    Date(int d = 1, int m = 1, int y = 2000);

    std::string toString() const;
};

typedef struct Date DateType;

#endif