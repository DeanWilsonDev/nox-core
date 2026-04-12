#include <string>
#include "test-input.hpp"

namespace Tester {

class TestInputBase {
 protected:
  std::string helloWorld = {"Hello World"};
  Prism::TestEquals equals;
};

class TestInput : TestInputBase {
 public:
  TestInput();
  std::string GetHelloWorld() { return this->helloWorld; };
};

}  // namespace Tester
