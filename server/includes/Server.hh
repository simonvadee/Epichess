#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "Session.hh"

using boost::asio::ip::tcp;

class Server
{
public:
  Server(boost::asio::io_service& io_service, short port);
  ~Server();

private:
  void		do_accept();

private:
  tcp::acceptor		acceptor_;
  tcp::socket		socket_;
};
