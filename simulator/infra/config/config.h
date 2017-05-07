/*
 * config.h - class for analysing and handling of inputed arguments
 * Copyright 2017 MIPT-MIPS
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <list>

#include <infra/types.h>

// small hack to include boost only in config.cpp
namespace boost { namespace program_options {
    class options_description;
} // namespace program_options 
} // namespace boost

namespace config {

using bod = boost::program_options::options_description;

class BaseValue
{
    friend void handleArgs( int argc, char** argv);
    virtual void reg( bod& d) = 0;

    static std::list<BaseValue*>& values() {
        static std::list<BaseValue*> instance;
        return instance;
    }

protected:
    BaseValue() { values().push_back( this); }
    virtual ~BaseValue() = default;
};

template<typename T>
class RequiredValue : public BaseValue {
protected:
    const std::string name;
    const std::string desc;

    T value;
    
    void reg( bod& d) override;
    RequiredValue<T>() = delete;
public:
    RequiredValue<T>( const char* name, const char* desc) noexcept
        : BaseValue( )
        , name( name)
        , desc( desc)
        , value( )
    { }
    
    operator const T&() const { return value; }
    ~RequiredValue() override = default;
};

template<typename T>
class Value : public RequiredValue<T> {
    const T default_value;

    void reg( bod& d) override;
    Value<T>() = delete;
public:
    Value<T>( const char* name, const T& val, const char* desc) noexcept
        : RequiredValue<T>( name, desc)
        , default_value( val)
    { this->value = val; }

    ~Value() final = default;
};

/* methods */
void handleArgs( int argc, char** argv);

} // namespace config

#endif  // CONFIG_H

