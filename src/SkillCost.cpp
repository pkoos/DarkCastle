
#include "SkillCost.h"

std::map<int,int> fill_skill_cost()
{
  std::map<int,int> skill_cost_map;
  skill_cost_map[SKILL_KICK] = 5;
  skill_cost_map[SKILL_TRIP] = 7;
  skill_cost_map[SKILL_RESCUE] = 10;
  skill_cost_map[SKILL_REDIRECT] = 5;
  skill_cost_map[SKILL_EAGLE_CLAW] = 12;
  skill_cost_map[SKILL_QUIVERING_PALM] = 45;
  skill_cost_map[SKILL_STUN] = 15;
  skill_cost_map[SKILL_BACKSTAB] = 4;
  skill_cost_map[SKILL_SNEAK] = 5;
  skill_cost_map[SKILL_HIDE] = 2;
  skill_cost_map[SKILL_HARM_TOUCH] = 30;
  skill_cost_map[SKILL_STALK] = 12;
  skill_cost_map[SKILL_POCKET] = 15;
  skill_cost_map[SKILL_STEAL] = 20;
  skill_cost_map[SKILL_CIRCLE] = 12;
  skill_cost_map[SKILL_DISARM] = 20;
  skill_cost_map[SKILL_VITAL_STRIKE] = 15;
  skill_cost_map[SKILL_EYEGOUGE] = 12;
  skill_cost_map[SKILL_JAB] = 4;
  skill_cost_map[SKILL_CRIPPLE] = 30;
  skill_cost_map[SKILL_PALM] = 2;
  skill_cost_map[SKILL_SLIP] = 2;
  skill_cost_map[SKILL_DECEIT] = 16;
  skill_cost_map[SKILL_APPRAISE] = 6;
  skill_cost_map[SKILL_PICK_LOCK] = 3;
  skill_cost_map[SKILL_ESCAPE] = 25;
  skill_cost_map[SKILL_LAY_HANDS] = 30;
  skill_cost_map[SKILL_BASH] = 10;
  skill_cost_map[SKILL_FOCUSED_REPELANCE] =45 ;
  skill_cost_map[SKILL_BLOOD_FURY] = 60;
  skill_cost_map[SKILL_CRAZED_ASSAULT] = 18;
  skill_cost_map[SKILL_RAGE] = 6;
  skill_cost_map[SKILL_FEROCITY] = 15;
  skill_cost_map[SKILL_BATTLECRY] = 24;
  skill_cost_map[SKILL_BERSERK] = 12;
  skill_cost_map[SKILL_HITALL] = 24;
  skill_cost_map[SKILL_BULLRUSH] = 20;
  skill_cost_map[SKILL_KNOCKBACK] = 24;
  skill_cost_map[SKILL_ARCHERY] = 2;
  skill_cost_map[SKILL_FORAGE] = 8;
  skill_cost_map[SKILL_TAME] = 15;
  skill_cost_map[SKILL_AMBUSH] = 24;
  skill_cost_map[SKILL_TRACK] = 3;
  skill_cost_map[SKILL_TACTICS] = 15;
  skill_cost_map[SKILL_RETREAT] = 15;
  skill_cost_map[SKILL_DEATHSTROKE] = 20;
  skill_cost_map[SKILL_BLADESHIELD] = 15;
  skill_cost_map[SKILL_GUARD] = 5;
  skill_cost_map[SKILL_PRIMAL_FURY] = 40;
  skill_cost_map[SKILL_HEADBUTT] = 15;
  skill_cost_map[SKILL_FREE_ANIMAL] = 25;
  skill_cost_map[SKILL_BEHEAD] = 25;
  skill_cost_map[SKILL_MAKE_CAMP] = 50;
  skill_cost_map[SKILL_ONSLAUGHT] = 50;
  skill_cost_map[SKILL_IMBUE] = 50;
  skill_cost_map[SKILL_BATTERBRACE] = 60;
  skill_cost_map[SKILL_TRIAGE] = 40;
  skill_cost_map[SKILL_BREW] = 100;
  skill_cost_map[SKILL_SCRIBE] = 100;
  skill_cost_map[SKILL_PURSUIT] = 20;
  return skill_cost_map;
}
