# CompressedStacks.cpp
```cpp
// Class for test instance
// T is the type of the context and D is the type of the input data. Please change it on 1st and 3rd lines.
class Instance: public Problem<T,D>{
public:
  Instance(std::string filePath, int size):Problem<T,D>(filePath, size){}
private:
  // Functions to implement according to the problem and input
  int mReadInput(){
    std::cout << "Implement mReadInput for your instance" << std::endl;
    return 0;
  }
  void mInitStack(){
    std::cout << "Implement mInitStack for your instance" << std::endl;
  }
  bool mPopCondition(int elt){
    std::cout << "Implement mPopCondition for your instance" << std::endl;
    return false;
  }
  void mPopAction(Data<D> elt){
    std::cout << "Implement mPopAction for your instance" << std::endl;
  }
  bool mPushCondition(int elt){
    std::cout << "Implement mPushCondition for your instance" << std::endl;
    return true;
  }
  void mPushAction(Data<D> elt){
    std::cout << "Implement mPushAction for your instance" << std::endl;
  }
};
```
