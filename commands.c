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
    strncpy(status, response + strlen("HTTP/1.1 "), 3);

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Admin autentificat cu succes\n");
    } else if (!strncmp(status, "40", 2)) {
        char *json = strstr(response, "{");  // find the start of the JSON object
        if (json) {
            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);  // the server already provides an error message when the user is already logged in or the credentionals are incorrect
            json_value_free(root_value);
        }
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

void add_user(char *username, char *password)
{
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val); 

    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_USER_URL);
    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[3];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Utilizator adăugat cu succes\n");
    } else if (!strncmp(status, "40", 2)) {
        char *json = strstr(response, "{");  // find the start of the JSON object
        if (json) {
            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
            json_value_free(root_value);
        }
    } else {
        printf("unknown error - status: %s\n", status);
    }

    close_connection(sockfd);
    free(message);
    free(response);
    free(body_str);
}

void get_users()
{
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_USERS_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[3];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code

    char *json = strstr(response, "{");  // find the start of the JSON object

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        if (json) {
            printf("SUCCESS: Lista utilizatorilor\n");

            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            JSON_Array *users_array = json_object_get_array(root_obj, "users");
            int array_size = json_array_get_count(users_array);

            for (int i = 0; i < array_size; i++) {
                JSON_Object *user_obj = json_array_get_object(users_array, i);
                char *username = (char *)json_object_get_string(user_obj, "username");
                char *password = (char *)json_object_get_string(user_obj, "password");
                printf("#%d %s:%s\n", i + 1, username, password);
            }
        }
    } else if (!strncmp(status, "40", 2)) {
        if (json) {            
            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
            json_value_free(root_value);
        }
    } else {
        printf("unknown error - status: %s\n", status);
    }

    close_connection(sockfd);
    free(message);
    free(response);
}

void delete_user(char *username)
{
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_LEN];
    strcpy(url, BASE_URL);
    strcat(url, DELETE_USER_URL);
    strcat(url, "/");
    strcat(url, username);
    char *message = compute_delete_request(HOST, url, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[3];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Utilizator șters\n");
    } else if (!strncmp(status, "40", 2)) {
        char *json = strstr(response, "{");  // find the start of the JSON object
        if (json) {
            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
            json_value_free(root_value);
        }
    } else {
        printf("unknown error - status: %s\n", status);
    }

    close_connection(sockfd);
    free(message);
    free(response);
}

void logout_admin()
{
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGOUT_ADMIN_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[3];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Admin delogat\n");
    } else if (!strncmp(status, "40", 2)) {
        char *json = strstr(response, "{");  // find the start of the JSON object
        if (json) {
            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
            json_value_free(root_value);
        }
    } else {
        printf("unknown error - status: %s\n", status);
    }

    close_connection(sockfd);
    free(message);
    free(response);
}


