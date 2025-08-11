#include "indexer.h"
#include <sstream>
#include <algorithm>
#include <cctype>

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
}
