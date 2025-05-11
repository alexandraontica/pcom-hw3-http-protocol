#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <math.h>
#include "commands.h"
#include "requests.h"
#include "helpers.h"
#include "parson.h"

int num_cookies = 0;
char cookies[MAX_LEN][MAX_BODY_LEN];
char token[MAX_BODY_LEN];

void login_admin(char *username, char *password) 
{
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val);

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGIN_ADMIN_URL);
    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, NULL, 0, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);
    status[3] = '\0';

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

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_USER_URL);
    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

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

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_USERS_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object

    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            printf("SUCCESS: Lista utilizatorilor\n");

            JSON_Array *users_array = json_object_get_array(root_obj, "users");
            int array_size = json_array_get_count(users_array);

            for (int i = 0; i < array_size; i++) {
                JSON_Object *user_obj = json_array_get_object(users_array, i);
                const char *username = json_object_get_string(user_obj, "username");
                const char *password = json_object_get_string(user_obj, "password");
                printf("#%d %s:%s\n", i + 1, username, password);
            }
        } else if (!strncmp(status, "40", 2)) {
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
        } else {
            printf("unknown error - status: %s\n", status);
        }
        
        json_value_free(root_value);
    } else {
        printf("ERROR: Invalid or no JSON response\n");
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

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, DELETE_USER_URL);
    strcat(url, "/");
    strcat(url, username);
    char *message = compute_delete_request(HOST, url, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

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

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGOUT_ADMIN_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Admin delogat\n");

        // clear the cookies
        for (int i = 0; i < num_cookies; i++) {
            memset(cookies[i], 0, sizeof(cookies[i]));
        }
        num_cookies = 0;
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

void login(char *admin_username, char *username, char *password)
{
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "admin_username", admin_username);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val);

    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGIN_URL);
    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Autentificare reușită\n");
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

void logout() {
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGOUT_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Utilizator delogat\n");

        // clear the cookies
        for (int i = 0; i < num_cookies; i++) {
            memset(cookies[i], 0, sizeof(cookies[i]));
        }
        num_cookies = 0;

        // clear the token
        memset(token, 0, sizeof(token));
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

void get_access()
{
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_ACCESS_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            printf("SUCCESS: Token JWT primit\n");
            const char *user_token = json_object_get_string(root_obj, "token");
            strcpy(token, user_token);
        } else if (!strncmp(status, "40", 2)) {
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
        } else {
            printf("unknown error - status: %s\n", status);
        }
    
        json_value_free(root_value);
    } else {
        printf("ERROR: Invalid or no JSON response\n");
    }

    close_connection(sockfd);
    free(message);
    free(response);
}

void get_movies()
{
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_MOVIES_URL);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            printf("SUCCESS: Lista filmelor\n");
  
            JSON_Array *users_array = json_object_get_array(root_obj, "movies");
            int array_size = json_array_get_count(users_array);

            for (int i = 0; i < array_size; i++) {
                JSON_Object *user_obj = json_array_get_object(users_array, i);
                const char *title = json_object_get_string(user_obj, "title");
                // int id = (int)json_object_get_number(user_obj, "id");
                printf("#%d %s\n", i + 1, title);
            }
        } else if (!strncmp(status, "40", 2)) {
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
        } else {
            printf("unknown error - status: %s\n", status);
        }
    
        json_value_free(root_value);
    } else {
        printf("ERROR: Invalid or no JSON response\n");
    }

    close_connection(sockfd);
    free(message);
    free(response);
}

void get_movie(char *id)
{
    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_MOVIE_URL);
    strcat(url, "/");
    strcat(url, id);
    char *message = compute_get_request(HOST, url, NULL, cookie_ptrs, num_cookies, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            const char *title = json_object_get_string(root_obj, "title");
            int year = (int)json_object_get_number(root_obj, "year");
            const char *description = json_object_get_string(root_obj, "description");
          
            // when i parsed the rating as a double, i always got 0.0
            // so I had to check if it's a string or a number
            // and parse it accordingly
            double rating;
            JSON_Value *r_val = json_object_get_value(root_obj, "rating");
            if (json_value_get_type(r_val) == JSONNumber) {
                rating = json_value_get_number(r_val);
                printf("rating = %.1f\n", rating);
            } else if (json_value_get_type(r_val) == JSONString) {
                const char *s = json_value_get_string(r_val);
                rating = atof(s);
                printf("rating = %.1f (parsed from string)\n", rating);
            }

            printf("{\n"
                   "  \"title\": \"%s\",\n"
                   "  \"year\": %d,\n"
                   "  \"description\": \"%s\",\n"
                   "  \"rating\": %.1f\n"
                   "}\n",
               title, year, description, rating);        
        } else if (!strncmp(status, "40", 2)) {
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
        } else {
            printf("unknown error - status: %s\n", status);
        }
    
        json_value_free(root_value);
    } else {
        printf("ERROR: Invalid or no JSON response\n");
    }

    close_connection(sockfd);
    free(message);
    free(response);
}

void add_movies(char *title, int year, char *description, double rating)
{
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "title", title);
    json_object_set_number(root_obj, "year", year);
    json_object_set_string(root_obj, "description", description);
    json_object_set_number(root_obj, "rating", round(rating * 10.0) / 10.0);
    char *body_str = json_serialize_to_string(root_val);

    printf("body_str: %s\n", body_str);

    char *cookie_ptrs[num_cookies];
    for (int i = 0; i < num_cookies; i++) {
        cookie_ptrs[i] = cookies[i];
    }

    char url[MAX_BODY_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_MOVIE_URL);
    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookie_ptrs, num_cookies, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Film adăugat\n");
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
