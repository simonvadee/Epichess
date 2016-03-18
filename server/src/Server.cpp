#include "Server.hh"

using boost::asio::ip::tcp;

Server::Server(boost::asio::io_service& io_service, short port)
  : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
    socket_(io_service)
{
  do_accept();
}

Server::~Server()
{

}

void		Server::do_accept()
{
  acceptor_.async_accept(socket_,
			 [this](boost::system::error_code ec)
			 {
			   if (!ec)
			     {
			       std::make_shared<Session>(std::move(socket_))->start();
			     }

			   do_accept();
			 });
}
