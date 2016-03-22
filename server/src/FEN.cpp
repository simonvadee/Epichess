#include <iostream>
#include "FEN.hh"

namespace FEN
{

bool		is_valid(std::string const& fen)
{
  unsigned int	rowCount = 0;
  unsigned int	spaceCount = 0;

  for (size_t i = 0; i < fen.size(); ++i)
    {
      if (fen[i] == '/')
	++rowCount;
      else if (fen[i] == ' ')
	++spaceCount;
    }
  if (rowCount != 8)
  //     || spaceCount != 5)
    return false;
  return true;
}

}
