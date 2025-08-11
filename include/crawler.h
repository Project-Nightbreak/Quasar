#pragma once
#include <string>

namespace quasar {
    class Crawler {
    public:
        std::string fetch(const std::string& url);
    };
}
