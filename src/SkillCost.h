#ifndef SKILL_COST_H_
#define SKILL_COST_H_

#include <map>

#include "SkillIds.h"


std::map<int,int> fill_skill_cost();
const std::map<int,int> skill_cost = fill_skill_cost();

#endif