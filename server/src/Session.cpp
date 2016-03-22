#include "Session.hh"

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket)
  : _socket(std::move(socket)), _response(NULL), _data(NULL), _ia(new BrainCore())
{
}

void			Session::start()
{
  do_read();
}

void			Session::do_read()
{
  auto self(shared_from_this());
  _socket.async_read_some(boost::asio::buffer(_request, max_length),
			  [this, self](boost::system::error_code ec, std::size_t length)
			  {
			    if (!ec)
			      {
				_request[length] = 0;
				do_parse(length);
			      }
			  });
}
void			Session::do_parse(std::size_t length)
{
  std::string		toParse(_request);
  size_t		findGet = toParse.find("GET");
  size_t		findHttp = toParse.find("HTTP");

  if (findGet != std::string::npos && findHttp != std::string::npos)
    toGet = toParse.substr(findGet + 4, findHttp - (findGet + 5));

  if (toGet == "/"
      || toGet.find(".html") != std::string::npos
      || toGet.find(".js") != std::string::npos
      || toGet.find(".png") != std::string::npos
      || toGet.find(".css") != std::string::npos)
    set_content(toGet);
  else
    play_ia(toGet);

  do_read();
}

void			Session::do_write(size_t length)
{
  auto self(shared_from_this());
  boost::asio::async_write(_socket, boost::asio::buffer(_data, length),
  			   [this, self](boost::system::error_code ec, std::size_t bytesTransfered)
  			   {
  			     if (!ec)
  			       {
  				 memset(_request, 0, max_length);
				 if (_response)
				   delete[] _response;
				 if (_data)
				   delete[] _data;
				 _response = NULL;
				 _data = NULL;
  			       }
  			   });
}

void			Session::set_content(std::string& toGet)
{
  std::ifstream	ifs;
  std::string	buff;

  if (toGet == "/")
  toGet = "/board.html";

  std::string	file = "./client/serve" + toGet;

  std::cout << "REQUEST: " << toGet << std::endl;

  if (toGet.find(".png") != std::string::npos)
    ifs.open(file, std::ifstream::in | std::ios::binary);
  else
    ifs.open(file, std::ifstream::in);

  if (ifs.is_open())
    {
      ifs.seekg(0, ifs.end);
      _responseLength = ifs.tellg();
      ifs.seekg(0, ifs.beg);
      _response = new char[_responseLength + 1];
      ifs.read(_response, _responseLength);
      _response[_responseLength] = 0;
      do_write(make_header("200 OK"));
    }
  else
    {
      std::cerr << "file not found !!" << std::endl;
      _responseLength = 0;
      do_write(make_header("404 NOT FOUND"));
    }
}

void			Session::play_ia(std::string const& fen)
{
  std::cout << "REQUEST: " << fen << std::endl;

  if (!FEN::is_valid(fen))
    {
      _responseLength = 0;
      do_write(make_header("400 Bad Request"));
      return ;
    }
  _response = _ia->fightBack(fen);
  _responseLength = strlen(_response);
  do_write(make_header("200 OK"));
}

size_t			Session::make_header(std::string const& status)
{
  size_t		ret = 0;
  std::string		header;
  unsigned int		j = 0;

  header = "HTTP/1.1 " + status + "\r\n";
  header += "Content-Length: " + std::to_string(_responseLength) + "\r\n\r\n";

  ret = header.size() + 2;

  _data = new char[ret + _responseLength];
  memset(_data, 0, ret + _responseLength);
  strcat(_data, header.c_str());
  for (unsigned int i = header.size(); i < ret + _responseLength - 2; ++i)
    _data[i] = _response[j++];
  std::cout << "send " << ret + _responseLength << std::endl << header << std::endl;
  return ret + _responseLength;
}
