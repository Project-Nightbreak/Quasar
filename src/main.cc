#include "crawler.h"
#include "indexer.h"
#include "html_parser.h"
#include <iostream>
#include <unordered_set>
#include <filesystem>

int main() {
    quasar::Crawler crawler;
    quasar::Indexer indexer;

    const std::string index_file = "index.json";

    if (std::filesystem::exists(index_file)) {
        std::cout << "Loading index from file...\n";
        indexer.load_from_file(index_file);
    } else {
        std::unordered_set<std::string> visited;
        std::string seed = "https://example.com";
        std::vector<std::string> to_visit = {seed};
        int max_pages = 5;

        while (!to_visit.empty() && visited.size() < (size_t)max_pages) {
            std::string url = to_visit.back();
            to_visit.pop_back();

            if (visited.count(url)) continue;
            visited.insert(url);

            try {
                std::cout << "Crawling: " << url << "\n";
                std::string html = crawler.fetch(url);

                std::string text = quasar::HtmlParser::extract_text(html);
                indexer.add_document(url, text);

                auto links = quasar::HtmlParser::extract_links(html);
                for (auto& link : links) {
                    if (link.find("http") == 0 && !visited.count(link)) {
                        to_visit.push_back(link);
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Error fetching " << url << ": " << e.what() << "\n";
            }
        }

        indexer.save_to_file(index_file);
    }

    // Search loop
    std::string query;
    std::cout << "\nEnter search term: ";
    while (std::getline(std::cin, query)) {
        auto results = indexer.search(query);
        if (results.empty()) {
            std::cout << "No results found.\n";
        } else {
            std::cout << "Results:\n";
            for (const auto& u : results) {
                std::cout << " - " << u << "\n";
            }
        }
        std::cout << "\nEnter search term: ";
    }
}
