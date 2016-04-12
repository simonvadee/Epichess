#ifndef CHESSBOARD_HH_
# define CHESSBOARD_HH_

# include "Board.hh"

typedef enum
  {
    EMPTY = 0,
    KING ,
    QUEEN = 2,
    ROOK,
    BISHOP = 4,
    KNIGHT,
    PAWN = 6
  } PAWNS;

class ChessBoard : public Board
{
public:
  ChessBoard();
  ~ChessBoard();
  
private:
  std::vector<Pos>	_possibleMoves;

public:
  std::vector<Pos>&	findPossibleMoves(Pos& pos, int turn);
  int			evaluate();

private:
  bool			isThreatning(PAWNS type, int dir, int dist);
  bool			isInMap(Pos& pos);
  bool			isChessMate(Pos& pos);
  int			isIsolatedBlockedDouble(Pos& pos, int fact);
  int			evalMainPawns(PAWNS pawn, int fact);
  int			getMobility(int turn);

  void			findKingMoves(Pos& pos);
  void			findQueenMoves(Pos& pos);
  void			findRookMoves(Pos& pos);
  void			findBishopMoves(Pos& pos);
  void			findKnightMoves(Pos& pos);
  void			findPawnMoves(Pos& pos);
};

#endif
