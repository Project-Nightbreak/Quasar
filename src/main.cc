#include "crawler.h"
#include <iostream>

int main() {
    quasar::Crawler crawler;

    try {
        std::string content = crawler.fetch("https://example.com");
        std::cout << "Fetched content:\n" << content << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error fetching page: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
