#include "indexer.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace {
    std::string normalize(const std::string& word) {
        std::string result;
        for (char c : word) {
            if (std::isalnum(static_cast<unsigned char>(c)))
                result.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
        return result;
    }
}

namespace quasar {
    void Indexer::add_document(const std::string& url, const std::string& text) {
        std::istringstream iss(text);
        std::string word;
        while (iss >> word) {
            auto clean = normalize(word);
            if (!clean.empty()) {
                index[clean].push_back(url);
            }
        }
    }

    std::vector<std::string> Indexer::search(const std::string& query) const {
        auto it = index.find(normalize(query));
        if (it != index.end()) {
            return it->second;
        }
        return {};
    }

    void Indexer::save_to_file(const std::string& filename) const {
        json j(index);
        std::ofstream file(filename);
        file << j.dump(2);
    }

    void Indexer::load_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return;
        json j;
        file >> j;
        index = j.get<std::unordered_map<std::string, std::vector<std::string>>>();
    }
}
