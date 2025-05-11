#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "commands.h"

int main()
{
    while(1) {
        char command[MAX_LEN];
        scanf("%s", command);

        if (!strncmp(command, "login_admin", 11)) {
            printf("username=");
            char username[MAX_LEN];
            scanf("%s", username);

            printf("password=");
            char password[MAX_LEN];
            scanf("%s", password);

            login_admin(username, password);
        } else if (!strncmp(command, "add_user", 8)) {
            printf("username=");
            char username[MAX_LEN];
            scanf("%s", username);

            printf("password=");
            char password[MAX_LEN];
            scanf("%s", password);

            add_user(username, password);
        } else if (!strncmp(command, "get_users", 9)) {
            get_users();
        } else if (!strncmp(command, "delete_user", 11)) {
            printf("username=");
            char username[MAX_LEN];
            scanf("%s", username);

            delete_user(username);
        } else if (!strncmp(command, "logout_admin", 12)) {
            logout_admin();
        } else if (!strncmp(command, "login", 5)) {
            //
        } else if (!strncmp(command, "get_access", 10)) {
            //
        } else if (!strncmp(command, "get_movies", 10)) {
            //
        } else if (!strncmp(command, "get_movie", 9)) {
            //
        } else if (!strncmp(command, "add_movie", 9)) {
            //
        } else if (!strncmp(command, "delete_movie", 12)) {
            //
        } else if (!strncmp(command, "update_movie", 12)) {
            //
        } else if (!strncmp(command, "get_collections", 15)) {
            //
        } else if (!strncmp(command, "get_collection", 14)) {
            //
        } else if (!strncmp(command, "add_collection", 14)) {
            //
        } else if (!strncmp(command, "delete_collection", 17)) {
            //
        } else if (!strncmp(command, "add_movie_to_collection", 23)) {
            //
        } else if (!strncmp(command, "delete_movie_from_collection", 28)) {
            //
        } else if (!strncmp(command, "logout", 6)) {
            //
        } else if (!strncmp(command, "exit", 4)) {
            break;
        }
    }

    return 0;
}
