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

namespace awesome {

template <class Stream1, class Stream2, class Handler>
void async_transfer(Stream1& stream1, Stream2& stream2,
    boost::asio::mutable_buffers_1 working_buffer, Handler handler)
{
  stream1.async_read_some(working_buffer,
      [&stream1, &stream2, working_buffer, handler](
        const boost::system::error_code& ec, std::size_t length)
      {
        if (!ec)
        {
          boost::asio::async_write(stream2,
            boost::asio::buffer(working_buffer, length),
            [&stream1, &stream2, working_buffer, handler](
              const boost::system::error_code& ec, std::size_t)
            {
              if (!ec)
              {
                async_transfer(stream1, stream2, working_buffer, handler);
              }
              else
              {
                handler(ec);
              }
            });
        }
        else
        {
          handler(ec);
        }
      });
}

} // namespace awesome

#endif // AWESOME_TRANSFER_HPP
