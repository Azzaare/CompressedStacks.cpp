# CompressedStacks.cpp

The CompressedStacks.cpp module/library implements the compressed stack structure. This data structure behaves like a usual stack with the usual push and pop operations, but has the additional advantage that it uses less memory. Intuitively, when large blocks of information are pushed into the stack it *compresses* the bottom part (only stores partial information). This information is recomputed whenever it is needed afterwards. See the paper of [Barba *et al.*](https://arxiv.org/abs/1208.3663) for more details on this structure. More information about the implementation can be found in [Baffier *et al*](https://arxiv.org/abs/1706.04708).

Please consult the [wiki](https://github.com/Azzaare/CompressedStacks.cpp/wiki) of this project for further details such as : speed and memory consumption tests, more details about installation, examples, and more.

## Description of Stack Algorithms
<p>
This structure works mostly as a normal stack (handling push and pop operations, as well as being able to access the top k elements of the stack, for some small k). Note that this structure can only be used with sequential deterministic algorithms. We model these algorithms with the template StackAlgo whose only implemented function implemented is the run operation in which we scan the whole input (see code below).
</p>

```cpp
template <class T, class D> void StackAlgo<T, D>::run() {
  initStack();
  while (notEndOfFile()) {
    D data = readInput(line);
    while (notEmptystack()) {
      if (popCondition(data)) {
        prePop(data);
        elt = pop();
        postPop(elt,data);
      } else {
        noPop(data);
      }
    }
    if (pushCondition(data)) {
      prePush(data);
      push(data);
      postPush(data);
    } else {
      noPush(data);
    }
  }
  reportStack();
}
```
## Use case
Concrete examples such as a basic test run and the upper hull problems can be found in the [wiki](https://github.com/Azzaare/CompressedStacks.cpp/wiki).

### Abstract example : ```Instance<T,D,I>```
<p>An instance of a Stack Algorithm is described by a set of templates parameters T, D, and I and a set of methods used in the run function above.</p>

```cpp
// T is the type of the context, D is the type of the input data and I is the type of your integer indexes.

class Instance: public StackAlgo<T,D,I>{
public:
  Instance(std::string filePath) : StackAlgo<T, D, I>(filePath) {}
private:
  // Methods to implement according to the problem and input structure
  // Some of those methods might be left empty
  D readInput(std::vector<std::string> line);
  std::shared_ptr<T> initStack();

  bool popCondition(D data);
  void prePop(D data);
  void postPop(D data, Data<T, D, I> elt);
  void noPop(D data);

  bool pushCondition(D data);
  void prePush(Data<T, D, I> elt);
  void postPush(Data<T, D, I> elt);
  void noPush(D data);

  void reportStack();
};
```
### How to run your problem
Suppose the class Instance implements the interface ```StackAlgo<T, D, I>```. You can run an instance of your problem described in the input located at <i>filepath</i>. The last command just print an output in the console of your compressed stack after the run.

```cpp
Instance stack(filePath);
stack.run();
stack.println();
```

## Contributing
This project is far from being complete and would benefit greatly from future contributions. Commented code, following the existing file structure is strongly preferred. Please contact one of the author (or create an issue) in case of need. Here is a short sample of possible contributions :
* Use CI (continuous integration). Definitively the most wanted feature
* Extends the compressed stack structure to a dequeue structure (push and pop from top and bottom)
* Add other problems to the examples folder (following, if possible a similar structure)
* Extends the compressed stack structure to a compressed tree search structure
* Dynamic size compressed stack.


## Credits
Although this project is a joint work, based on the theoretical work in [Barba *et al.*](https://arxiv.org/abs/1208.3663), credits belong to specific authors for part of the implementation. The work covering the implementation of the Stack Algorithm framework, the Compressed Stack structure and extras functionalities (```include``` and ```extras``` repositories) has been done by [Jean-Francois Baffier](https://github.com/Azzaarehttps://github.com/Azzaare). All the examples and their generating algorithms, along with all the test have been implemented by [Yago Diez](https://github.com/nicill). The class design and project overview was done by [Matias Korman](https://github.com/matiaskorman)

## License
This project is an open source software under the MIT license. Please check the ```LICENSE``` file for further information.
