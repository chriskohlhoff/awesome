//
// server.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2011 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef AWESOME_SERVER_HPP
#define AWESOME_SERVER_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include "connection.hpp"

namespace awesome {

using boost::asio::ip::tcp;

// The top-level class of the proxy.
class server
  : private boost::noncopyable
{
public:
  // Construct the server to listen on the specified port.
  server(const std::string& listen_address, const std::string& listen_port,
      const std::string& target_address, const std::string& target_port);

  // Run the server's io_service loop.
  void run();

private:
  // Get the endpoint corresponding to the address and port.
  tcp::endpoint resolve_endpoint(
      const std::string& address, const std::string& port);

  // Initiate an asynchronous accept operation.
  void start_accept();

  // Handle completion of an asynchronous accept operation.
  void handle_accept(const boost::system::error_code& ec);

  // The io_service used to perform asynchronous operations.
  boost::asio::io_service io_service_;

  // Resolver used to turn host and service names into TCP endpoints.
  tcp::resolver resolver_;

  // Acceptor used to listen for incoming connections.
  tcp::acceptor acceptor_;

  // The target endpoint for all forwarded connections.
  tcp::endpoint up_endpoint_;

  // The next connection to be accepted.
  connection_ptr new_connection_;
};

} // namespace awesome

#endif // AWESOME_SERVER_HPP
