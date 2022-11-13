
class WebPreview {
    
    // Page that open when userrequest an unexisting page
    static void notfoundPage();

    // Routing "/"
    static void rootPage();

public:

    // Configure routings and starts the server
    void initServer();

    // handles client
    void handleClient();
};
