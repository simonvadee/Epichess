#include "BrainCore.hh"
#include <iostream>

int main(int ac, char **av)
{
  if (ac != 2)
    return 0;

  std::cout << "Start" << std::endl;
  BrainCore* b = new BrainCore();

  b->fightBack(av[1]);
}
