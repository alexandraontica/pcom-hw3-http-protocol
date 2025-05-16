#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

int is_int(char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
}

int is_float(char *input) {
    int found_dot = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '.') {
            if (found_dot) {
                return 0;
            }
            found_dot = 1;
        } else if (input[i] < '0' || input[i] > '9') {
            return 0;
        }
    }
    return 1;
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
                printf("ERROR: Username-ul contine spatii.\n");
                continue;
            }

            printf("password=");
            char password[MAX_SHORT_LEN];
            fgets(password, MAX_SHORT_LEN, stdin);
            password[strcspn(password, "\n")] = '\0';

            if (strchr(password, ' ')) {
                printf("ERROR: Parola contine spatii.\n");
                continue;
            }

            login_admin(username, password);
        } else if (!strncmp(command, "add_user", 8)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.\n");
                continue;
            }

            printf("password=");
            char password[MAX_SHORT_LEN];
            fgets(password, MAX_SHORT_LEN, stdin);
            password[strcspn(password, "\n")] = '\0';

            if (strchr(password, ' ')) {
                printf("ERROR: Parola contine spatii.\n");
                continue;
            }

            add_user(username, password);
        } else if (!strncmp(command, "get_users", 9)) {
            get_users();
        } else if (!strncmp(command, "delete_user", 11)) {
            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.\n");
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
                printf("ERROR: Username-ul contine spatii.\n");
                continue;
            }

            printf("username=");
            char username[MAX_SHORT_LEN];
            fgets(username, MAX_SHORT_LEN, stdin);
            username[strcspn(username, "\n")] = '\0';

            if (strchr(username, ' ')) {
                printf("ERROR: Username-ul contine spatii.\n");
                continue;
            }

            printf("password=");
            char password[MAX_SHORT_LEN];
            fgets(password, MAX_SHORT_LEN, stdin);
            password[strcspn(password, "\n")] = '\0';

            if (strchr(password, ' ')) {
                printf("ERROR: Parola contine spatii.\n");
                continue;
            }

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

            if (!is_int(id)) {
                printf("ERROR: Id trebuie sa fie un numar natural.\n");
                continue;
            }

            get_movie(id);
        }  else if (!strncmp(command, "add_movie_to_collection", 23)) {
            printf("collection_id=");
            char collection_id[MAX_SHORT_LEN];
            fgets(collection_id, MAX_SHORT_LEN, stdin);
            collection_id[strcspn(collection_id, "\n")] = '\0';

            if (!is_int(collection_id)) {
                printf("ERROR: Id-ul colectiei trebuie sa fie un numar natural.\n");
                continue;
            }

            printf("movie_id=");
            char movie_id_str[MAX_SHORT_LEN];
            fgets(movie_id_str, MAX_SHORT_LEN, stdin);
            movie_id_str[strcspn(movie_id_str, "\n")] = '\0';

            if (!is_int(movie_id_str)) {
                printf("ERROR: Id-ul filmului trebuie sa fie un numar natural.\n");
                continue;
            }

            int movie_id;
            sscanf(movie_id_str, "%d", &movie_id);

            add_movie_to_collection(collection_id, movie_id);
        } else if (!strncmp(command, "add_movie", 9)) {
            printf("title=");
            char title[MAX_SHORT_LEN];
            fgets(title, MAX_SHORT_LEN, stdin);  // titles can have spaces
            title[strcspn(title, "\n")] = '\0';

            printf("year=");
            char year_str[MAX_SHORT_LEN];
            fgets(year_str, MAX_SHORT_LEN, stdin);
            year_str[strcspn(year_str, "\n")] = '\0';

            if (!is_int(year_str)) {
                printf("ERROR: Anul trebuie sa fie un numar natural.\n");
                continue;
            }

            int year;
            sscanf(year_str, "%d", &year);

            printf("description=");
            char description[MAX_SHORT_LEN];
            fgets(description, MAX_SHORT_LEN, stdin);  // descriptions can have spaces     
            description[strcspn(description, "\n")] = '\0';

            printf("rating=");
            char rating_str[MAX_SHORT_LEN];
            fgets(rating_str, MAX_SHORT_LEN, stdin);
            rating_str[strcspn(rating_str, "\n")] = '\0';

            if (!is_float(rating_str)) {
                printf("ERROR: Rating-ul trebuie sa fie un numar real.\n");
                continue;
            }

            float rating;
            sscanf(rating_str, "%f", &rating);

            add_movie(title, year, description, rating);
        } else if (!strncmp(command, "delete_movie_from_collection", 28)) {
            printf("collection_id=");
            char collection_id[MAX_SHORT_LEN];
            fgets(collection_id, MAX_SHORT_LEN, stdin);
            collection_id[strcspn(collection_id, "\n")] = '\0';

            if (!is_int(collection_id)) {
                printf("ERROR: Id-ul colectiei trebuie sa fie un numar natural.\n");
                continue;
            }

            printf("movie_id=");
            char movie_id[MAX_SHORT_LEN];
            fgets(movie_id, MAX_SHORT_LEN, stdin);
            movie_id[strcspn(movie_id, "\n")] = '\0';

            if (!is_int(movie_id)) {
                printf("ERROR: Id-ul filmului trebuie sa fie un numar natural.\n");
                continue;
            }

            delete_movie_from_collection(collection_id, movie_id);
        } else if (!strncmp(command, "delete_movie", 12)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            if (!is_int(id)) {
                printf("ERROR: Id-ul filmului trebuie sa fie un numar natural.\n");
                continue;
            }

            delete_movie(id);
        } else if (!strncmp(command, "update_movie", 12)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            if (!is_int(id)) {
                printf("ERROR: Id-ul filmului trebuie sa fie un numar natural.\n");
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

            if (!is_int(year_str)) {
                printf("ERROR: Anul trebuie sa fie un numar natural.\n");
                continue;
            }

            int year;
            sscanf(year_str, "%d", &year);

            printf("description=");
            char description[MAX_SHORT_LEN];
            fgets(description, MAX_SHORT_LEN, stdin);           
            description[strcspn(description, "\n")] = '\0';

            printf("rating=");
            char rating_str[MAX_SHORT_LEN];
            fgets(rating_str, MAX_SHORT_LEN, stdin);
            rating_str[strcspn(rating_str, "\n")] = '\0';

            if (!is_float(rating_str)) {
                printf("ERROR: Rating-ul trebuie sa fie un numar real.\n");
                continue;
            }

            float rating;
            sscanf(rating_str, "%f", &rating);

            update_movie(id, title, year, description, rating);
        } else if (!strncmp(command, "get_collections", 15)) {
            get_collections();
        } else if (!strncmp(command, "get_collection", 14)) {
            printf("id=");
            char id[MAX_SHORT_LEN];
            fgets(id, MAX_SHORT_LEN, stdin);
            id[strcspn(id, "\n")] = '\0';

            if (!is_int(id)) {
                printf("ERROR: Id-ul colectiei trebuie sa fie un numar natural.\n");
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

            if (!is_int(num_movies_str)) {
                printf("ERROR: Numarul de filme trebuie sa fie natural.\n");
                continue;
            }

            sscanf(num_movies_str, "%d", &num_movies);

            char *movie_id[num_movies];
            for (int i = 0; i < num_movies; i++) {
                printf("movie_id[%d]=", i);
                movie_id[i] = malloc(MAX_SHORT_LEN);
                fgets(movie_id[i], MAX_SHORT_LEN, stdin);
                movie_id[i][strcspn(movie_id[i], "\n")] = '\0';

                if (!is_int(movie_id[i])) {
                    printf("ERROR: Id-ul filmului trebuie sa fie un numar natural.\n");
                    free(movie_id[i]);
                    i--;
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

            if (!is_int(id)) {
                printf("ERROR: Id-ul colectiei trebuie sa fie un numar natural.\n");
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
