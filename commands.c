#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "commands.h"
#include "requests.h"
#include "helpers.h"
#include "parson.h"

int num_cookies = 0;
char cookies[MAX_LEN][MAX_BODY_LEN];

void login_admin(char *username, char *password) 
{
    if (num_cookies > 0) {
        printf("ERROR: already logged in\n");
        return;
    }

    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val); 

    char url[MAX_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGIN_ADMIN_URL);
    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, NULL, 0, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[3];
    strncpy(status, response + 9, 3);  // skip HTTP/1.1 to get the status code

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: admin login successful\n");
    } else if (!strncmp(status, "401", 3) || !strncmp(status, "403", 3)) {
        printf("ERROR: invalid credentials\n");
    } else {
        printf("unknown error - status: %s\n", status);
    }

    char *cookie = strstr(response, "Set-Cookie: ");
    if (cookie) {
        cookie += strlen("Set-Cookie: ");
        char *end = strstr(cookie, ";");
        if (end) {
            *end = '\0';
        }
    }

    if (cookie) {
        strcpy(cookies[num_cookies], cookie);
        num_cookies++;
    }

    close_connection(sockfd);
    free(message);
    free(response);
    free(body_str);
}