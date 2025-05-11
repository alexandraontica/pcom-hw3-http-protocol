#ifndef __COMMANDS__
#define __COMMANDS__

#define MAX_SHORT_LEN 50
#define MAX_LONG_LEN 500
#define HOST "63.32.125.183"
#define PORT 8081
#define CONTENT_TYPE "application/json"
#define BASE_URL "/api/v1/tema"
#define LOGIN_ADMIN_URL "/admin/login"
#define ADD_USER_URL "/admin/users"
#define GET_USERS_URL ADD_USER_URL
#define DELETE_USER_URL ADD_USER_URL
#define LOGOUT_ADMIN_URL "/admin/logout"
#define LOGIN_URL "/user/login"
#define LOGOUT_URL "/user/logout"
#define GET_ACCESS_URL "/library/access"
#define GET_MOVIES_URL "/library/movies"
#define GET_MOVIE_URL GET_MOVIES_URL
#define ADD_MOVIE_URL GET_MOVIES_URL
#define DELETE_MOVIE_URL GET_MOVIES_URL
#define UPDATE_MOVIE_URL GET_MOVIES_URL
#define GET_COLLECTIONS_URL "/library/collections"
#define GET_COLLECTION_URL GET_COLLECTIONS_URL
#define ADD_COLLECTION_URL GET_COLLECTIONS_URL
#define ADD_MOVIE_TO_COLLECTION_FIRST_URL GET_COLLECTIONS_URL
#define ADD_MOVIE_TO_COLLECTION_SECOND_URL "/movies"
#define DELETE_COLLECTION_URL GET_COLLECTIONS_URL
#define DELETE_MOVIE_FROM_COLLECTION_FIRST_URL GET_COLLECTIONS_URL
#define DELETE_MOVIE_FROM_COLLECTION_SECOND_URL ADD_MOVIE_TO_COLLECTION_SECOND_URL

void login_admin(char *username, char *password);
void add_user(char *username, char *password);
void get_users();
void delete_user(char *username);
void logout_admin();
void login(char *admin_username, char *username, char *password);
void logout();
void get_access();
void get_movies();
void get_movie(char *id);
void add_movie(char *title, int year, char *description, double rating);
void delete_movie(char *id);
void update_movie(char *id, char *title, int year, char *description, double rating);
void get_collections();
void get_collection(char *id);
int add_empty_collection(char *title, int *id);
int add_movie_to_collection_aux(char *collection_id, int movie_id);
void add_collection(char *title, int num_movies, char *movie_id[]);
void add_movie_to_collection(char *collection_id, int movie_id);
void delete_collection(char *id);
void delete_movie_from_collection(char *collection_id, char *movie_id);

#endif // __COMMANDS__