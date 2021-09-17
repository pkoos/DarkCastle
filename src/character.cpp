#include <QString>
#include "character.h"
#include "db.h" // exp_table
#include "levels.h"
#include "handler.h"

char_data::char_data()
{
    uuid = QUuid::createUuid();
    mobdata = nullptr;
    pcdata = nullptr;
    objdata = nullptr;
    desc = nullptr;        // NULL normally for mobs
    name = nullptr;        // Keyword 'kill X'
    short_desc = nullptr;  // Action 'X hits you.'
    long_desc = nullptr;   // For 'look room'
    description = nullptr; // For 'look mob'
    title = nullptr;

    sex = 0;
    c_class = 0;
    race = 0;
    level = 0;
    position = 0; // Standing, sitting, fighting

    str = 0;
    raw_str = 0;
    str_bonus = 0;
    intel = 0;
    raw_intel = 0;
    intel_bonus = 0;
    wis = 0;
    raw_wis = 0;
    wis_bonus = 0;
    dex = 0;
    raw_dex = 0;
    dex_bonus = 0;
    con = 0;
    raw_con = 0;
    con_bonus = 0;

    conditions[0] = 0; // Drunk full etc.
    conditions[1] = 0; // Drunk full etc.
    conditions[2] = 0; // Drunk full etc.

    weight = 0;        /* PC/NPC's weight */
    height = 0;        /* PC/NPC's height */

    hometown = 0; /* PC/NPC home town */
    gold = 0;     /* Money carried                           */
    plat = 0;     /* Platinum                                */
    exp = 0;      /* The experience of the player            */

    in_room = 0;

    immune = 0;  // Bitvector of damage types I'm immune to
    resist = 0;  // Bitvector of damage types I'm resistant to
    suscept = 0; // Bitvector of damage types I'm susceptible to
    memset(saves, 0, sizeof(saves));
    mana = 0;
    max_mana = 0; /* Not useable                             */
    raw_mana = 0; /* before int bonus                        */
    hit = 0;
    max_hit = 0; /* Max hit for NPC                         */
    raw_hit = 0; /* before con bonus                        */
    move = 0;
    raw_move = 0;
    max_move = 0; /* Max move for NPC                        */
    ki = 0;
    max_ki = 0;
    raw_ki = 0;

    alignment = 0; // +-1000 for alignments

    hpmetas = 0;   // total number of times meta'd hps
    manametas = 0; // total number of times meta'd mana
    movemetas = 0; // total number of times meta'd moves
    acmetas = 0;   // total number of times meta'd ac
    agemetas = 0;  // number of years age has been meta'd

    hit_regen = 0;  // modifier to hp regen
    mana_regen = 0; // modifier to mana regen
    move_regen = 0; // modifier to move regen
    ki_regen = 0;   // modifier to ki regen

    melee_mitigation = 0; // modifies melee damage
    spell_mitigation = 0; // modified spell damage
    song_mitigation = 0;  // modifies song damage
    spell_reflect = 0;

    clan = 0; /* Clan the char is in */

    armor = 0;   // Armor class
    hitroll = 0; // Any bonus or penalty to the hit roll
    damroll = 0; // Any bonus or penalty to the damage roll

    glow_factor = 0; // Amount that the character glows

    beacon = nullptr; /* pointer to my beacon */

    songs = std::vector<songInfo>(); // Song list
                                     //     int16 song_timer;       /* status for songs being sung */
                                     //     int16 song_number;      /* number of song being sung */
                                     //     char * song_data;        /* args for the songs */

    memset(equipment, 0, sizeof(equipment));

    skills.clear();   // Skills List
    skillsSaveLoadOrder = queue<int16>();
    affected = nullptr; // Affected by list
    carrying = nullptr; // Inventory List

    poison_amount = 0; // How much poison damage I'm taking every few seconds

    carry_weight = 0; // Carried weight
    carry_items = 0;  // Number of items carried

    hunting = nullptr; // Name of "track" target
    ambush = nullptr;  // Name of "ambush" target

    guarding = nullptr;   // Pointer to who I am guarding
    guarded_by = nullptr; // List of people guarding me

    memset(affected_by, 0, sizeof(affected_by)); // Quick reference bitvector for spell affects
    combat = 0;                                  // Bitvector for combat related flags (bash, stun, shock)
    misc = 0;                                    // Bitvector for IS_MOB/logs/channels.  So possessed mobs can channel

    fighting = nullptr;      /* Opponent     */
    next = nullptr;          /* Next anywhere in game */
    next_in_room = nullptr;  /* Next in room */
    next_fighting = nullptr; /* Next fighting */
    altar = nullptr;
    followers = nullptr;  /* List of followers */
    master = nullptr;     /* Who is char following? */
    group_name = nullptr; /* Name of group */

    timer = 0;           // Timer for update
    shotsthisround = 0;  // Arrows fired this round
    spellcraftglyph = 0; // Used for spellcraft glyphs
    changeLeadBonus = false;
    curLeadBonus = 0;
    cRooms = 0; // number of rooms consecrated/desecrated
    deaths = 0; /* deaths is reused for mobs as a
                                       timer to check for WAIT_STATE */

    cID = 0; // character ID

    timerAttached = nullptr;
    tempVariable = nullptr;
    spelldamage = 0;

#ifdef USE_SQL
    player_id = 0;
#endif
    spec = 0;

    brace_at=nullptr;
    brace_exit=nullptr;
}

