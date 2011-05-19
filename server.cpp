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

namespace awesome {

server::server(
    const std::string& listen_address, const std::string& listen_port,
    const std::string& target_address, const std::string& target_port)
  : io_service_(),
    resolver_(io_service_),
    acceptor_(io_service_, *resolver_.resolve({listen_address, listen_port})),
    up_endpoint_(*resolver_.resolve({target_address, target_port})),
    down_socket_(io_service_)
{
}

void server::run()
{
  accept();
  io_service_.run();
}

void server::accept()
{
  acceptor_.async_accept(down_socket_,
      [&](const boost::system::error_code& ec)
      {
        if (!ec)
        {
          std::make_shared<connection>(std::move(down_socket_))->start(up_endpoint_);
        }

        accept();
      });
}

} // namespace awesome
