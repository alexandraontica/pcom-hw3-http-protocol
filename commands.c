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

char admin_cookies[MAX_LONG_LEN];
char user_cookies[MAX_LONG_LEN];
char token[MAX_LONG_LEN];

int cmp_obj_by_id(const void *json_obj_a, const void *json_obj_b) {
    JSON_Object *a = *(JSON_Object**)json_obj_a;
    JSON_Object *b = *(JSON_Object**)json_obj_b;
    int id_a = (int)json_object_get_number(a, "id");
    int id_b = (int)json_object_get_number(b, "id");
    return id_a - id_b;
}

void login_admin(char *username, char *password) 
{
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val);

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGIN_ADMIN_URL);

    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';  // make sure the string is null-terminated

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

    char *cookie = strstr(response, "Set-Cookie: ");  // extract the cookie from the response
    if (cookie) {
        cookie += strlen("Set-Cookie: ");
        char *end = strstr(cookie, ";");
        if (end) {
            *end = '\0';
        }
    }

    // save the cookie in the cookies array
    if (cookie) {
        strcpy(admin_cookies, cookie);
    }

    close_connection(sockfd);
    free(message);
    free(response);
    free(body_str);
}

void add_user(char *username, char *password)
{
    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val); 

    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_USER_URL);

    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
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
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_USERS_URL);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
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

            // go through the array to print the users
            for (int i = 0; i < array_size; i++) {
                JSON_Object *user_obj = json_array_get_object(users_array, i);
                int id = (int)json_object_get_number(user_obj, "id");
                const char *username = json_object_get_string(user_obj, "username");
                const char *password = json_object_get_string(user_obj, "password");
                printf("#%d %s:%s\n", id, username, password);
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
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, DELETE_USER_URL);
    strcat(url, "/");
    strcat(url, username);

    char *message = compute_delete_request(HOST, url, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
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
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGOUT_ADMIN_URL);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Admin delogat\n");

        // clear the cookies
        memset(admin_cookies, 0, sizeof(admin_cookies));

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

void login(char *admin_username, char *username, char *password)
{
    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "admin_username", admin_username);
    json_object_set_string(root_obj, "username", username);
    json_object_set_string(root_obj, "password", password);
    char *body_str = json_serialize_to_string(root_val);

    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL    
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGIN_URL);

    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, NULL);
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

    // extract the cookie from the response
    char *cookie = strstr(response, "Set-Cookie: ");
    if (cookie) {
        cookie += strlen("Set-Cookie: ");
        char *end = strstr(cookie, ";");
        if (end) {
            *end = '\0';
        }
    }

    // save the cookie in the cookies array
    if (cookie) {
        strcpy(user_cookies, cookie);
    }

    close_connection(sockfd);
    free(message);
    free(response);
    free(body_str);
}

void logout() {
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, LOGOUT_URL);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Utilizator delogat\n");

        // clear the cookies
        memset(user_cookies, 0, sizeof(user_cookies));

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
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_ACCESS_URL);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, NULL);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
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
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_MOVIES_URL);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            printf("SUCCESS: Lista filmelor\n");
  
            JSON_Array *movies_array = json_object_get_array(root_obj, "movies");
            int array_size = json_array_get_count(movies_array);

            JSON_Object *movies_list[array_size];
            for (int i = 0; i < array_size; i++) {
                movies_list[i] = json_array_get_object(movies_array, i);
            }

            // sort the movies by id
            qsort(movies_list, array_size, sizeof(JSON_Object*), cmp_obj_by_id);

            // go through the array to print the movies
            for (int i = 0; i < array_size; i++) {
                JSON_Object *movie_obj = movies_list[i];
                int id = (int)json_object_get_number(movie_obj, "id");
                const char  *title = json_object_get_string(movie_obj, "title");
                printf("#%d %s\n", id, title);
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
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_MOVIE_URL);
    strcat(url, "/");
    strcat(url, id);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
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
          
            // when I parsed the rating as a double, I always got 0.0
            // so I had to check if it's a string or a number
            // and parse it accordingly
            float rating;
            JSON_Value *r_val = json_object_get_value(root_obj, "rating");
            char *rating_string = (char *)json_value_get_string(r_val);
            rating = atof(rating_string);
            sprintf(rating_string, "%.1f", rating);

            printf("{\n"
                   "  \"title\": \"%s\",\n"
                   "  \"year\": %d,\n"
                   "  \"description\": \"%s\",\n"
                   "  \"rating\": \"%s\"\n"  // if I print the rating as a float the checker disregards everything after . (1.6 is intrepreted as 1)
                   "}\n",
               title, year, description, rating_string);        
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

