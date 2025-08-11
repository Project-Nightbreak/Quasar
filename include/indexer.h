#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace quasar {
    class Indexer {
    public:
        void add_document(const std::string& url, const std::string& text);
        std::vector<std::string> search(const std::string& query) const;
        void save_to_file(const std::string& filename) const;
        void load_from_file(const std::string& filename);

    private:
        std::unordered_map<std::string, std::vector<std::string>> index;
    };
}
