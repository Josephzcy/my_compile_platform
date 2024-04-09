#ifndef COMMON_BASE_UNCOPYABLE_H
#define COMMON_BASE_UNCOPYABLE_H

//  Private copy constructor and copy assignment ensure classes derived from
//  class Uncopyable cannot be copied.

//  Contributed by Dave Abrahams

// namespace common {

/// the private base class way
namespace UncopyableDetails  // protection from unintended ADL
{
class Uncopyable
{
protected:
    Uncopyable() {}
    ~Uncopyable() {}
private:  // emphasize the following members are private
    Uncopyable(const Uncopyable&);
    const Uncopyable& operator=(const Uncopyable&);
};
} // namespace UncopyableDetails

typedef UncopyableDetails::Uncopyable Uncopyable;

/*

Usage:

class Foo : private Uncopyable
{
};

*/

/// the macro way
#define DECLARE_UNCOPYABLE(Class) \
private: \
    Class(const Class&); \
    Class& operator=(const Class&)

/*

Usage:

class Foo
{
DECLARE_UNCOPYABLE(Foo);
};

*/

// } // namespace common

#endif // COMMON_BASE_UNCOPYABLE_H
