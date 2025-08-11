#include "indexer.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iomanip>

using json = nlohmann::json;

namespace quasar {
    // Common stopwords (English + HTML/CSS noise)
    static const std::unordered_set<std::string> STOPWORDS = {
        "the", "is", "in", "at", "of", "on", "for", "and", "a", "to", "or", "with",
        "this", "that", "it", "as", "an", "by", "be", "from", "are", "was", "were",
        "div", "span", "body", "html", "head", "title", "meta", "link", "script",
        "style", "backgroundcolor", "color", "margin", "padding", "width", "height",
        "em", "px", "bold", "fontfamily", "sans", "serif", "sansserif", "border",
        "align", "alink", "vlink", "hover", "visited", "auto"
    };

    std::string Indexer::normalize(const std::string& word) {
        std::string result;
        for (char c : word) {
            if (std::isalnum(static_cast<unsigned char>(c)))
                result.push_back(std::tolower(static_cast<unsigned char>(c)));
        }
        return result;
    }

    bool Indexer::is_stopword(const std::string& word) {
        return STOPWORDS.find(word) != STOPWORDS.end();
    }

    void Indexer::add_document(const std::string& url, const std::string& text) {
        std::istringstream iss(text);
        std::string word;
        int word_count = 0;

        while (iss >> word) {
            auto clean = normalize(word);
            if (clean.empty() || is_stopword(clean)) continue;

            index[clean][url]++;
            word_count++;
        }

        doc_lengths[url] = word_count;
        total_docs = doc_lengths.size();
    }

    std::vector<std::pair<std::string, double>> Indexer::search(const std::string& query) const {
        std::istringstream iss(query);
        std::string term;
        std::unordered_map<std::string, double> scores;

        std::vector<std::string> terms;
        while (iss >> term) {
            term = normalize(term);
            if (!term.empty() && !is_stopword(term)) {
                terms.push_back(term);
            }
        }
        if (terms.empty()) return {};

        for (const auto& q : terms) {
            auto it = index.find(q);
            if (it == index.end()) continue;

            int df = it->second.size();
            // Improved BM25-style IDF to avoid negatives
            double idf = std::log((total_docs - df + 0.5) / (df + 0.5) + 1.0);

            for (const auto& [url, tf] : it->second) {
                double tf_score = static_cast<double>(tf) / doc_lengths.at(url);
                scores[url] += tf_score * idf;
            }
        }

        std::vector<std::pair<std::string, double>> results(scores.begin(), scores.end());
        std::sort(results.begin(), results.end(),
                  [](auto& a, auto& b) { return a.second > b.second; });

        return results;
    }

    void Indexer::save_to_file(const std::string& filename) const {
        json j;
        j["index"] = index;
        j["doc_lengths"] = doc_lengths;
        j["total_docs"] = total_docs;
        std::ofstream file(filename);
        file << j.dump(2);
    }

    void Indexer::load_from_file(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return;
        json j;
        file >> j;
        index = j["index"].get<std::unordered_map<std::string, std::unordered_map<std::string, int>>>();
        doc_lengths = j["doc_lengths"].get<std::unordered_map<std::string, int>>();
        total_docs = j["total_docs"].get<int>();
    }
}
