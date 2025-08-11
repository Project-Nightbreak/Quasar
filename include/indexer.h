#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>

namespace quasar {
    class Indexer {
    public:
        void add_document(const std::string& url, const std::string& text);
        std::vector<std::pair<std::string, double>> search(const std::string& query) const;
        void save_to_file(const std::string& filename) const;
        void load_from_file(const std::string& filename);

    private:
        std::unordered_map<std::string, std::unordered_map<std::string, int>> index;
        std::unordered_map<std::string, int> doc_lengths;
        int total_docs = 0;

        static std::string normalize(const std::string& word);
        static bool is_stopword(const std::string& word);
    };
}
