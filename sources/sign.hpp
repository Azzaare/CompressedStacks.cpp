/*** Signature : Header ***/
#include <vector>

template <class T>
class Signature
{
public:
  // Constructors
  Signature<T>(int index, int position, T context);

  // Setters
  void setLast(int index);

  // Getters
  int getFirst();
  int getLast();
  int getPosition();
  T getContext();

private:
  int mFirst;
  int mLast;
  int mPosition;
  T mContext;
};

/* Derived types: Block and Levels */
// A Partially Compressed Block is composed of the signatures of its SubBlocks
template<class T> using Block = std::vector<T,Signature<T>>;
template<class T> Block<T> initBlock(int space);

// Each level of compressed Blocks (first and second) are stored in Levels
template<class T> using Levels = std::vector<Block<T>>;
template<class T> Levels<T> initLevels(int space, int depth);
