//
//  main.cpp
//  FibonacciHierarchy
//
//  Created by Дмитрий Ткаченко on 29/11/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#include <iostream>
#include "FibonacciHierarchy.h"

typedef TypeList<int, bool, char, float, long, double> someList;
typedef GenFibonacciHierarchy<someList, Container> FibonacciHierarchy;

int main(int argc, const char * argv[]) {
    FibonacciHierarchy myHierarchy;
    return 0;
}
