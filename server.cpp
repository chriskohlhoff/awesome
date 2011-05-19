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
#include <functional>

namespace awesome {

using std::placeholders::_1;

server::server(
    const std::string& listen_address, const std::string& listen_port,
    const std::string& target_address, const std::string& target_port)
  : io_service_(),
    resolver_(io_service_),
    acceptor_(io_service_, resolve_endpoint(listen_address, listen_port)),
    up_endpoint_(resolve_endpoint(target_address, target_port)),
    down_socket_(io_service_)
{
}

void server::run()
{
  start_accept();
  io_service_.run();
}

tcp::endpoint server::resolve_endpoint(
    const std::string& address, const std::string& port)
{
  tcp::resolver::query query(address, port);
  return *resolver_.resolve(query);
}

void server::start_accept()
{
  acceptor_.async_accept(down_socket_,
      std::bind(&server::handle_accept, this, _1));
}

void server::handle_accept(const boost::system::error_code& ec)
{
  if (!ec)
  {
    std::make_shared<connection>(std::move(down_socket_))->start(up_endpoint_);
  }

  start_accept();
}

} // namespace awesome
