#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco;

class StoryHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        if (request.getURI() == "/getTimeStories") {
            response.setContentType("application/json");
            response.setStatus(HTTPResponse::HTTP_OK);
            std::ostream& out = response.send();

            std::string url = "https://time.com";
            std::string command = "curl -s " + url + " -o temp.html";
            std::system(command.c_str());

            std::ifstream file("temp.html");
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string html = buffer.str();

            std::vector<std::string> stories;
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

                stories.push_back("{\"title\": \"" + title + "\", \"link\": \"https://time.com" + link + "\"}");
                startPos = endPos;
            }

            out << "[\n";
            for (size_t i = 0; i < stories.size(); ++i) {
                out << "  " << stories[i];
                if (i < stories.size() - 1) out << ",";
                out << "\n";
            }
            out << "]\n";
        } else {
            response.setStatus(HTTPResponse::HTTP_NOT_FOUND);
            response.setContentType("text/plain");
            std::ostream& out = response.send();
            out << "Not Found";
        }
    }
};

class StoryRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest&) override {
        return new StoryHandler;
    }
};

class StoryServerApp : public ServerApplication {
protected:
    void initialize(Application& self) override {
        loadConfiguration();
        ServerApplication::initialize(self);
    }

    void uninitialize() override {
        ServerApplication::uninitialize();
    }

    int main(const std::vector<std::string>& args) override {
        ServerSocket serverSocket(8080);
        HTTPServer server(new StoryRequestHandlerFactory, serverSocket, new HTTPServerParams);
        server.start();
        waitForTerminationRequest();
        server.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    StoryServerApp app;
    return app.run(argc, argv);
}
