//
// transfer.hpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AWESOME_TRANSFER_HPP
#define AWESOME_TRANSFER_HPP

#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <memory>

namespace awesome {

template <class Stream1, class Stream2, class Handler>
struct transfer_op
{
  bool do_read;
  Stream1& stream1;
  Stream2& stream2;
  boost::asio::mutable_buffers_1 working_buffer;
  Handler handler;

  void operator()(const boost::system::error_code& ec, std::size_t length)
  {
    if (!ec)
    {
      if (do_read)
      {
        do_read = false;
        stream1.async_read_some(working_buffer, *this);
      }
      else
      {
        do_read = true;
        boost::asio::async_write(stream2,
            boost::asio::buffer(working_buffer, length), *this);
      }
    }
    else
    {
      handler(ec);
    }
  }
};

template <class Stream1, class Stream2, class Handler>
void* asio_handler_allocate(std::size_t n,
    transfer_op<Stream1, Stream2, Handler>* op)
{
  using boost::asio::asio_handler_allocate;
  return asio_handler_allocate(n, std::addressof(op->handler));
}

template <class Stream1, class Stream2, class Handler>
void asio_handler_deallocate(void* p, std::size_t n,
    transfer_op<Stream1, Stream2, Handler>* op)
{
  using boost::asio::asio_handler_deallocate;
  return asio_handler_deallocate(p, n, std::addressof(op->handler));
}

template <class Function, class Stream1, class Stream2, class Handler>
void asio_handler_invoke(const Function& f,
    transfer_op<Stream1, Stream2, Handler>* op)
{
  using boost::asio::asio_handler_invoke;
  return asio_handler_invoke(f, std::addressof(op->handler));
}

template <class Stream1, class Stream2, class Handler>
void async_transfer(Stream1& stream1, Stream2& stream2,
    boost::asio::mutable_buffers_1 working_buffer, Handler handler)
{
  transfer_op<Stream1, Stream2, Handler>{
    true, stream1, stream2, working_buffer, handler }(
      boost::system::error_code(), 0);
}

} // namespace awesome

#endif // AWESOME_TRANSFER_HPP
