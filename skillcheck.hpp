#include <string>
#include "dice.hpp"
#include <algorithm>


using namespace dice;


struct Skill {
    std::string name;
    int value;
};



// Resolves skill using formula
// score = skill - 3d6 + modifier
//score > = 0 -> success

class SkillCheck {
    public:
    SkillCheck(const Skill& skill, int mod = 0) : 
    score(0), 
    roll(0),
    modifier(mod),
    skillDie(Die(6), 3),
    skill(skill){
        resolve();
    }

    bool isSuccess() const {
        return score >=0; 
    } 
    
    int getRoll() const {
        return roll;
    }

    int getScore() const {
        return score;
    }

    private:

    int score, roll, modifier;


    Skill skill;

    DiceSet skillDie;   

    void resolve() {
        skillDie.rollAll();
        roll = skillDie.getRoll();

        if(roll >= critFailureThreshold(skill.value)) {

            Die extraDie(6);
            extraDie.roll();
            score = skill.value - roll - extraDie.getRoll() + modifier;
            return;
        }

        else if (roll <= critSuccessThreshold(skill.value)) {
            score = skill.value + modifier;
            return;
        }


        score = skill.value - roll + modifier;
    }

    int critFailureThreshold(int skill){
        if (skill < 7) return 14;
        return std::min(18, 15+(skill -7)/3);
    }

    int critSuccessThreshold(int skill) {
        if(skill < 7) return 0;
        return 3 + (skill - 7)/3;

    }

};