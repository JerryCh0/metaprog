//
//  Reader.h
//  Reader
//
//  Created by Дмитрий Ткаченко on 16/12/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#ifndef Reader_h
#define Reader_h

#include "TypeList.h"
#include <string>

static const std::string filename = "input.txt";

template<typename TL>
class Reader {
public:
    Reader(): inputStream(filename) {}
    ~Reader() { inputStream.close(); }
    
    void* readNextLine() {
        assert(inputStream.is_open());
        BufferBasicType* buffer = (BufferBasicType*) std::malloc(MemSize<TL>::value);
        dumpToBuffer<TL>(inputStream, buffer);
        return buffer;
    }
private:
    std::ifstream inputStream;
};

#endif /* Reader_h */
