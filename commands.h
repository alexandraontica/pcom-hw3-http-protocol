#ifndef __COMMANDS__
#define __COMMANDS__

#define MAX_LEN 50
#define MAX_BODY_LEN 500
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
#define GET_ACCESS_URL "/library/access"

void login_admin(char *username, char *password);
void add_user(char *username, char *password);
void get_users();
void delete_user(char *username);
void logout_admin();
void login(char *admin_username, char *username, char *password);
void get_access();

#endif // __COMMANDS__