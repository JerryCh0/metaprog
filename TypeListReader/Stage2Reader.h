//
//  Stage2Reader.h
//  TypeListReader
//
//  Created by Дмитрий Ткаченко on 17/12/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#ifndef Stage2Reader_h
#define Stage2Reader_h

#include "TypeList.h"
#include <string>

static const std::string filename2 = "input2.txt";

template<typename FileTL, typename DecompressedTL, typename DecompressorsTL>
class Stage2Reader {
public:
    Stage2Reader(): inputStream(filename2) {}
    ~Stage2Reader() { inputStream.close(); }
    
    void* readNextLine() {
        assert(inputStream.is_open());
        BufferBasicType* buffer = (BufferBasicType*) std::malloc(BoostedMemSize<DecompressedTL, DecompressorsTL>::value);
        decompressToBuffer<FileTL, DecompressedTL, DecompressorsTL>(inputStream, buffer);
        return buffer;
    }
private:
    std::ifstream inputStream;
};

#endif /* Stage2Reader_h */
