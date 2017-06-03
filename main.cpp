#include <stdio.h>
#include <iostream>
#include <algorithm>

#include "my_sql_wrapper_c.hpp"
#include "my_sql_wrapper_cpp_conn.hpp"

void test_c(void) {
    char host[100] = "localhost";
    char user[100] = "jhc888007";
    char password[100] = "";
    char database[100] = "redis";
    int port = 0;
    MySqlWrapperC m(host, user, password, database, port);

    if (m.connect() == false) {
        m.dumpError();
        return;
    }

    m.useDatabase("redis");
    m.command("create table AAA(code int, name char(20))");
    m.command("drop table AAA");
    std::vector<std::vector<std::string> > data;
    m.get("select * from events_all_time", data);
    for_each(data.begin(), data.end(), [](std::vector<std::string> v)->void {
        for_each(v.begin(), v.end(), [](std::string &s)->void {
            std::cout << s << " ";
        });
        std::cout << std::endl;
    });

    m.close();
    return;
}

void test_cpp() {
    MySqlWrapperCppConn m("localhost", "jhc888007", "");

    if (m.connect() == false) {
        return;
    }

    bool ret;
    ret = m.command("use redis");
    if (!ret) {
        std::cout << "error" << std::endl;
        return;
    }
    ret = m.command("select * from events_all_time");

    m.close();
}

int main() {
    test_c();
    test_cpp();
    return 0;
}
