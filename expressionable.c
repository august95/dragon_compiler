#include "compiler.h"

#define TOTAL_OPERATOR_GROUPS 14
#define MAX_PERATORS_IN_GROUP 12



struct expresssionable_op_precedence_group op_precedence[TOTAL_OPERATOR_GROUPS] = {
    {.operators={"++","--", "()", "[]", "(", "[", ".", "->" ,NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"*", "/", "%", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"+", "-", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"<<", ">>", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"<", ">", "<=", ">=",  NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"==", "!=", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"&", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"^", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"|", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"&&", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"||", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT},
    {.operators={"?", ":" ,NULL}, .associtivity=ASSOSCIATIVITY_RIGHT_TO_LEFT},
    {.operators={"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=" ,NULL}, .associtivity=ASSOSCIATIVITY_RIGHT_TO_LEFT},
    {.operators={",", NULL}, .associtivity=ASSOSCIATIVITY_LEFT_TO_RIGHT}
};
