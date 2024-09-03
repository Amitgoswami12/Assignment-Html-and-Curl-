#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

struct Story {
    std::string title;
    std::string link;
};

std::string fetchHTML(const std::string& url) {
    std::string command = "curl -s " + url + " > temp.html";
    std::system(command.c_str());

    std::ifstream file("temp.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<Story> parseStories(const std::string& html) {
    std::vector<Story> stories;
    size_t startPos = 0;

    while (stories.size() < 6) {
        startPos = html.find("<h3 class=\"headline\">", startPos);
        if (startPos == std::string::npos) break;

        startPos += 21; // Length of the tag
        size_t endPos = html.find("</h3>", startPos);
        std::string title = html.substr(startPos, endPos - startPos);

        startPos = html.find("<a href=\"", endPos);
        if (startPos == std::string::npos) break;

        startPos += 9; // Length of the tag
        endPos = html.find("\"", startPos);
        std::string link = html.substr(startPos, endPos - startPos);

        stories.push_back({title, "https://time.com" + link});
        startPos = endPos;
    }

    return stories;
}

void printStories(const std::vector<Story>& stories) {
    std::cout << "[\n";
    for (size_t i = 0; i < stories.size(); ++i) {
        std::cout << "  {\n";
        std::cout << "    \"title\": \"" << stories[i].title << "\",\n";
        std::cout << "    \"link\": \"" << stories[i].link << "\"\n";
        std::cout << "  }";
        if (i < stories.size() - 1) std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "]\n";
}


int main() {
    std::string html = fetchHTML("https://time.com");
    std::vector<Story> stories = parseStories(html);
    printStories(stories);
    return 0;
}
