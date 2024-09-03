#include <iostream>
#include <fstream>
#include <sstream>   // Required for std::stringstream
#include <cstdlib>   // Required for std::system

std::string fetchHTML(const std::string& url) {
    std::string command = "curl -s " + url + " -o temp.html";
    std::system(command.c_str());

    std::ifstream file("temp.html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    std::string url = "https://time.com";
    std::string html = fetchHTML(url);

    // Save HTML to a file for inspection
    std::ofstream outFile("output.html");
    outFile << html;
    outFile.close();

    std::cout << "HTML content saved to output.html" << std::endl;

    return 0;
}
