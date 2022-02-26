#ifndef CLASS_H
#define CLASS_H

#include "ClassIds.h"
#include <string>
#include <vector>

using namespace std;

struct class_data
{
    string name;
    string lname;
    string abbrev;
    bool playable;
    int8_t min_str;
    int8_t min_dex;
    int8_t min_con;
    int8_t min_int;
    int8_t min_wis;
};

typedef vector<struct class_data> classes_t;

extern classes_t classes;

#endif