//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection.hpp"
#include <iostream>
#include "transfer.hpp"

namespace awesome {

connection::connection(tcp::socket&& down_socket)
  : down_socket_(std::move(down_socket)),
    up_socket_(down_socket_.get_io_service())
{
}

void connection::start(const tcp::endpoint& up_endpoint)
{
  std::cout << "connection::start()" << std::endl;

  connection_ptr this_ptr = shared_from_this();
  up_socket_.async_connect(up_endpoint,
      [this, this_ptr](const boost::system::error_code& ec)
      {
        if (!is_stopped())
        {
          if (!ec)
          {
            async_transfer(down_socket_, up_socket_, boost::asio::buffer(down_buffer_),
                [this, this_ptr](const boost::system::error_code&)
                {
                  if (!is_stopped())
                  {
                    stop();
                  }
                });

            async_transfer(up_socket_, down_socket_, boost::asio::buffer(down_buffer_),
                [this, this_ptr](const boost::system::error_code&)
                {
                  if (!is_stopped())
                  {
                    stop();
                  }
                });
          }
          else
          {
            stop();
          }
        }
      });
}

void connection::stop()
{
  std::cout << "connection::stop()" << std::endl;

  boost::system::error_code ec;
  down_socket_.close(ec);
  up_socket_.close(ec);
}

bool connection::is_stopped() const
{
  return !down_socket_.is_open() && !up_socket_.is_open();
}

} // namespace awesome
