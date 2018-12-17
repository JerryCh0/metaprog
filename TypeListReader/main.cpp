//
//  main.cpp
//  TypeListReader
//
//  Created by Дмитрий Ткаченко on 16/12/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#include <iostream>
#include "Reader.h"
#include "Stage2Reader.h"

typedef TypeList<int, char, float> Stage1List;

typedef TypeList<float, int, char> FileTL;
typedef TypeList<Int, Char, NullType> DecompressedTL;
typedef TypeList<NoDecompressor, NoDecompressor, Decompressor<char, int>> DecompressorsTL;

int main(int argc, const char * argv[]) {
    
    std::cout << "Stage 1 test:\n";
    Reader<Stage1List>* reader = new Reader<Stage1List>;
    printBuffer<Stage1List>(reader->readNextLine());
    printBuffer<Stage1List>(reader->readNextLine());
    printBuffer<Stage1List>(reader->readNextLine());
    
    std::cout << "\n\nStage 2 test:\n";
    auto stage2Reader = new Stage2Reader<FileTL, DecompressedTL, DecompressorsTL>;
    boostedPrintBuffer<DecompressedTL, DecompressorsTL>(stage2Reader->readNextLine());
    boostedPrintBuffer<DecompressedTL, DecompressorsTL>(stage2Reader->readNextLine());
}
