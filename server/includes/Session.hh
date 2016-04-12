#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#include "FEN.hh"
#include "BrainCore.hh"

using boost::asio::ip::tcp;

class Session
  : public std::enable_shared_from_this<Session>
{
public:

  Session(tcp::socket socket);

  void			start();

private:


  void			set_content(std::string&);
  void			play_ia(std::string const&);

  void			do_read();
  void			do_parse(std::size_t length);
  void			do_write(std::size_t length);

  size_t		make_header(std::string const& status);

private:

  tcp::socket		_socket;
  enum { max_length = 1024 };

  char			_request[max_length];
  char*			_response;
  size_t		_responseLength;

  char*			_data;

  std::string		toGet;

  BrainCore*		_ia;

};
