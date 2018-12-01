//
//  LinearHierarchy.h
//  FibonacciHierarchy
//
//  Created by Дмитрий Ткаченко on 30/11/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#ifndef LinearHierarchy_h
#define LinearHierarchy_h

#include "TypeList.h"

template
<
    typename TL,
    template<typename Root, typename Branch> class Container
>
struct GenLinearHierarchy;

template
<
    template<typename Root, typename Branch> class Container
>
struct GenLinearHierarchy<EmptyTypeList, Container> : public Container<NullType, NullType> {};

template
<
    typename Head,
    typename ...Tail,
    template<typename Root, typename Branch> class Container
>
struct GenLinearHierarchy<TypeList<Head, Tail...>, Container> :
    public Container<Head, GenLinearHierarchy<TypeList<Tail...>, Container>> {};

template <typename Root, typename Branch>
struct Container: public Branch {};

#endif /* LinearHierarchy_h */
