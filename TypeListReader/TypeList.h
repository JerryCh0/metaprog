//
//  TypeList.h
//  FibonacciHierarchy
//
//  Created by Дмитрий Ткаченко on 29/11/2018.
//  Copyright © 2018 Klabertants. All rights reserved.
//  Using Andrey Aleksandresku's Book and https://habr.com/post/220217/
//

#ifndef TypeList_h
#define TypeList_h

class NullType {
public:
    static void* decompress(void* compressed) { return compressed; }
    
    friend std::ostream& operator<<(std::ostream& os, const NullType& NullType_) {
        return os;
    }
};

template<typename ... T>
struct TypeList {
    typedef NullType Head;
    typedef NullType Tail;
};

typedef TypeList<> EmptyTypeList;

template<typename H, typename ... T>
struct TypeList<H, T...> {
    typedef H Head;
    typedef TypeList<T...> Tail;
};

// Length
template <typename TL>
struct Length {
    enum {
        value = 1 + Length<typename TL::Tail>::value
    };
};

template <>
struct Length<EmptyTypeList> {
    enum {
        value = 0
    };
};

// Type at index
template <unsigned int index, typename TL>
struct TypeAt;

template<typename TL>
struct TypeAt<0, TL> {
    typedef typename TL::Head type;
};

template<unsigned int index, typename TL>
struct TypeAt {
    typedef typename TypeAt<index - 1, typename TL::Tail>::type type;
};

// Index of element
template <typename ElemType, typename TL>
struct IndexOf;

template <typename ElemType>
struct IndexOf<ElemType, EmptyTypeList> {
    enum {
        value = -1
    };
};

template <typename ElemType, typename ... Tail>
struct IndexOf<ElemType, TypeList<ElemType, Tail...>> {
    enum {
        value = 0
    };
};

template <typename ElemType, typename TL>
struct IndexOf {
private:
    enum {
        contains = IndexOf<ElemType, typename TL::Tail>::value
    };
public:
    enum {
        value = (contains == -1) ? contains : 1 + contains
    };
};

// Append type or typelist
template <typename TL, typename ElemType>
struct Append;

template <typename ... TL>
struct Append<TypeList<TL...>, NullType> {
    typedef TypeList<TL...> result;
};

template <typename ElemType>
struct Append<EmptyTypeList, ElemType> {
    typedef TypeList<ElemType> result;
};

template <typename ... TL>
struct Append<EmptyTypeList, TypeList<TL...>> {
    typedef TypeList<TL...> result;
};

template <typename ElemType, typename ... TL>
struct Append<ElemType, TypeList<TL...>> {
    typedef TypeList<ElemType, TL...> result;
};

template <typename ... TL, typename ElemType>
struct Append<TypeList<TL...>, ElemType> {
    typedef TypeList<TL..., ElemType> result;
};

template <typename ... TL, typename ... aTL>
struct Append<TypeList<TL...>, TypeList<aTL...>> {
    typedef TypeList<TL..., aTL...> result;
};

// Erase type at index
template <unsigned int index, typename TL>
struct EraseAt;

template <typename TL>
struct EraseAt<0, TL> {
    typedef typename TL::Tail result;
};

template <unsigned int index, typename TL>
struct EraseAt {
    typedef typename Append<typename TL::Head, typename EraseAt<index - 1, typename TL::Tail>::result>::result result;
};

// Add type at index
template <unsigned int index, typename ElemType, typename TL>
struct AddAt;

template <typename ElemType, typename TL>
struct AddAt<0, ElemType, TL> {
    typedef typename Append<ElemType, TL>::result result;
};

template <unsigned int index, typename ElemType, typename TL>
struct AddAt {
    typedef typename Append<typename TL::Head, typename AddAt<index - 1, ElemType, typename TL::Tail>::result>::result result;
};

// Replace type T with type U
template <typename TL, typename T, typename U>
struct Replace;

template <typename T, typename U>
struct Replace<EmptyTypeList, T, U> {
    typedef EmptyTypeList result;
};

template <typename ... Tail, typename T, typename U>
struct Replace<TypeList<T, Tail...>, T, U> {
    typedef TypeList<U, Tail...> result;
};

template <typename TL, typename T, typename U>
struct Replace {
    typedef typename Append<typename TL::Head, typename Replace<typename TL::Tail, T, U>::result>::result result;
};

// Sublisting from index with given length
template <typename TL, unsigned int start, unsigned int length>
struct TypeSublist;

template <typename TL, unsigned int start>
struct TypeSublist<TL, start, 0> {
    typedef EmptyTypeList result;
};

template <unsigned int start, unsigned int length>
struct TypeSublist<EmptyTypeList, start, length> {
    typedef EmptyTypeList result;
};

template <typename Head, typename ...Tail>
struct TypeSublist<TypeList<Head, Tail...>, 0, 1> {
    typedef TypeList<Head> result;
};

