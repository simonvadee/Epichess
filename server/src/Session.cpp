#include "Session.hh"

using boost::asio::ip::tcp;

Session::Session(tcp::socket socket)
  : _socket(std::move(socket))
{
}

// Session::~Session()
// {
// }

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
    {
      toGet = toParse.substr(findGet + 4, findHttp - (findGet + 5));
      // std::cout << "toget ===== " << toGet << std::endl;
    }
  std::ifstream	ifs;
  std::string	buff;

  if (toGet == "/")
    toGet = "/board.html";

  std::cout << "REQUEST: " << toGet << std::endl;
  std::string	file = "./client/serve" + toGet;

  if (toGet.find("png") != std::string::npos)
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
      do_write(make_header());
    }
  else
    {
      std::cerr << "file not found !!" << std::endl;
      _data = new char[strlen("HTTP/1.1 404 NOT FOUND\r\n\r\n")];
      strcat(_data, "HTTP/1.1 404 NOT FOUND\r\n\r\n");
      do_write(strlen(_data));
    }
  do_read();
}

void			Session::do_write(size_t length)
{
  auto self(shared_from_this());
  boost::asio::async_write(_socket, boost::asio::buffer(test, length),
  			   [this, self](boost::system::error_code ec, std::size_t bytesTransfered)
  			   {
  			     if (!ec)
  			       {
  				 memset(_request, 0, max_length);
  			       }
  			   });
}

size_t			Session::make_header()
{
  size_t		ret = 0;
  std::string		header;
  unsigned int		j = 0;

  header = "HTTP/1.1 200 OK\r\n";
  if (toGet.find("png") != std::string::npos)
    header += "Content-Type: image/png\r\n";
  header += "Content-Length: " + std::to_string(_responseLength) + "\r\n\r\n";

  ret = header.size() + 2;

  test = new char[ret + _responseLength];
  strcat(test, header.c_str());
  for (unsigned int i = header.size(); i != ret + _responseLength - 1; ++i)
    {
      test[i] = _response[j];
      ++j;
    }
  std::cout  << std::endl;
  std::cout << "send " << ret + _responseLength << std::endl << header << std::endl << std::endl;
  return ret + _responseLength;
}
