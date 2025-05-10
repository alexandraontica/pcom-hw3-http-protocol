#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

        
    // Ex 1.1: GET dummy from main server
    message = compute_get_request("api.openweathermap.org", "/api/v1/dummy", NULL, NULL, 0);
    sockfd = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n%s\n\n", response);
    free(response);

    // Ex 1.2: POST dummy and print response from main server
    char **post_data = malloc(3 * sizeof(char *));
    post_data[0] = malloc(50);
    post_data[1] = malloc(50);
    strcpy(post_data[0], "first-name=smth");
    strcpy(post_data[1], "last-name=smth");
    message = compute_post_request("api.openweathermap.org", "/api/v1/dummy", "application/x-www-form-urlencoded", post_data, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n%s\n\n", response);
    free(response);

    // Ex 2: Login into main server
    strcpy(post_data[0], "username=student");
    strcpy(post_data[1], "password=student");
    message = compute_post_request("api.openweathermap.org", "/api/v1/auth/login", "application/x-www-form-urlencoded", post_data, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n%s\n\n", response);
    free(response);

    // Ex 3: GET weather key from main server
    char cookie[100] = "connect.sid=s%3AUb3FcaNhu4EeOVxHzQn0Mxuj9O_E6ScR.jMY7iNcQUJnGH9ODws%2FnEHsWF%2FLPYaFOG4O6Q3W6rw0";
    char **cookies = malloc(1 * sizeof(char *));
    cookies[0] = malloc(100);
    strcpy(cookies[0], cookie);
    message = compute_get_request("api.openweathermap.org", "/api/v1/weather/key", NULL, cookies, 1);
    sockfd = open_connection("54.217.160.10", 8080, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n%s\n\n", response);
    free(response);

    // Ex 4: GET weather data from OpenWeather API
    // {"key":"b912dd495585fbf756dc6d8f415a7649"}
    // ip 141.95.47.140
    message = compute_get_request("api.openweathermap.org", "/data/2.5/weather", "appid=b912dd495585fbf756dc6d8f415a7649&lat=44&lon=25", NULL, 0);
    sockfd = open_connection("141.95.47.140", 8080, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response:\n%s\n\n", response);
    free(response);

    // Ex 5: POST weather data for verification to main server
    
    // Ex 6: Logout from main server

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    return 0;
}
