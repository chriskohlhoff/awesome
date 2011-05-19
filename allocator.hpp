//
// allocator.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AWESOME_ALLOCATOR_HPP
#define AWESOME_ALLOCATOR_HPP

#include <type_traits>

namespace awesome {

// Represents a single connection from a client.
class allocator
{
public:
  allocator(const allocator&) = delete;
  allocator& operator=(const allocator&) = delete;

  allocator()
    : in_use_(false)
  {
  }

  void* allocate(std::size_t n)
  {
    if (in_use_ || n >= 1024)
      return ::operator new(n);
    in_use_ = true;
    return static_cast<void*>(&space_);
  }

  void deallocate(void* p)
  {
    if (p != static_cast<void*>(&space_))
      ::operator delete(p);
    else
      in_use_ = false;
  }

private:
  // The reusable memory space made available by the allocator.
  std::aligned_storage<1024>::type space_;

  // Whether the reusable memory space is currently in use.
  bool in_use_;
};

} // namespace awesome

#endif // AWESOME_ALLOCATOR_HPP
