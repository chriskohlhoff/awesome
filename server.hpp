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
#include <memory>
#include <string>
#include "allocator.hpp"
#include "coroutine.hpp"

namespace awesome {

using boost::asio::ip::tcp;

// The top-level class of the proxy.
class server : coroutine
{
public:
  // Construct the server to listen on the specified port.
  server(const std::string& listen_address, const std::string& listen_port,
      const std::string& target_address, const std::string& target_port);

  // Copy constructor to print when called.
  server(const server& other);

  // Use default move constructor.
  server(server&&) = default;

  // Run the operations associated with the server.
  void operator()(boost::system::error_code ec = boost::system::error_code(),
      std::size_t length = 0);

private:
  // The io_service used to perform asynchronous operations.
  std::shared_ptr<boost::asio::io_service> io_service_;

  // Resolver used to turn host and service names into TCP endpoints.
  std::shared_ptr<tcp::resolver> resolver_;

  // Acceptor used to listen for incoming connections.
  std::shared_ptr<tcp::acceptor> acceptor_;

  // The target endpoint for all forwarded connections.
  tcp::endpoint up_endpoint_;

  // The pair of sockets that are being connected together by the proxy.
  std::shared_ptr<tcp::socket> socket1_, socket2_;

  // Buffer for forwarding data.
  std::shared_ptr<std::array<unsigned char, 1024>> buffer_;

  // Custom memory allocator.
  std::shared_ptr<allocator> allocator_;

  // Custom allocation hook.
  friend void* asio_handler_allocate(std::size_t n, server* s)
  {
    return s->allocator_->allocate(n);
  }

  // Custom deallocation hook.
  friend void asio_handler_deallocate(void* p, std::size_t, server* s)
  {
    s->allocator_->deallocate(p);
  }

  // Custom invocation hook.
  template <class Function>
  friend void asio_handler_invoke(const Function& f, server*)
  {
    Function tmp;
    tmp();
  }

  // Custom invocation hook.
  template <class Function>
  friend void asio_handler_invoke(Function& f, server*)
  {
    f();
  }
};

} // namespace awesome

#endif // AWESOME_SERVER_HPP
