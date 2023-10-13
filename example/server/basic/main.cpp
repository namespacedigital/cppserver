#include <namespace.digital/cppserver/router/router.hpp>
#include <iostream>

int main() {
    using namespacedigital::cppserver::router::Router;

    Router r = Router();
    std::cout << r.getHello() << std::endl;
}
