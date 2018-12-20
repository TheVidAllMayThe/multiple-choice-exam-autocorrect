#include <utility>
#include <vector>

#define NUM_OPTIONS 4
#define NUM_QUESTIONS_MULT 42
#define NUM_QUESTIONS_VF 12
#define QUESTION_VALUE 20.0/54.0

enum Side { BLACK_SIDE = 1, WHITE_SIDE = 2 };
enum Cell { EMPTY, BLACK = BLACK_SIDE, WHITE = WHITE_SIDE };

enum OptionsMult { A, B, C, D, NMUL };
enum OptionsVF { V, F, NVF };

using namespace std;

class Test
{
    public:
        Test();
        float getScore(vector<OptionsMult> answersMult, vector<OptionsVF> answersVF);
        vector<bool> getResultMult(vector<OptionsMult> answersMult);  
        vector<bool> getResultVF(vector<OptionsVF> answersVF);  

        vector<OptionsMult> correctMult;
        vector<OptionsVF> correctVF;
};