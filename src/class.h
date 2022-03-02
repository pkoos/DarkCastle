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

struct class_skill_defines
{
    char * skillname;         // name of skill
    int16  skillnum;          // ID # of skill
    int16  levelavailable;    // what level class can get it
    int16  maximum;           // maximum value PC can train it to (1-100)
    uint8_t group;             // which class tree group it is assigned
    int16  attrs;	      // What attributes the skill is based on
};

typedef vector<struct class_data> classes_t;

extern classes_t classes;

#endif