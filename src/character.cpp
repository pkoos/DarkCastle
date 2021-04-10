#include <QString>
#include "character.h"
#include "db.h" // exp_table

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