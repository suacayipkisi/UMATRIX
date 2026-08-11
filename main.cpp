#include <initializer_list>
#include <iostream>

#include "src/UMTriple.h"
#include "src/infos.h"

int main(){
    std::cout << Info::nameMessage() << Info::name() << " starting" << '\n';

    UMTriple<double> testTensor{3};


    testTensor.u_pushBack(UMTMember<double>{{4, 4, 7}, 14.4});
    testTensor.u_pushBack({{7, 7, 7}, 44.89});

    UMTMember<double> testType{testTensor.getValue({4, 4, 7})};

    std::cout << testType.getValue() << '\n';;
    return 0;
}
