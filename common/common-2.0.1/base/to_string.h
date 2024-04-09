/*
 * =====================================================================================
 *
 *       Filename:  to_string.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年06月22日 19时55分06秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Devin (AO), devin@minieye.cc
 *   Organization:  
 *
 * =====================================================================================
 */
#if defined(ANDROID)

#ifndef STDTOSTRING_H
#define STDTOSTRING_H
#include <string>
#include <sstream>

using namespace std;

namespace std
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
#endif

#endif
