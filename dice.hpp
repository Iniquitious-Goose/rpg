//Die: uniform random natural numbers 1-sides
//DiceSet: aggregate multiple die rolls with optional modifier 

#pragma once
#include <random>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <vector>
#include "dice.hpp"

namespace dice{
    inline int validateSides(int x) {

    try {
    if(x <=0) {
        throw x;
    }
    
    }
    catch(int x) {
        std::cerr<<"Error: Sides must be >= 1\nSides are: \n" <<x;
    return 1;
}

    return x;
}

inline int validateCount(int x) {
    try {
if(x <=0) {
        std::cerr<<"Error: Count must be >= 1\n";;
        throw x;
    }
}
catch (int x) {
    std::cerr<<"Error: Count must be >= 1\nCount is: \n" <<x;
    return 1;
}
    return x;
}


class Die {

    public:
    Die(int sides = 6);

    void roll() ;
    int getRoll() const;
    
    void setSides(int);
    int getSides() const;
    std::string toString() const;
    
    
    private:
    int sides;
    int lastRoll;
    static std::mt19937 rng;
 
   

};


class DiceSet {

    public:
    DiceSet(Die dice, int count = 1, int modifier = 0);
    DiceSet();

    int getSides() const;

    void rollAll();
    int getModifier() const;

    int getSum()const;

    void setCount(const int);
    int getCount()const;

    void setDie(Die);
    Die getDie() const;

    void setSum(const int);
    
    void setModifier(const int);

    int getRoll () const;

    int getMin() const {return min;}

    int getMax() const {return max;}

    double getMean() const {return mean;}

    void setMin(const int x) {min = x;}
    void setMax(const int x) {max = x;}
    void setMean(const double x) {mean = x;}
    void dropMin() {sum -= min;}
    void dropMax() {sum -= max;}

    void setDrop(bool min, bool max) {dropLow = min; dropHigh = max;}
    std::string toString() const;
    std::string toBaseString() const;



    private:
    std::vector<int> diceSet;
    int count, modifier, sum, lastRoll, min = 0, max = 0;
    double mean =0;
    Die dice;
    bool dropLow = false, dropHigh = false;


};

//Methods for Die
inline Die::Die(int sides) : sides(validateSides(sides)), lastRoll(0){}

inline std::mt19937 Die::rng{std::random_device{}()};

inline void Die::roll() {
    std::uniform_int_distribution<int> dist(1, sides);

    lastRoll = dist(rng);
}

inline int Die::getRoll() const {
    return lastRoll;
}


inline void Die::setSides(const int x) {
    sides = validateSides(x);
}

inline int Die::getSides() const{
    return sides;
}

inline std::string Die::toString() const {
    std::ostringstream output;
    
    output << "Rolled" << ": " << getRoll() << "\n";


    return output.str();
}






//Methods exclusive to DiceSet
inline DiceSet::DiceSet(Die dice, int count, int modifier) : count(validateCount(count)), modifier(modifier), sum(0), dice(dice){

    rollAll();
}

inline DiceSet::DiceSet() : count(1), modifier(0), sum(0), dice(Die(6)){}

inline void DiceSet::setModifier(const int x) {modifier = x;}
inline void DiceSet::setSum(const int x) {sum = x;}

inline int DiceSet::getSum() const{
    return sum;
}

inline Die DiceSet::getDie() const{
    return dice;
}



inline void DiceSet::setCount(const int x) {
    count =validateCount(x);
}

inline int DiceSet::getModifier()const {return modifier;}

inline int DiceSet::getCount() const{return count;}
inline int DiceSet::getRoll() const{return lastRoll;}

inline int DiceSet::getSides() const {return dice.getSides();}

inline void DiceSet::rollAll() {
    
        setSum(modifier);

        diceSet.clear();

       for (int i =0; i < getCount(); i++) {
        dice.roll();
        diceSet.push_back(dice.getRoll());
        sum +=dice.getRoll();
    }

    setMean(static_cast<double>(sum)/count);

    min = diceSet[0]; max = diceSet[0];

    for (int i =0; i < getCount(); i++) {
        if (min > diceSet[i]) {
        min = diceSet[i];
        }
        else if (max < diceSet[i]) {
        max = diceSet[i];
        }
    }

    if (dropHigh) {
        dropMax();
    }
    if (dropLow) {
        dropMin();
    }


    
}

inline void DiceSet::setDie(Die die) {
    dice = die;
}
inline std::string DiceSet::toString() const{
    std::ostringstream output;
    
    for(size_t i =0; i < diceSet.size(); i++) {
        output << "Roll #" << i+1 << ": " << diceSet[i] << "\n";
    }

    output << "Modifier: " << modifier << "\nSum: " << sum << "\nHighest: " << max << "\nLowest: " << min << "\nMean: " << mean;
    return output.str();
}

inline std::string DiceSet::toBaseString() const{
    std::ostringstream output;
    
    output << count << "d" << dice.getSides() << " ";
    if(modifier >= 0) {
        output << "+ ";
    }

    output << modifier;

    return output.str();
}

}
