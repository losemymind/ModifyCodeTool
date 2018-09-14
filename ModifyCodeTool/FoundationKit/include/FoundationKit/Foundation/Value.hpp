/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

  Support Type:
      uint8
      uint16
      uint32
      unsigned long
      uint64
      bool
      char
      short
      int
      long
      long long
      float
      double
      char*
      std::string 

****************************************************************************/

/****************************************************************************
  Copyright (c) 2017 libo All rights reserved.
 
  losemymind.libo@gmail.com

****************************************************************************/
#ifndef FOUNDATIONKIT_VALUE_HPP
#define FOUNDATIONKIT_VALUE_HPP


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <vector>
#include <string>
#include <typeinfo>
#include "FoundationKit/FoundationMacros.hpp"
#include "FoundationKit/Base/types.hpp"

NS_FK_BEGIN

class AnyBase
{
public:
    AnyBase() {}
    virtual~AnyBase() {}
    virtual std::string ToString() { return "AnyBase"; }
};

template<typename _Ty>
class AnyHolder : public AnyBase
{
public:
    AnyHolder(const _Ty& data){RawData = data;}
    virtual~AnyHolder() {}
    virtual std::string ToString() { return  typeid(_Ty).name(); }
    _Ty Get(){return RawData;}
private:
    _Ty RawData;
};

template<>
class AnyHolder<void> : public AnyBase
{
public:
    AnyHolder() {}
    virtual~AnyHolder() {}
    virtual std::string ToString() { return  typeid(void).name(); }
    void Get() { }
};

class  Value
{
public:
    static const Value Null;
public:
    enum class EType
    {
        NONE = 0,
        UCHAR,
        USHORT,
        UINT,
        ULONGLONG,
        BOOL,
        CHAR,
        SHORT,
        INT,
        LONGLONG,
        FLOAT,
        DOUBLE,
        STRING,
        OTHER,
    };

    Value();
    ~Value();

    template<typename _Ty>
    Value(_Ty data);
    Value(const Value& other);
    Value(Value&& other);
    Value(uint8 data);
    Value(uint16 data);
    Value(uint32 data);
    Value(uint64 data);
    Value(bool data);
    Value(char data);
    Value(int16 data);
    Value(int32 data);
    Value(int64 data);
    Value(float data);
    Value(double data);
    Value(const char* data);
    Value(const std::string& data);

    // assignment operator
    template<typename _Ty>
    Value& operator= (_Ty data);
    Value& operator= (const Value& other);
    Value& operator= (Value&& other);
    Value& operator= (uint8 data);
    Value& operator= (uint16 data);
    Value& operator= (uint32 data);
    Value& operator= (uint64 data);
    Value& operator= (bool data);
    Value& operator= (char data);
    Value& operator= (int16 data);
    Value& operator= (int32 data);
    Value& operator= (int64 data);
    Value& operator= (float data);
    Value& operator= (double data);
    Value& operator= (const char* data);
    Value& operator= (const std::string& data);

    /** != operator overloading */
    bool operator!= (const Value& other);
    /** != operator overloading */
    bool operator!= (const Value& other) const;
    /** == operator overloading */
    bool operator== (const Value& other);
    /** == operator overloading */
    bool operator== (const Value& other) const;

    inline bool  IsNull()  const { return _type == EType::NONE; }
    inline EType GetType() const { return _type; };
    void         Copy(Value&  other);
    void         Move(Value&& other);
    void         Swap(Value&  other);
    void         Clear();
    std::string  ToString();

//================= Define Template Method =================
    template< typename _Ty >
    inline _Ty As();

    template< typename _Ty >
    inline operator _Ty()
    {
        return As<_Ty>();
    }
private:
    void Reset(EType valType);
    template< typename _Ty >
    inline _Ty ImplicitAs();
    Value(EType valType);
    union
    {
        uint8              _ucharVal;
        uint16             _ushortVal;
        uint32             _uintVal;
        uint64             _ulonglongVal;
        bool               _boolVal;
        int8               _charVal;
        int16              _shortVal;
        int32              _intVal;
        int64              _longlongVal;
        float              _floatVal;
        double             _doubleVal;
        char*              _stringVal; // char*/std::string
        AnyBase*           _otherData;
    }_field;

    EType _type;
};

typedef std::vector<Value>   ValueList;

//template <class _Ty>
//inline std::vector<Value>& operator << (std::vector<Value>& out, _Ty& c)
//{
//    out.emplace_back(c);
//    return out;
//}
//
//template <class _Ty>
//inline  std::vector<Value>& operator >> (std::vector<Value>& in, _Ty& c)
//{
//    ASSERTED(in.size() > 0, _FILE_AND_LINE_);
//    if (in.size() > 0)
//    {
//        c = in.begin()->As<_Ty>();
//        in.erase(in.begin());
//    }
//    return in;
//}

NS_FK_END
#endif // FOUNDATIONKIT_VALUE_HPP

#include "Value.inl"



