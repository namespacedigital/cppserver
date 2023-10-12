#include <server/router/router.hpp>
#include <iostream>

int main() {
    using namespacedigital::server::router::Router;

    Router r = Router();
    std::cout << r.getHello() << std::endl;
}
