#include "crawler.h"
#include "indexer.h"
#include <gumbo.h>
#include <iostream>
#include <string>

std::string extract_text(GumboNode* node) {
    if (node->type == GUMBO_NODE_TEXT) {
        return std::string(node->v.text.text) + " ";
    }
    std::string text;
    if (node->type == GUMBO_NODE_ELEMENT &&
        node->v.element.tag != GUMBO_TAG_SCRIPT &&
        node->v.element.tag != GUMBO_TAG_STYLE) {
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            text += extract_text(static_cast<GumboNode*>(children->data[i]));
        }
    }
    return text;
}

int main() {
    quasar::Crawler crawler;
    quasar::Indexer indexer;

    try {
        std::string html = crawler.fetch("https://example.com");

        GumboOutput* output = gumbo_parse(html.c_str());
        std::string text = extract_text(output->root);
        gumbo_destroy_output(&kGumboDefaultOptions, output);

        indexer.add_document("https://example.com", text);

        std::string query;
        std::cout << "Enter search term: ";
        while (std::getline(std::cin, query)) {
            auto results = indexer.search(query);
            if (results.empty()) {
                std::cout << "No results found.\n";
            } else {
                std::cout << "Results:\n";
                for (const auto& url : results) {
                    std::cout << " - " << url << "\n";
                }
            }
            std::cout << "Enter search term: ";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