char_data::~char_data()
{
	int iWear;
//  struct affected_type *af;
	struct char_player_alias * x;
	struct char_player_alias * next;
	MPROG_ACT_LIST * currmprog;
	auto &character_list = DC::instance().character_list;

	character_list.erase(this);

	if (tempVariable)
	{
		struct tempvariable *temp, *tmp;
		for (temp = tempVariable; temp; temp = tmp)
				{
			tmp = temp->next;
			dc_free(temp->name);
			dc_free(temp->data);
			dc_free(temp);
		}
	}
	SETBIT(affected_by, AFF_IGNORE_WEAPON_WEIGHT); // so weapons stop falling off

	for (iWear = 0; iWear < MAX_WEAR; iWear++) {
		if (equipment[iWear])
			obj_to_char(unequip_char(this, iWear, 1), this);
	}
	while (carrying)
		extract_obj(carrying);

	if (!IS_NPC(this)) {
		if (name)
			delete[] name;
		if (short_desc)
			delete[] short_desc;
		if (long_desc)
			delete[] long_desc;
		if (description)
			delete[] description;
		if (pcdata)
		{
			// these won't be here if you free an unloaded char
			pcdata->skillchange = 0;
			if (pcdata->last_site)
				delete[] pcdata->last_site;
			if (pcdata->ignoring)
				delete[] pcdata->ignoring;
			if (pcdata->poofin)
				delete[] pcdata->poofin;
			if (pcdata->poofout)
				delete[] pcdata->poofout;
			if (pcdata->prompt)
				delete[] pcdata->prompt;
			if (pcdata->last_prompt)
				delete[] pcdata->last_prompt;
			if (pcdata->last_tell)
				delete[] pcdata->last_tell;
			if (pcdata->golem)
				log("Error, golem not released properly", ANGEL, LOG_BUG);
			if (pcdata->joining)
				delete[] pcdata->joining;
			/* Free aliases... (I was to lazy to do before. ;) */
			for (x = pcdata->alias; x; x = next) {
				next = x->next;
				if (x->keyword)
					dc_free(x->keyword);
				if (x->command)
					dc_free(x->command);
				dc_free(x);
			}

			if (pcdata->away_msgs)
				delete[] pcdata->away_msgs;

			if (pcdata->lastseen)
				delete[] pcdata->lastseen;

			delete pcdata;
		}
	}
	else {
		remove_memory(this, 'f');
		remove_memory(this, 'h');
		while (mobdata->mpact) {
			currmprog = mobdata->mpact->next;
			if (mobdata->mpact->buf)
            {
				delete mobdata->mpact->buf;
            }
			delete mobdata->mpact;
			mobdata->mpact = currmprog;
		}
		delete mobdata;
	}

	if (title)
		delete[] title;
	title = nullptr;

	remove_memory(this, 't');

// Since affect_remove updates the linked list itself, do it this way
	while (affected)
		affect_remove(this, affected, SUPPRESS_ALL);

}

const QUuid & char_data::getUUID()
{
    return uuid;
}

const char *char_data::cond_colorcodes[] = {
    BOLD GREEN,
    GREEN,
    BOLD YELLOW,
    YELLOW,
    RED,
    BOLD RED,
    BOLD GREY,
};

QString char_data::getColorName()
{
    qreal percent = getHPPercent();

    QString color;
    if (percent >= 1)
        color = cond_colorcodes[0];
    else if (percent >= .9)
        color = cond_colorcodes[1];
    else if (percent >= .75)
        color = cond_colorcodes[2];
    else if (percent >= .50)
        color = cond_colorcodes[3];
    else if (percent >= .30)
        color = cond_colorcodes[4];
    else if (percent >= .15)
        color = cond_colorcodes[5];
    else if (percent >= 0)
        color = cond_colorcodes[6];

    return color + getName() + NTEXT;
}

