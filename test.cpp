#include "test.hpp"
#include <iostream>
#include <assert.h>  

using namespace std;

Test::Test(){
    correctMult = {C,B,D,D,D,D,A,B,C,C,A,B,B,C,B,C,C,D,A,D,D,D,A,A,B,B,D,C,A,C,B,B,A,C,C,B,D,A,C,C,D,A};
    correctVF = {V,V,V,V,V,V,F,F,V,V,F,V};
}

float Test::getScore(vector<OptionsMult> answersMult, vector<OptionsVF> answersVF){
    assert(answersMult.size() == correctMult.size());
    assert(answersVF.size() == correctVF.size());
    float score = 0.0f;
    for(int i = 0; i < answersMult.size() ; i++){
        if(answersMult[i] == correctMult[i]){
            score += QUESTION_VALUE;
        }
        else if (answersMult[i] == NMUL){
        }
        else{
            score -= QUESTION_VALUE / 3;
        }
    }
    
    for(int i = 0; i < answersVF.size() ; i++){
        if(answersVF[i] == correctVF[i]){
            score += QUESTION_VALUE;
        }
        else if (answersVF[i] == NVF){
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

