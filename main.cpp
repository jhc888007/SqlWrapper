#include "my_sql_wrapper_c.hpp"


int main() {
    char host[100] = "localhost";
    char user[100] = "jhc888007";
    char password[100] = "";
    char database[100] = "redis";
    int port = 0;
    MySqlWrapperC m(host, user, password, database, port);
    m.connect();
    return 0;
}
