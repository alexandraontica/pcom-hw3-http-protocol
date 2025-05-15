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
        fgets(command, MAX_SHORT_LEN, stdin);
        command[strcspn(command, "\n")] = '\0';

        if (!strncmp(command, "login_admin", 11)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.");
                continue;
            }

            printf("password=");
            char password[MAX_SHORT_LEN];
            fgets(password, MAX_SHORT_LEN, stdin);
            password[strcspn(password, "\n")] = '\0';

            login_admin(username, password);
        } else if (!strncmp(command, "add_user", 8)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.");
                continue;
            }

            printf("password=");
            char password[MAX_SHORT_LEN];
            fgets(password, MAX_SHORT_LEN, stdin);
            password[strcspn(password, "\n")] = '\0';

            add_user(username, password);
        } else if (!strncmp(command, "get_users", 9)) {
            get_users();
        } else if (!strncmp(command, "delete_user", 11)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.");
                continue;
            }

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.");
                continue;
            }

            delete_user(username);
        } else if (!strncmp(command, "logout_admin", 12)) {
            logout_admin();
        } else if (!strncmp(command, "login", 5)) {
            printf("admin_username=");
            char admin_username[MAX_SHORT_LEN];
            fgets(admin_username, MAX_SHORT_LEN, stdin);
            admin_username[strcspn(admin_username, "\n")] = '\0';

            if (strchr(admin_username, ' ')) {
                printf("ERROR: Username-ul contine spatii.");
                continue;
            }

            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.");
                continue;
            }

            printf("password=");
            char password[MAX_SHORT_LEN];
            fgets(password, MAX_SHORT_LEN, stdin);
            password[strcspn(password, "\n")] = '\0';

            login(admin_username, username, password);
        } else if (!strncmp(command, "get_access", 10)) {
            get_access();
        } else if (!strncmp(command, "get_movies", 10)) {
            get_movies();
        } else if (!strncmp(command, "get_movie", 9)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            int id_int;
            char endptr;
            int ret = sscanf(id, "%d%c", &id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. ID is an integer\n");
                continue;
            }

            get_movie(id);
        }  else if (!strncmp(command, "add_movie_to_collection", 23)) {
            printf("collection_id=");
            char collection_id[MAX_SHORT_LEN];
            fgets(collection_id, MAX_SHORT_LEN, stdin);
            collection_id[strcspn(collection_id, "\n")] = '\0';

            int collection_id_int;
            char endptr;
            int ret = sscanf(collection_id, "%d%c", &collection_id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Collection ID is an integer\n");
                continue;
            }

            printf("movie_id=");
            char movie_id_str[MAX_SHORT_LEN];
            fgets(movie_id_str, MAX_SHORT_LEN, stdin);
            movie_id_str[strcspn(movie_id_str, "\n")] = '\0';

            int movie_id;
            ret = sscanf(movie_id_str, "%d%c", &movie_id, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Movie ID is an integer\n");
                continue;
            }

            add_movie_to_collection(collection_id, movie_id);
        } else if (!strncmp(command, "add_movie", 9)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            fgets(title, MAX_SHORT_LEN, stdin);  // titles can have spaces
            title[strcspn(title, "\n")] = '\0';  // remove the newline character

            printf("year=");
            char year_str[MAX_SHORT_LEN];
            fgets(year_str, MAX_SHORT_LEN, stdin);
            year_str[strcspn(year_str, "\n")] = '\0';

            int year;
            char endptr;
            int ret = sscanf(year_str, "%d%c", &year, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Year is an integer\n");
                continue;
            }

            printf("description=");
            char description[MAX_SHORT_LEN];
            fgets(description, MAX_SHORT_LEN, stdin);  // descriptions can have spaces     
            description[strcspn(description, "\n")] = '\0';  // remove the newline character

            printf("rating=");
            char rating_str[MAX_SHORT_LEN];
            fgets(rating_str, MAX_SHORT_LEN, stdin);
            rating_str[strcspn(rating_str, "\n")] = '\0';

            double rating;
            ret = sscanf(rating_str, "%lf%c", &rating, &endptr);

            // if (ret != 1 || endptr != '\0') {
            //     printf("Invalid input. Rating is a double\n");
            //     continue;
            // }

            add_movie(title, year, description, rating);
        } else if (!strncmp(command, "delete_movie_from_collection", 28)) {
            printf("collection_id=");
            char collection_id[MAX_SHORT_LEN];
            fgets(collection_id, MAX_SHORT_LEN, stdin);
            collection_id[strcspn(collection_id, "\n")] = '\0';

            int collection_id_int;
            char endptr;
            int ret = sscanf(collection_id, "%d%c", &collection_id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Collection ID is an integer\n");
                continue;
            }

            printf("movie_id=");
            char movie_id[MAX_SHORT_LEN];
            fgets(movie_id, MAX_SHORT_LEN, stdin);
            movie_id[strcspn(movie_id, "\n")] = '\0';

            int movie_id_int;
            ret = sscanf(movie_id, "%d%c", &movie_id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Movie ID is an integer\n");
                continue;
            }

            delete_movie_from_collection(collection_id, movie_id);
        } else if (!strncmp(command, "delete_movie", 12)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            int id_int;
            char endptr;
            int ret = sscanf(id, "%d%c", &id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. ID is an integer\n");
                continue;
            }

            delete_movie(id);
        } else if (!strncmp(command, "update_movie", 12)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            int id_int;
            char endptr;
            int ret = sscanf(id, "%d%c", &id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. ID is an integer\n");
                continue;
            }

            printf("title=");
            char title[MAX_SHORT_LEN];
            fgets(title, MAX_SHORT_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            printf("year=");
            char year_str[MAX_SHORT_LEN];
            fgets(year_str, MAX_SHORT_LEN, stdin);
            year_str[strcspn(year_str, "\n")] = '\0';

            int year;
            ret = sscanf(year_str, "%d%c", &year, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Year is an integer\n");
                continue;
            }

            printf("description=");
            char description[MAX_SHORT_LEN];
            fgets(description, MAX_SHORT_LEN, stdin);           
            description[strcspn(description, "\n")] = '\0';

            printf("rating=");
            char rating_str[MAX_SHORT_LEN];
            fgets(rating_str, MAX_SHORT_LEN, stdin);
            rating_str[strcspn(rating_str, "\n")] = '\0';

            double rating;
            ret = sscanf(rating_str, "%lf%c", &rating, &endptr);

            // if (ret != 1 || endptr != '\0') {
            //     printf("Invalid input. Rating is a double\n");
            //     continue;
            // }

            update_movie(id, title, year, description, rating);
        } else if (!strncmp(command, "get_collections", 15)) {
            get_collections();
        } else if (!strncmp(command, "get_collection", 14)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            int id_int;
            char endptr;
            int ret = sscanf(id, "%d%c", &id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. ID is an integer\n");
                continue;
            }

            get_collection(id);
        } else if (!strncmp(command, "add_collection", 14)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            fgets(title, MAX_SHORT_LEN, stdin);           
            title[strcspn(title, "\n")] = '\0';

            int num_movies;
            printf("num_movies=");
            char num_movies_str[MAX_SHORT_LEN];
            fgets(num_movies_str, MAX_SHORT_LEN, stdin);
            num_movies_str[strcspn(num_movies_str, "\n")] = '\0';

            char endptr;
            int ret = sscanf(num_movies_str, "%d%c", &num_movies, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. Number of movies is an integer\n");
                continue;
            }

            char *movie_id[num_movies];
            for (int i = 0; i < num_movies; i++) {
                printf("movie_id[%d]=", i);
                movie_id[i] = malloc(MAX_SHORT_LEN);
                fgets(movie_id[i], MAX_SHORT_LEN, stdin);
                movie_id[i][strcspn(movie_id[i], "\n")] = '\0';

                int movie_id_int;
                char endptr;
                int ret = sscanf(movie_id[i], "%d%c", &movie_id_int, &endptr);

                if (ret != 1 || endptr != '\0') {
                    printf("Invalid input. Movie ID is an integer\n");
                    continue;
                }
            }

            add_collection(title, num_movies, movie_id);

            for (int i = 0; i < num_movies; i++) {
                free(movie_id[i]);
            }
        } else if (!strncmp(command, "delete_collection", 17)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            int id_int;
            char endptr;
            int ret = sscanf(id, "%d%c", &id_int, &endptr);

            if (ret != 1 || endptr != '\0') {
                printf("Invalid input. ID is an integer\n");
                continue;
            }

            delete_collection(id);
        } else if (!strncmp(command, "logout", 6)) {
            logout();
        } else if (!strncmp(command, "exit", 4)) {
            break;
        }
    }

    return 0;
}
