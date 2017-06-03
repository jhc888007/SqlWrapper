#include <mysql/mysql.h>

class MySqlWrapperC {
    const char* _host;
    const char* _user;
    const char* _password;
    const char* _database;
    int _port;
    
    MYSQL _instance;
    MYSQL_RES *_result;

    int _errno;
    const char* _error;

    void error() {
        _errno = mysql_errno(&_instance);
        _error = mysql_error(&_instance);
    }
public:
    MySqlWrapperC(const char *host, const char *user, const char *password, const char *database, int port):
        _host(host), _user(user), _password(password), _database(database), _port(port) {}
    ~MySqlWrapperC(){}

    bool connect() {
        if (mysql_real_connect(&_instance, _host, _user, _password, _database, _port, 0, 0) != NULL)
            return true;
        else
            error();
        return false;
    }
};

