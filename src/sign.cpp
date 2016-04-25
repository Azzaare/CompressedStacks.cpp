/*** Signature : implementation ***/
#include "../include/sign.hpp"

/** New Block and Levels **/
template <class T>
Block<T> initBlock(int space)
{
  Block<T> block;
  block.resize(space);
  return block;
}

template <class T>
Levels<T> initLevels(int space, int depth)
{
  Levels<T> levels;
  for (int lvl = 1; lvl < depth; lvl++)
   {
    Block<T> block = initBlock<T>(space);
    levels.push(block);
   }
   levels.resize(depth-1);

   return levels;
}
