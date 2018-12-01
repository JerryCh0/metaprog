//
//  FibonacciNumber.h
//  FibonacciHierarchy
//
//  Created by Дмитрий Ткаченко on 30/11/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#ifndef FibonacciNumber_h
#define FibonacciNumber_h

template<unsigned int index>
struct FibonacciNumber;

template<>
struct FibonacciNumber<0> {
    enum {
        value = 1
    };
};

template<>
struct FibonacciNumber<1> {
    enum {
        value = 1
    };
};

template<unsigned int index>
struct FibonacciNumber {
    enum {
        value = FibonacciNumber<index - 1>::value + FibonacciNumber<index - 2>::value
    };
};

#endif /* FibonacciNumber_h */