void add_movie(char *title, int year, char *description, float rating)
{
    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "title", title);
    json_object_set_number(root_obj, "year", year);
    json_object_set_string(root_obj, "description", description);
    json_object_set_number(root_obj, "rating", rating);
    char *body_str = json_serialize_to_string(root_val);
    
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_MOVIE_URL);

    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
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

void delete_movie(char *id)
{
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, DELETE_MOVIE_URL);
    strcat(url, "/");
    strcat(url, id);

    char *message = compute_delete_request(HOST, url, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Film șters cu succes\n");
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

void update_movie(char *id, char *title, int year, char *description, float rating)
{
    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "title", title);
    json_object_set_number(root_obj, "year", year);
    json_object_set_string(root_obj, "description", description);
    json_object_set_number(root_obj, "rating", rating);
    char *body_str = json_serialize_to_string(root_val);

    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, UPDATE_MOVIE_URL);
    strcat(url, "/");
    strcat(url, id);

    char *message = compute_put_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Film actualizat\n");
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

void get_collections() 
{
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_COLLECTIONS_URL);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            printf("SUCCESS: Lista colecțiilor\n");
  
            JSON_Array *collections_array = json_object_get_array(root_obj, "collections");
            int array_size = json_array_get_count(collections_array);

            JSON_Object *collections_list[array_size];
            for (int i = 0; i < array_size; i++) {
                collections_list[i] = json_array_get_object(collections_array, i);
            }
            
            // sort the collections by id
            qsort(collections_list, array_size, sizeof(JSON_Object*), cmp_obj_by_id);

            // go through the array to print the collections
            for (int i = 0; i < array_size; i++) {
                JSON_Object *col = collections_list[i];
                int id  = (int)json_object_get_number(col, "id");
                const char  *title = json_object_get_string(col, "title");
                printf("#%d: %s\n", id, title);
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

void get_collection(char *id)
{
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, GET_COLLECTION_URL);
    strcat(url, "/");
    strcat(url, id);

    char *message = compute_get_request(HOST, url, NULL, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
            printf("SUCCESS: Detalii colecție\n");
  
            JSON_Array *movies_array = json_object_get_array(root_obj, "movies");
            int array_size = json_array_get_count(movies_array);

            const char *title = json_object_get_string(root_obj, "title");
            const char *owner = json_object_get_string(root_obj, "owner");

            printf("title: %s\n", title);
            printf("owner: %s\n", owner);

            JSON_Object *movies_list[array_size];
            for (int i = 0; i < array_size; i++) {
                movies_list[i] = json_array_get_object(movies_array, i);
            }

            // sort the movies by id
            qsort(movies_list, array_size, sizeof(JSON_Object*), cmp_obj_by_id);

            // go through the array to print the movies
            for (int i = 0; i < array_size; i++) {
                JSON_Object *m = movies_list[i];
                int id = (int)json_object_get_number(m, "id");
                const char  *title = json_object_get_string(m, "title");
                printf("#%d: %s\n", id, title);
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

int add_empty_collection(char *title, int *id)
{
    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_string(root_obj, "title", title);
    char *body_str = json_serialize_to_string(root_val);

    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_COLLECTION_URL);

    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    char *json = strstr(response, "{");  // find the start of the JSON object
    if (json) {
        JSON_Value *root_value = json_parse_string(json);
        JSON_Object *root_obj = json_value_get_object(root_value);

        if (!strncmp(status, "20", 2)) {
            // if no error occures, the request returns the id of the collection created;
            // I save it because I will need it to add movies to the collection
            *id = (int)json_object_get_number(root_obj, "id");
        } else if (!strncmp(status, "40", 2)) {
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);    
        } else {
            printf("unknown error - status: %s\n", status);
        }
        
        json_value_free(root_value);
    }

    close_connection(sockfd);
    free(message);
    free(response);
    free(body_str);

    // return the status code
    return atoi(status);
}

int add_movie_to_collection_aux(char *collection_id, int movie_id) {
    // build the JSON I will send to the server
    JSON_Value  *root_val = json_value_init_object();
    JSON_Object *root_obj = json_value_get_object(root_val);
    json_object_set_number(root_obj, "id", movie_id);
    char *body_str = json_serialize_to_string(root_val);

    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, ADD_MOVIE_TO_COLLECTION_FIRST_URL);
    strcat(url, "/");
    strcat(url, collection_id);
    strcat(url, ADD_MOVIE_TO_COLLECTION_SECOND_URL);

    char *message = compute_post_request(HOST, url, CONTENT_TYPE,
        &body_str, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    // treat only the error case because I call this functions for 2 different tasks and need different success messages
    if (!strncmp(status, "40", 2)) {
        char *json = strstr(response, "{");  // find the start of the JSON object
        if (json) {
            JSON_Value *root_value = json_parse_string(json);
            JSON_Object *root_obj = json_value_get_object(root_value);
            const char *error_message = json_object_get_string(root_obj, "error");
            printf("ERROR: %s\n", error_message);
            json_value_free(root_value);
        }
    }

    close_connection(sockfd);
    free(message);
    free(response);
    free(body_str);

    // return the status code
    return atoi(status);
}

void add_collection(char *title, int num_movies, char *movie_id[])
{
    int id;

    // create an empty collection and get its id
    int status = add_empty_collection(title, &id);

    // if the collection creation failed, return
    if (status >= 400) {
        return;
    }

    char id_str[MAX_SHORT_LEN];
    sprintf(id_str, "%d", id);

    // add the movies to the collection
    for (int i = 0; i < num_movies; i++) {
        status = add_movie_to_collection_aux(id_str, atoi(movie_id[i]));

        // if the movie addition failed, return
        if (status >= 400) {
            return;
        }
    }

    // print the details of the collection (get collection already does that)
    get_collection(id_str);
}

void add_movie_to_collection(char *collection_id, int movie_id)
{
    // add the movie to the collection
    int status = add_movie_to_collection_aux(collection_id, movie_id);
    
    // print success message (the error message is handled by auxiliary function)
    if (status >= 200 && status <= 210) {
        printf("SUCCESS: Film adăugat în colecție\n");
    }
}

void delete_collection(char *id)
{
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, DELETE_COLLECTION_URL);
    strcat(url, "/");
    strcat(url, id);

    char *message = compute_delete_request(HOST, url, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Colecție ștearsă\n");
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

void delete_movie_from_collection(char *collection_id, char *movie_id)
{
    // create the cookies array
    char *cookies_array[2];
    cookies_array[0] = admin_cookies;
    cookies_array[1] = user_cookies;

    // build the URL
    char url[MAX_LONG_LEN];
    strcpy(url, BASE_URL);
    strcat(url, DELETE_MOVIE_FROM_COLLECTION_FIRST_URL);
    strcat(url, "/");
    strcat(url, collection_id);
    strcat(url, DELETE_MOVIE_FROM_COLLECTION_SECOND_URL);
    strcat(url, "/");
    strcat(url, movie_id);

    char *message = compute_delete_request(HOST, url, cookies_array, NUM_COOKIES, token);
    int sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);

    // extract the status code from the response
    char status[4];
    strncpy(status, response + strlen("HTTP/1.1 "), 3);  // skip HTTP/1.1 to get the status code
    status[3] = '\0';

    if (!strncmp(status, "20", 2)) {  // compare with 20 (not 200) bc I can also receive 201 and be correct
        printf("SUCCESS: Film șters din colecție\n");
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