QString char_data::getName()
{
    QString name;

    if (!IS_NPC(this))
        name = this->name;
    else
        name = this->short_desc;

    return name;
}

quint64 char_data::getGold()
{
    return this->gold;
}

quint64 char_data::getPlatinum()
{
    return this->plat;
}

quint64 char_data::getHP()
{
    return this->hit;
}

quint64 char_data::getMaxHP()
{
    return hit_limit(this);
}

qreal char_data::getHPPercent()
{
    return this->getHP() / this->getMaxHP();
}

quint64 char_data::getMana()
{
    return this->mana;
}

quint64 char_data::getMaxMana()
{
    return mana_limit(this);
}

qreal char_data::getManaPercent()
{
    return this->getMana() / this->getMaxMana();
}

quint64 char_data::getMove()
{
    return this->move;
}

quint64 char_data::getMaxMove()
{
    return move_limit(this);
}

qreal char_data::getMovePercent()
{
    return this->getMove() / this->getMaxMove();
}    

quint64 char_data::getKi()
{
    return this->ki;
}

quint64 char_data::getMaxKi()
{
    return this->max_ki;
}

qreal char_data::getKiPercent()
{
    return this->getKi() / this->getMaxKi();
}

quint64 char_data::getXP()
{
    return this->exp;
}

// +/-
qint64 char_data::getXPtoLevel()
{
    return exp_table[this->getLevel() + 1] - this->getXP();
}

quint64 char_data::getLevel()
{
    return this->level;
}

// +/-
qint64 char_data::getAlignment()
{
    return this->alignment;
}

QString char_data::getCondition()
{
    qreal percent = getHPPercent();

    if (percent >= 1)
        return "excellent condition";
    else if (percent >= 0.9)
        return "a few scratches";
    else if (percent >= 0.75)
        return "slightly hurt";
    else if (percent >= 0.5)
        return "fairly fucked up";
    else if (percent >= 0.30)
        return "bleeding freely";
    else if (percent >= 0.15)
        return "covered in blood";
    else if (percent >= 0)
        return "near death";
    else
        return "dead as a doornail";
}

QString char_data::getConditionColor()
{
    qreal percent = getHPPercent();

    if (percent >= 1)
        return BOLD GREEN "excellent condition" NTEXT;
    else if (percent >= 0.9)
        return GREEN "a few scratches" NTEXT;
    else if (percent >= 0.75)
        return BOLD YELLOW "slightly hurt" NTEXT;
    else if (percent >= 0.5)
        return YELLOW "fairly fucked up" NTEXT;
    else if (percent >= 0.30)
        return RED "bleeding freely" NTEXT;
    else if (percent >= 0.15)
        return BOLD RED "covered in blood" NTEXT;
    else if (percent >= 0)
        return BOLD GREY "near death" NTEXT;
    else
        return "dead as a doornail";
}

void char_data::show(QString messg)
{
    send_to_char(messg.toStdString().c_str(), this);
}

bool char_data::swapSkills(int16 old_skill, int16 new_skill)
{
    ch_skills_t::iterator i = skills.find(old_skill);
    if (i != skills.end())
    {
        char_skill_data skill = i->second;
        skill.skillnum = new_skill;
        skills[new_skill] = skill;
        skills.erase(i);

        skillsSaveLoadOrder.push(new_skill);        
        return true;
    }

    return false;
}

bool char_data::removeSkill(int16 skillnum)
{
    ch_skills_t::iterator i = skills.find(skillnum);
    if (i != skills.end())
    {
        skills.erase(skillnum);
        return true;
    }
    return false;
}


char_data *findCharacter(QUuid uuid)
{
    auto &character_list = DC::instance().character_list;
    auto result = find_if(character_list.begin(), character_list.end(),[&uuid](char_data * const &ch){
        if (ch->getUUID() == uuid) {
            return true;
        } else {
            return false;
        }
    });

    if (result != character_list.end()) 
    {
        return *result;
    }
    return nullptr;
}

