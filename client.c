#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
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
            printf("admin_username=");
            char admin_username[MAX_LEN];
            scanf("%s", admin_username);

            printf("username=");
            char username[MAX_LEN];
            scanf("%s", username);

            printf("password=");
            char password[MAX_LEN];
            scanf("%s", password);

            login(admin_username, username, password);
        } else if (!strncmp(command, "get_access", 10)) {
            get_access();
        } else if (!strncmp(command, "get_movies", 10)) {
            get_movies();
        } else if (!strncmp(command, "get_movie", 9)) {
            printf("id=");
            char id[MAX_LEN];
            scanf("%s", id);

            get_movie(id);
        } else if (!strncmp(command, "add_movie", 9)) {
            printf("title=");
            char title[MAX_LEN];
            getchar();
            fgets(title, MAX_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            printf("year=");
            int year;
            scanf("%d", &year);

            printf("description=");
            char description[MAX_LEN];
            getchar();
            fgets(description, MAX_LEN, stdin);           
            description[strcspn(description, "\n")] = '\0';

            printf("rating=");
            double rating;
            scanf("%lf", &rating);

            add_movies(title, year, description, rating);
        } else if (!strncmp(command, "delete_movie", 12)) {
            printf("id=");
            char id[MAX_LEN];
            scanf("%s", id);

            delete_movie(id);
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
            logout();
        } else if (!strncmp(command, "exit", 4)) {
            break;
        } else {
            printf("invalid command\n");
        }
    }

    return 0;
}
