#include <iostream>
#include <cstdlib>  // Required for std::system

int main() {
    // URL to download from
    std::string url = "https://time.com";
    
    // File to save the HTML content
    std::string outputFile = "temp.html";
    
    // Command to download the HTML content using curl
    std::string command = "curl -s " + url + " -o " + outputFile;
    
    // Execute the command
    int result = std::system(command.c_str());

    // Check if the command was successful
    if (result == 0) {
        std::cout << "HTML content downloaded successfully and saved to " << outputFile << std::endl;
    } else {
        std::cerr << "Failed to download HTML content. Curl exited with code " << result << std::endl;
    }

    return 0;
}
