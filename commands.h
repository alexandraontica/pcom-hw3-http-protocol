#ifndef __COMMANDS__
#define __COMMANDS__

#define MAX_LEN 50
#define MAX_BODY_LEN 500
#define HOST "63.32.125.183"
#define PORT 8081
#define CONTENT_TYPE "application/json"
#define BASE_URL "/api/v1/tema"
#define LOGIN_ADMIN_URL "/admin/login"

void login_admin(char *username, char *password);

#endif // __COMMANDS__