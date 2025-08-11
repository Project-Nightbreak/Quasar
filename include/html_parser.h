#pragma once
#include <string>
#include <vector>

namespace quasar {
    class HtmlParser {
    public:
        static std::string extract_text(const std::string& html);
        static std::vector<std::string> extract_links(const std::string& html);
    };
}
