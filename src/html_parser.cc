#include "html_parser.h"
#include <gumbo.h>

namespace {
    void extract_text_recursive(GumboNode* node, std::string& out) {
        if (node->type == GUMBO_NODE_TEXT) {
            out.append(node->v.text.text);
            out.push_back(' ');
        } else if (node->type == GUMBO_NODE_ELEMENT ||
                   node->type == GUMBO_NODE_TEMPLATE) {
            GumboVector* children = &node->v.element.children;
            for (unsigned int i = 0; i < children->length; ++i) {
                extract_text_recursive(static_cast<GumboNode*>(children->data[i]), out);
            }
        }
    }

    void extract_links_recursive(GumboNode* node, std::vector<std::string>& links) {
        if (node->type != GUMBO_NODE_ELEMENT) return;
        GumboAttribute* href;
        if (node->v.element.tag == GUMBO_TAG_A &&
            (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
            links.push_back(href->value);
        }
        GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            extract_links_recursive(static_cast<GumboNode*>(children->data[i]), links);
        }
    }
}

namespace quasar {
    std::string HtmlParser::extract_text(const std::string& html) {
        GumboOutput* output = gumbo_parse(html.c_str());
        std::string result;
        extract_text_recursive(output->root, result);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        return result;
    }

    std::vector<std::string> HtmlParser::extract_links(const std::string& html) {
        GumboOutput* output = gumbo_parse(html.c_str());
        std::vector<std::string> links;
        extract_links_recursive(output->root, links);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
        return links;
    }
}
