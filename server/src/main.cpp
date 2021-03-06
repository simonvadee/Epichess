#include "Server.hh"

int main(int argc, char* argv[])
{
  try
    {
      if (argc != 2)
	{
	  std::cerr << "Usage: ./bin/epichess <port>" << std::endl;;
	  return 1;
	}

      boost::asio::io_service io_service;

      Server s(io_service, std::atoi(argv[1]));

      io_service.run();
    }
  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
  return 0;
}
