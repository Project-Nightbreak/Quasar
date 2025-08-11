#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace quasar {
    class Indexer {
    public:
        void add_document(const std::string& url, const std::string& text);
        std::vector<std::string> search(const std::string& query) const;

    private:
        std::unordered_map<std::string, std::vector<std::string>> index;
    };
}
