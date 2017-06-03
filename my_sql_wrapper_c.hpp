#include <string>
#include <mysql/mysql.h>
#include <stdio.h>
#include <vector>

class MySqlWrapperC {
    std::string _host;
    std::string _user;
    std::string _password;
    std::string _database;
    int _port;
    
    MYSQL _instance;

    bool _connected;

    int _errno;
    const char* _error;

    void error() {
        _errno = mysql_errno(&_instance);
        _error = mysql_error(&_instance);
    }
public:
    MySqlWrapperC(const char *host, const char *user, const char *password, const char *database, int port):
        _host(host), _user(user), _password(password), _database(database), _port(port), _connected(false) {
            mysql_library_init(0, NULL, NULL);
            mysql_init(&_instance);
            mysql_options(&_instance, MYSQL_SET_CHARSET_NAME, "gbk");
        }
    ~MySqlWrapperC(){}

    bool connect() {
        if (_connected) return false;
        if (mysql_real_connect(&_instance, _host.c_str(), _user.c_str(), _password.c_str(), _database.c_str(),
            _port, 0, 0) != NULL) {
            _connected = true;
            return true;
        }
        error();
        return false;
    }
    void close() {
        if (_connected == false) return;
        mysql_close(&_instance);
    }

    bool useDatabase(const char *database_name) {
        std::string query = "create database if not exists ";
        query += std::string(database_name);
        if (0 == mysql_query(&_instance, query.c_str())) {
            query = "use ";
            query += std::string(database_name);
            if (0 == mysql_query(&_instance, query.c_str())) {
                return true;
            }
        }
        error();
        return false;
    }

    bool command(const char *query) {
        if (0 == mysql_query(&_instance, query)) {
            return true;
        }
        error();
        return false;
    }

    bool get(const char *query, std::vector<std::vector<std::string> > &data) {
        if (0 != mysql_query(&_instance, query)) {
            error();
            return false;
        }

        data.clear();

        MYSQL_RES *result = mysql_store_result(&_instance);
        int field = mysql_num_fields(result);
        MYSQL_ROW line = NULL;
        line = mysql_fetch_row(result);

        while (NULL != line) {
            std::vector<std::string> linedata;
            for (int i = 0; i < field; i++) {
                if (line[i]) {
                    linedata.push_back(std::string(line[i]));
                } else {
                    linedata.push_back(std::string(""));
                }
            }
            line = mysql_fetch_row(result);
            data.push_back(linedata);
        }
        return true;
    }

    void dumpError() {
        printf("Errno: %d, error: %s\n", _errno, _error);
    }
};

