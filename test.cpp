#include "test.hpp"
#include <iostream>
#include <assert.h>  

using namespace std;

Test::Test(vector<OptionsMult> correctMult, vector<OptionsVF> correctVF){
    correctMult = correctMult;
    correctVF = correctVF;
}

float Test::getScore(vector<OptionsMult> answersMult, vector<OptionsVF> answersVF){
    assert(answersMult.size() == correctMult.size());
    assert(answersVF.size() == correctVF.size());
    float score = 0;
    for(int i = 0; i < answersMult.size() ; i++){
        if(answersMult[i] == correctMult[i]){
            score += QUESTION_VALUE;
        }
        else if (answersMult[i] == N){
        }
        else{
            score -= QUESTION_VALUE / 3;
        }
    }
    
    for(int i = 0; i < answersVF.size() ; i++){
        if(answersVF[i] == correctVF[i]){
            score += QUESTION_VALUE;
        }
        else if (answersVF[i] == N){
        }
        else{
            score -= QUESTION_VALUE / 3;
        }
    }

    return score;
}

vector<bool> Test::getResultMult(vector<OptionsMult> answersMult){

}

vector<bool> Test::getResultVF(vector<OptionsVF> answersVF){

}