pc_data::pc_data(void)
{
    memset(pwd, 0, sizeof(pwd));
    ignoring = nullptr;                 /* List of ignored names */

    alias = nullptr; /* Aliases */

    totalpkills = 0;         // total number of pkills THIS LOGIN
    totalpkillslv = 0;       // sum of levels of pkills THIS LOGIN
    pdeathslogin = 0;        // pdeaths THIS LOGIN

    rdeaths = 0;             // total number of real deaths
    pdeaths = 0;             // total number of times pkilled
    pkills = 0;              // # of pkills ever 
    pklvl = 0;               // # sum of levels of pk victims ever
    group_kills = 0;         // # of kills for group 
    grplvl = 0;              // sum of levels of group victims 

    last_site = nullptr;                /* Last login from.. */
    time = time_data();          // PC time data.  logon, played, birth
   
    bad_pw_tries = 0;        // How many times people have entered bad pws

    statmetas = 0;           // How many times I've metad a stat
                                // This number could go negative from stat loss
    kimetas = 0;             // How many times I've metad ki (pc only)

    wizinvis = 0;

    practices = 0;         // How many can you learn yet this level
    specializations = 0;   // How many specializations a player has left
    memset(saves_mods, 0, sizeof(saves_mods));  // Character dependant mods to saves (meta'able)

    bank = 0;           /* gold in bank                            */

    toggles = 0;            // Bitvector for toggles.  (Was specials.act)
    punish = 0;             // flags for punishments
    quest_bv1 = 0;          // 1st bitvector for quests

    poofin = nullptr;       /* poofin message */
    poofout = nullptr;      /* poofout message */    
    prompt = nullptr;       /* Sadus' disguise.. unused */

    buildLowVnum = 0;
    buildHighVnum = 0;
    buildMLowVnum = 0;
    buildMHighVnum = 0;
    buildOLowVnum = 0;
    buildOHighVnum = 0;
    skillchange = nullptr; /* Skill changing equipment. */

    last_mob_edit=0;       // vnum of last mob edited
    last_obj_edit=0;       // vnum of last obj edited

    last_tell=nullptr;          /* last person who told           */
    last_mess_read=0;     /* for reading messages */

    // TODO: these 3 need to become PLR toggles
    holyLite=false;          // Holy lite mode
    stealth=false;           // If on, you are more stealth then norm. (god)
    incognito=false;         // invis imms will be seen by people in same room

    possesing=false; 	      /*  is the person possessing? */
    unjoinable=false;        // Do NOT autojoin

    golem = nullptr; // CURRENT golem. 
    memset(hide, 0, sizeof(hide));
    memset(hiding_from, 0, sizeof(hiding_from));
    away_msgs = nullptr;
    tell_history = nullptr;
    joining = nullptr;
    quest_points = 0;
    memset(quest_current, 0, sizeof(quest_current));
    memset(quest_current_ticksleft, 0, sizeof(quest_current_ticksleft));
    memset(quest_cancel, 0, sizeof(quest_cancel));
    memset(quest_complete, 0, sizeof(quest_complete));
    last_prompt = nullptr;
    lastseen = nullptr;
    profession = 0;
}

char_file_u4::char_file_u4()
{
    sex = 0;     /* Sex */
    c_class = 0; /* Class */
    race = 0;    /* Race */
    level = 0;   /* Level */

    raw_str = 0;
    raw_intel = 0;
    raw_wis = 0;
    raw_dex = 0;

    raw_con = 0;
    conditions[0] = 0;
    conditions[1] = 0;
    conditions[2] = 0;
    conditions[3] = 0;

    weight = 0;
    height = 0;
    hometown = 0;

    gold = 0;
    plat = 0;
    exp = 0;
    immune = 0;
    resist = 0;
    suscept = 0;

    mana = 0;     // current
    raw_mana = 0; // max without eq/stat bonuses
    hit = 0;
    raw_hit = 0;
    move = 0;
    raw_move = 0;
    ki = 0;
    raw_ki = 0;

    alignment = 0;
    unused1 = 0;

    hpmetas = 0; // Used by familiars too... why not.
    manametas = 0;
    movemetas = 0;

    armor = 0; // have to save these since mobs have different bases
    hitroll = 0;

    damroll = 0;
    unused2 = 0;

    afected_by = 0;
    afected_by2 = 0;
    misc = 0; // channel flags

    clan = 0;
    unused3 = 0;
    load_room = 0; // Which room to place char in

    acmetas = 0;
    agemetas = 0;
    extra_ints[0] = 0; // available just in case
    extra_ints[1] = 0; // available just in case
    extra_ints[2] = 0; // available just in case
}

char_skill_data::char_skill_data()
{    
    skillnum = 0;          // ID # of skill.
    learned = 0;           // % chance for success must be > 0
    memset(unused, 0, sizeof(unused));         // for future use
}

  
