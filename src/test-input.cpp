#include <string>
#include "test-input.hpp"

namespace Tester {

class TestInputBase {
 protected:
  std::string helloWorld = {"Hello World"};
  NoxCore::TestEquals equals;
};

class TestInput : TestInputBase {
 public:
  TestInput();
  std::string GetHelloWorld() { return this->helloWorld; };
};

}  // namespace Tester
