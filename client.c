#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <string.h>     /* memcpy, memset */
#include "commands.h"

int main()
{
    while(1) {
        char command[MAX_SHORT_LEN];
        scanf("%s", command);

        if (!strncmp(command, "login_admin", 11)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            scanf("%s", username);

            printf("password=");
            char password[MAX_SHORT_LEN];
            scanf("%s", password);

            login_admin(username, password);
        } else if (!strncmp(command, "add_user", 8)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            scanf("%s", username);

            printf("password=");
            char password[MAX_SHORT_LEN];
            scanf("%s", password);

            add_user(username, password);
        } else if (!strncmp(command, "get_users", 9)) {
            get_users();
        } else if (!strncmp(command, "delete_user", 11)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            scanf("%s", username);

            delete_user(username);
        } else if (!strncmp(command, "logout_admin", 12)) {
            logout_admin();
        } else if (!strncmp(command, "login", 5)) {
            printf("admin_username=");
            char admin_username[MAX_SHORT_LEN];
            scanf("%s", admin_username);

            printf("username=");
            char username[MAX_SHORT_LEN];
            scanf("%s", username);

            printf("password=");
            char password[MAX_SHORT_LEN];
            scanf("%s", password);

            login(admin_username, username, password);
        } else if (!strncmp(command, "get_access", 10)) {
            get_access();
        } else if (!strncmp(command, "get_movies", 10)) {
            get_movies();
        } else if (!strncmp(command, "get_movie", 9)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            scanf("%s", id);

            get_movie(id);
        }  else if (!strncmp(command, "add_movie_to_collection", 23)) {
            printf("collection_id=");
            char collection_id[MAX_SHORT_LEN];
            scanf("%s", collection_id);

            printf("movie_id=");
            int movie_id;
            scanf("%d", &movie_id);

            add_movie_to_collection(collection_id, movie_id);
        } else if (!strncmp(command, "add_movie", 9)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            getchar();  // read the newline character left by scanf
            fgets(title, MAX_SHORT_LEN, stdin);  // titles can have spaces
            title[strcspn(title, "\n")] = '\0';  // remove the newline character

            printf("year=");
            int year;
            scanf("%d", &year);

            printf("description=");
            char description[MAX_SHORT_LEN];
            getchar();
            fgets(description, MAX_SHORT_LEN, stdin);  // descriptions can have spaces     
            description[strcspn(description, "\n")] = '\0';  // remove the newline character

            printf("rating=");
            double rating;
            scanf("%lf", &rating);

            add_movie(title, year, description, rating);
        } else if (!strncmp(command, "delete_movie", 12)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            scanf("%s", id);

            delete_movie(id);
        } else if (!strncmp(command, "update_movie", 12)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            scanf("%s", id);

            printf("title=");
            char title[MAX_SHORT_LEN];
            getchar();
            fgets(title, MAX_SHORT_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            printf("year=");
            int year;
            scanf("%d", &year);

            printf("description=");
            char description[MAX_SHORT_LEN];
            getchar();
            fgets(description, MAX_SHORT_LEN, stdin);           
            description[strcspn(description, "\n")] = '\0';

            printf("rating=");
            double rating;
            scanf("%lf", &rating);

            update_movie(id, title, year, description, rating);
        } else if (!strncmp(command, "get_collections", 15)) {
            get_collections();
        } else if (!strncmp(command, "get_collection", 14)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            scanf("%s", id);

            get_collection(id);
        } else if (!strncmp(command, "add_collection", 14)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            getchar();
            fgets(title, MAX_SHORT_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            int num_movies;
            printf("num_movies=");
            scanf("%d", &num_movies);

            char *movie_id[num_movies];
            for (int i = 0; i < num_movies; i++) {
                printf("movie_id[%d]=", i);
                movie_id[i] = malloc(MAX_SHORT_LEN);
                scanf("%s", movie_id[i]);
            }

            add_collection(title, num_movies, movie_id);

            for (int i = 0; i < num_movies; i++) {
                free(movie_id[i]);
            }
        } else if (!strncmp(command, "delete_collection", 17)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            scanf("%s", id);

            delete_collection(id);
        } else if (!strncmp(command, "delete_movie_from_collection", 28)) {
            printf("collection_id=");
            char collection_id[MAX_SHORT_LEN];
            scanf("%s", collection_id);

            printf("movie_id=");
            char movie_id[MAX_SHORT_LEN];
            scanf("%s", movie_id);

            delete_movie_from_collection(collection_id, movie_id);
        } else if (!strncmp(command, "logout", 6)) {
            logout();
        } else if (!strncmp(command, "exit", 4)) {
            break;
        }
    }

    return 0;
}
