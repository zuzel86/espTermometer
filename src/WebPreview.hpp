
#pragma once

class WebPreview {

    // Page that open when userrequest an unexisting page
    static void notfoundPage();

    // Routing "/"
    static void rootPage();

public:

    // Configure routings and starts the server
    static void initServer();

    // handles client
    static void handleClient();
};