template <typename Head, typename ...Tail, unsigned int length>
struct TypeSublist<TypeList<Head, Tail...>, 0, length> {
    typedef typename Append<Head, typename TypeSublist<TypeList<Tail...>, 0, length - 1>::result>::result result;
};

template <typename TL, unsigned int start, unsigned int length>
struct TypeSublist {
    typedef typename TypeSublist<typename TL::Tail, start - 1, length>::result result;
};

// ------------- Additional logic for Reader task ------------------

#include <fstream>

typedef int BufferBasicType;

// Sum of elements size in bytes
template <typename TL>
struct MemSize {
    enum {
        value = sizeof(typename TypeAt<0, TL>::type) + MemSize<typename TL::Tail>::value
    };
};

template <>
struct MemSize<EmptyTypeList> {
    enum {
        value = 0
    };
};

// Writing into buffer
template <typename TL>
void dumpToBuffer(std::ifstream &inputStream, BufferBasicType* buffer) {
    typedef typename TypeAt<0, TL>::type currentType;
    inputStream >> *((currentType*)buffer);
    dumpToBuffer<typename TL::Tail>(inputStream, buffer + sizeof(currentType));
}

template <>
void dumpToBuffer<EmptyTypeList>(std::ifstream &inputStream, BufferBasicType* buffer) {}


// Printing buffer
template<typename TL>
void printBuffer(void *buffer) {
    typedef typename TypeAt<0, TL>::type currentType;
    std::cout << *((currentType*) buffer) << " ";
    printBuffer<typename TL::Tail>((BufferBasicType*)buffer + sizeof(currentType));
}

template<>
void printBuffer<EmptyTypeList>(void *buffer) { std::cout << std::endl; }

// -----     Additional logic for Reader stage 2 task    -------------

#include "Decompressor.h"

typedef int UniversalCompressedType;

// Decompressing into buffer
template<typename FileTL, typename DecompressedTL, typename DecompressorsTL>
void decompressToBuffer(std::ifstream &inputStream, BufferBasicType* buffer) {
    typedef typename TypeAt<0, FileTL>::type compressedType;
    typedef typename TypeAt<0, DecompressedTL>::type decompressedType;
    typedef typename TypeAt<0, DecompressorsTL>::type decompressorType;
    
    compressedType* tmp = new compressedType;
    
    inputStream >> *tmp;
    
    if (std::is_same<decompressedType, NullType>::value) {
        *buffer = *((BufferBasicType*)decompressorType::decompress(tmp));
        decompressToBuffer<typename FileTL::Tail, typename  DecompressedTL::Tail, typename DecompressorsTL::Tail>(inputStream, buffer + sizeof(UniversalCompressedType));
    } else {
        *buffer = *((BufferBasicType*)decompressedType::decompress(tmp));
        decompressToBuffer<typename FileTL::Tail, typename DecompressedTL::Tail, typename DecompressorsTL::Tail>(inputStream, buffer + sizeof(decompressedType));
    }
}

template<>
void decompressToBuffer<EmptyTypeList, EmptyTypeList, EmptyTypeList>(std::ifstream &inputStream, BufferBasicType* buffer) {}

// Boosted Memsize
template<typename DecompressedTL, typename DecompressorsTL>
struct BoostedMemSize {
    enum {
        value = (std::is_same<typename DecompressedTL::Head, NullType>::value) ? sizeof(UniversalCompressedType) + BoostedMemSize<typename DecompressedTL::Tail, typename DecompressorsTL::Tail>::value : sizeof(typename DecompressedTL::Head) + BoostedMemSize<typename DecompressedTL::Tail, typename DecompressorsTL::Tail>::value
    };
};

template <>
struct BoostedMemSize<EmptyTypeList, EmptyTypeList> {
    enum {
        value = 0
    };
};

// Boosted Printing buffer
template<typename DecompressedTL, typename DecompressorsTL>
void boostedPrintBuffer(void *buffer) {
    typedef typename TypeAt<0, DecompressedTL>::type decompressedType;
    typedef typename TypeAt<0, DecompressorsTL>::type decompressorType;
    if (std::is_same<decompressedType, NullType>::value) {
        std::cout << *((UniversalCompressedType*) buffer) << " ";
        boostedPrintBuffer<typename DecompressedTL::Tail, typename DecompressorsTL::Tail>((BufferBasicType*)buffer + sizeof(UniversalCompressedType));
    } else {
        std::cout << *((decompressedType*) buffer) << " ";
        boostedPrintBuffer<typename DecompressedTL::Tail, typename DecompressorsTL::Tail>((BufferBasicType*)buffer + sizeof(decompressedType));
    }
}

template<>
void boostedPrintBuffer<EmptyTypeList, EmptyTypeList>(void *buffer) { std::cout << std::endl; }

#endif /* TypeList_h */
