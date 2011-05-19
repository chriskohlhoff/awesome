//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AWESOME_CONNECTION_HPP
#define AWESOME_CONNECTION_HPP

#include <array>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include "coroutine.hpp"

namespace awesome {

using boost::asio::ip::tcp;

// Represents a single connection from a client.
class connection : coroutine
{
public:
  // Construct a connection with the given io_service.
  explicit connection(tcp::socket&& down_socket);

  // Run the operations associated with the connection.
  void operator()(boost::system::error_code ec,
      const tcp::endpoint& up_endpoint = tcp::endpoint());

private:
  // The socket used for communicating with the client.
  std::shared_ptr<tcp::socket> down_socket_;

  // The socket used for communicating with the server.
  std::shared_ptr<tcp::socket> up_socket_;

  // Buffer for forwarding data.
  std::shared_ptr<std::array<unsigned char, 1024>> buffer_;
};

} // namespace awesome

#endif // AWESOME_CONNECTION_HPP
