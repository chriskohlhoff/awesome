//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "server.hpp"
#include <boost/asio/write.hpp>

#include "yield.hpp"

namespace awesome {

using boost::asio::ip::tcp;

server::server(
    const std::string& listen_address, const std::string& listen_port,
    const std::string& target_address, const std::string& target_port)
  : io_service_(std::make_shared<boost::asio::io_service>()),
    resolver_(std::make_shared<tcp::resolver>(*io_service_)),
    acceptor_(std::make_shared<tcp::acceptor>(*io_service_,
          *resolver_->resolve({listen_address, listen_port}))),
    up_endpoint_(*resolver_->resolve({target_address, target_port})),
    allocator_(std::make_shared<allocator>())
{
}

server::server(const server& other)
  : coroutine(other),
    io_service_(other.io_service_),
    resolver_(other.resolver_),
    acceptor_(other.acceptor_),
    up_endpoint_(other.up_endpoint_),
    socket1_(other.socket1_),
    socket2_(other.socket2_),
    buffer_(other.buffer_),
    allocator_(other.allocator_)
{
  std::cout << "COPY" << std::endl;
}

void server::operator()(boost::system::error_code ec, std::size_t length)
{
  reenter (this)
  {
    fork server(*this)();

    if (is_parent())
    {
      io_service_->run();
      return;
    }

    do
    {
      socket1_ = std::make_shared<tcp::socket>(*io_service_);
      yield acceptor_->async_accept(*socket1_, std::move(*this));
      if (!ec) fork server(*this)();
    } while (is_parent());

    std::cout << "connection::start()" << std::endl;

    allocator_ = std::make_shared<allocator>();

    socket2_ = std::make_shared<tcp::socket>(*io_service_);
    yield socket2_->async_connect(up_endpoint_, std::move(*this));
    if (!ec)
    {
      fork server(*this)();

      buffer_ = std::make_shared<std::array<unsigned char, 1024>>();

      if (is_child())
      {
        allocator_ = std::make_shared<allocator>();
        std::swap(socket1_, socket2_);
      }

      while (!ec)
      {
        yield socket1_->async_read_some(
            boost::asio::buffer(*buffer_), std::move(*this));
        if (!ec)
        {
          yield boost::asio::async_write(*socket2_,
              boost::asio::buffer(*buffer_, length), std::move(*this));
        }
      }
    }

    if (socket1_->is_open() || socket2_->is_open())
    {
      std::cout << "connection::stop()" << std::endl;

      socket1_->close(ec);
      socket2_->close(ec);
    }
  }
}

} // namespace awesome

#include "unyield.hpp"
