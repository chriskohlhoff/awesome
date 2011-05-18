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
#include <boost/bind.hpp>
#include <iostream>
#include "transfer.hpp"

namespace awesome {

connection::connection(boost::asio::io_service& io_service)
  : down_socket_(io_service),
    up_socket_(io_service)
{
}

tcp::socket& connection::down_socket()
{
  return down_socket_;
}

void connection::start(const tcp::endpoint& up_endpoint)
{
  std::cout << "connection::start()" << std::endl;

  up_socket_.async_connect(up_endpoint,
      boost::bind(&connection::handle_connect, shared_from_this(), _1));
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

void connection::handle_connect(const boost::system::error_code& ec)
{
  if (!is_stopped())
  {
    if (!ec)
    {
      async_transfer(down_socket_, up_socket_, boost::asio::buffer(down_buffer_),
          boost::bind(&connection::handle_transfer, shared_from_this()));

      async_transfer(up_socket_, down_socket_, boost::asio::buffer(up_buffer_),
          boost::bind(&connection::handle_transfer, shared_from_this()));
    }
    else
    {
      stop();
    }
  }
}

void connection::handle_transfer()
{
  if (!is_stopped())
  {
    stop();
  }
}

} // namespace awesome
