/** Data class : declaration **/
#include <vector>
#include <stack>

template <class D>
class Data
{
public:
  // Getters
  int getIndex();
  D getData();

private:
  int mIndex;
  D mData;
};

template<class D>
using Explicit = std::stack<Data<D>,std::vector<Data<D>>>;
