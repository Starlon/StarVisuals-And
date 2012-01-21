#include "evaluator.h"
#include "visscript.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char point[] = "d=i+v*0.2; r=t+i*PI*4*count; x = cos(r)*d; y = sin(r) * d";
char frame[] = "t=t-0.01;count=count+1";
char beat[] = "";
char init[] = "n=800";

int main(int argc, char **argv)
{
    void *tree1;
    void *tree2;
    void *tree3;
    void *tree4;
    double d = 1;
    RESULT *result = malloc(sizeof(RESULT));
    memset(result, 0, sizeof(RESULT));

    SetResult(&result, R_NUMBER, &d);

    init_evaluator();

    SetVariable("n", result);
    SetVariable("b", result);
    SetVariable("x", result);
    SetVariable("y", result);
    SetVariable("i", result);
    SetVariable("v", result);
    SetVariable("w", result);
    SetVariable("h", result);
    SetVariable("t", result);
    SetVariable("d", result);
    SetVariable("red", result);
    SetVariable("green", result);
    SetVariable("blue", result);
    SetVariable("linesize", result);
    SetVariable("skip", result);
    SetVariable("drawmode", result);
    SetVariable("count", result);


    Compile(point, &tree1);
    Compile(frame, &tree2);
    Compile(beat, &tree3);
    Compile(init, &tree4);

while(1) {
    Eval(tree1, result);
    Eval(tree2, result);
    Eval(tree3, result);
    Eval(tree4, result);
}

    RESULT *var = GetVariable("d");

    var = GetVariable("t");
    printf("t = %f\n", var->number);

    free(result);

    return 0;
}
