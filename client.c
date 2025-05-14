#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

void empty_stdin_buffer() {
    // clear the input buffer to remove any leftover characters
    int c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

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
            int id_int;
            int ret = scanf("%d", &id_int);

            if (ret != 1) {
                printf("Invalid input. ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char id[MAX_SHORT_LEN];
            sprintf(id, "%d", id_int);

            get_movie(id);
        }  else if (!strncmp(command, "add_movie_to_collection", 23)) {
            printf("collection_id=");
            int collection_id_int;
            int ret = scanf("%d", &collection_id_int);

            if (ret != 1) {
                printf("Invalid input. Collection ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char collection_id[MAX_SHORT_LEN];
            sprintf(collection_id, "%d", collection_id_int);

            printf("movie_id=");
            int movie_id;
            ret = scanf("%d", &movie_id);

            if (ret != 1) {
                printf("Invalid input. Movie ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            add_movie_to_collection(collection_id, movie_id);
        } else if (!strncmp(command, "add_movie", 9)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            getchar();  // read the newline character left by scanf
            fgets(title, MAX_SHORT_LEN, stdin);  // titles can have spaces
            title[strcspn(title, "\n")] = '\0';  // remove the newline character

            printf("year=");
            int year;
            int ret = scanf("%d", &year);

            if (ret != 1) {
                printf("Invalid input. Year is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            printf("description=");
            char description[MAX_SHORT_LEN];
            getchar();
            fgets(description, MAX_SHORT_LEN, stdin);  // descriptions can have spaces     
            description[strcspn(description, "\n")] = '\0';  // remove the newline character

            printf("rating=");
            double rating;
            ret = scanf("%lf", &rating);

            // if (ret != 1) {
            //     printf("Invalid input. Rating is a double\n");
            //     empty_stdin_buffer();
            //     continue;
            // }

            add_movie(title, year, description, rating);
        } else if (!strncmp(command, "delete_movie", 12)) {
            printf("id=");
            int id_int;
            int ret = scanf("%d", &id_int);

            if (ret != 1) {
                printf("Invalid input. ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char id[MAX_SHORT_LEN];
            sprintf(id, "%d", id_int);

            delete_movie(id);
        } else if (!strncmp(command, "update_movie", 12)) {
            printf("id=");
            int id_int;
            int ret = scanf("%d", &id_int);

            if (ret != 1) {
                printf("Invalid input. ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char id[MAX_SHORT_LEN];
            sprintf(id, "%d", id_int);

            printf("title=");
            char title[MAX_SHORT_LEN];
            getchar();
            fgets(title, MAX_SHORT_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            printf("year=");
            int year;
            ret = scanf("%d", &year);

            if (ret != 1) {
                printf("Invalid input. Year is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            printf("description=");
            char description[MAX_SHORT_LEN];
            getchar();
            fgets(description, MAX_SHORT_LEN, stdin);           
            description[strcspn(description, "\n")] = '\0';

            printf("rating=");
            double rating;
            ret = scanf("%lf", &rating);

            // this is commented out because I don't know why it doesn't work
            // if (ret != 1) {
            //     printf("Invalid input. Rating is a double\n");
            //     empty_stdin_buffer();
            //     continue;
            // }

            update_movie(id, title, year, description, rating);
        } else if (!strncmp(command, "get_collections", 15)) {
            get_collections();
        } else if (!strncmp(command, "get_collection", 14)) {
            printf("id=");
            int id_int;
            int ret = scanf("%d", &id_int);

            if (ret != 1) {
                printf("Invalid input. ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char id[MAX_SHORT_LEN];
            sprintf(id, "%d", id_int);

            get_collection(id);
        } else if (!strncmp(command, "add_collection", 14)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            getchar();
            fgets(title, MAX_SHORT_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            int num_movies;
            printf("num_movies=");
            int ret = scanf("%d", &num_movies);

            if (ret != 1) {
                printf("Invalid input. Number of movies is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char *movie_id[num_movies];
            for (int i = 0; i < num_movies; i++) {
                printf("movie_id[%d]=", i);

                int movie_id_int;
                ret = scanf("%d", &movie_id_int);

                if (ret != 1) {
                    printf("Invalid input. Movie ID is an integer\n");
                    empty_stdin_buffer();

                    i--;
                    continue;
                }

                movie_id[i] = malloc(MAX_SHORT_LEN);
                sprintf(movie_id[i], "%d", movie_id_int);
            }

            add_collection(title, num_movies, movie_id);

            for (int i = 0; i < num_movies; i++) {
                free(movie_id[i]);
            }
        } else if (!strncmp(command, "delete_collection", 17)) {
            printf("id=");
            int id_int;
            int ret = scanf("%d", &id_int);

            if (ret != 1) {
                printf("Invalid input. ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char id[MAX_SHORT_LEN];
            sprintf(id, "%d", id_int);

            delete_collection(id);
        } else if (!strncmp(command, "delete_movie_from_collection", 28)) {
            printf("collection_id=");
            int collection_id_int;
            int ret = scanf("%d", &collection_id_int);

            if (ret != 1) {
                printf("Invalid input. Collection ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char collection_id[MAX_SHORT_LEN];
            sprintf(collection_id, "%d", collection_id_int);

            printf("movie_id=");
            int movie_id_int;
            ret = scanf("%d", &movie_id_int);

            if (ret != 1) {
                printf("Invalid input. Collection ID is an integer\n");
                empty_stdin_buffer();
                continue;
            }

            char movie_id[MAX_SHORT_LEN];
            sprintf(movie_id, "%d", movie_id_int);

            delete_movie_from_collection(collection_id, movie_id);
        } else if (!strncmp(command, "logout", 6)) {
            logout();
        } else if (!strncmp(command, "exit", 4)) {
            break;
        }
    }

    return 0;
}
