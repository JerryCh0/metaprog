//
//  Decompressor.h
//  TypeListReader
//
//  Created by Дмитрий Ткаченко on 17/12/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//

#ifndef Decompressor_h
#define Decompressor_h

#include <fstream>

class NoDecompressor {
public:
    static void* decompress(void* compressed) { return compressed; }
};

template<typename From, typename To>
class Decompressor {
public:
    static To* decompress(From* compressed) {
        To* result = new To;
        *result = (To)(*compressed);
        return result;
    }
};

class Int {
public:
    Int(float float_): int_(float_) {}
    static Int* decompress(float* compressed) {
        Int* result = new Int(*compressed);
        return result;
    }
    friend std::ostream& operator<<(std::ostream& os, const Int& Int_) {
        os << Int_.int_;
        return os;
    }
private:
    int int_;
};

class Char {
public:
    Char(int int_): char_(int_) {}
    static Char* decompress(int* compressed) {
        Char* result = new Char(*compressed);
        return result;
    }
    friend std::ostream& operator<<(std::ostream& os, const Char& Char_) {
        os << Char_.char_;
        return os;
    }
private:
    char char_;
};

#endif /* Decompressor_h */
