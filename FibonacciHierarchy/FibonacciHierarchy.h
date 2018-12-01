//
//  FibonacciHierarchy.h
//  FibonacciHierarchy
//
//  Created by Дмитрий Ткаченко on 01/12/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#ifndef FibonacciHierarchy_h
#define FibonacciHierarchy_h

#include "LinearHierarchy.h"
#include "FibonacciNumber.h"

template
<
    typename TL,
    unsigned int branchIndex,
    template <typename Root, typename Branch> class Container
>
struct GenFibonacciHierarchyHelper;

template
<
unsigned int branchIndex,
template <typename Root, typename Branch> class Container
>
struct GenFibonacciHierarchyHelper<EmptyTypeList, branchIndex, Container>: public Container<NullType, NullType> {};

template
<
typename TL,
unsigned int branchIndex,
template <typename Root, typename Branch> class Container
>
struct GenFibonacciHierarchyHelper : public
    GenLinearHierarchy<typename TypeSublist<TL, 0, FibonacciNumber<branchIndex>::value + 1>::result, Container>,
    GenFibonacciHierarchyHelper<typename TypeSublist<TL, FibonacciNumber<branchIndex>::value + 1, UINT_MAX>::result, branchIndex + 1, Container> {};

template
<
    typename TL,
    template <typename Root, typename Branch> class Container
>
struct GenFibonacciHierarchy;

template
<
typename TL,
template <typename Root, typename Branch> class Container
>
struct GenFibonacciHierarchy : public GenFibonacciHierarchyHelper<TL, 0, Container> {};

#endif /* FibonacciHierarchy_h */
