//
//  main.cpp
//  TypeListReader
//
//  Created by Дмитрий Ткаченко on 16/12/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#include <iostream>
#include "Reader.hpp"

typedef TypeList<int, char, float> MyList;

int main(int argc, const char * argv[]) {
    Reader<MyList>* reader = new Reader<MyList>;
    printBuffer<MyList>(reader->readNextLine());
    printBuffer<MyList>(reader->readNextLine());
    printBuffer<MyList>(reader->readNextLine());
}
