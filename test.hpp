#include <utility>
#include <vector>

#define NUM_OPTIONS 4
#define NUM_QUESTIONS_MULT 42
#define NUM_QUESTIONS_VF 12
#define QUESTION_VALUE 20/48

enum Side { BLACK_SIDE = 1, WHITE_SIDE = 2 };
enum Cell { EMPTY, BLACK = BLACK_SIDE, WHITE = WHITE_SIDE };

enum OptionsMult { A, B, C, D, N };
enum OptionsVF { V, F, N };

using namespace std;

class Test
{
    public:
        Test(vector<OptionsMult> correctMult, vector<OptionsVF> correctVF);
        float getScore(vector<OptionsMult> answersMult, vector<OptionsVF> answersVF);
        vector<bool> getResultMult(vector<OptionsMult> answersMult);  
        vector<bool> getResultVF(vector<OptionsVF> answersVF);  

    private:
        vector<OptionsMult> correctMult;
        vector<OptionsVF> correctVF;
};