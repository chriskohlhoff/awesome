//
// new.cpp
// ~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <new>

void* operator new(std::size_t n)
{
  if (void* p = std::malloc(n))
  {
    std::cout << "NEW(" << n << ") = " << p << std::endl;
    return p;
  }
  else
    throw std::bad_alloc();
}

void operator delete(void* p)
{
  if (p)
  {
    std::cout << "DELETE(" << p << ")" << std::endl;
    std::free(p);
  }
}

void* operator new[](std::size_t n)
{
  if (void* p = std::malloc(n))
  {
    std::cout << "NEW[](" << n << ") = " << p << std::endl;
    return p;
  }
  else
    throw std::bad_alloc();
}

void operator delete[](void* p)
{
  if (p)
  {
    std::cout << "DELETE[](" << p << ")" << std::endl;
    std::free(p);
  }
}
