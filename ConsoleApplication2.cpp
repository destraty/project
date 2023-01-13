#include <iostream>
#include <string>
#include "include/json.hpp"
#include "include/httplib.h"
using namespace httplib;
using namespace std;
using json = nlohmann::json;
string cash_check() {
start:
    Client time("http://worldtimeapi.org");
    auto res = time.Get("/api/timezone/Europe/Simferopol");
    while (!res || res->status != 200) {
        res = time.Get("/api/timezone/Europe/Simferopol");
    }   
    int utm;
    if (res && res->status == 200) utm = json::parse(res->body)["unixtime"];
    ifstream file("widget.html");
    string str, cash_data;
    getline(file, str, '\0'); 
    ifstream cash_file("cash.txt");
    getline(cash_file, cash_data, '\0');
    json j2 = json::parse(cash_data);
    for (int k = 0; k < j2["list"].size(); k++) { 
        int cur_tm = j2["list"][k]["dt"]; cout << to_string(cur_tm) + ' ' + to_string(utm) + '\n';
        if (cur_tm >= utm) { cout << "Cash was taken from file\n";
            str.replace(str.find("{list[i].weather[0].description}"), 32, j2["list"][k]["weather"][0]["description"]);
            str.replace(str.find("{list[i].weather[0].icon}"), 25, j2["list"][k]["weather"][0]["icon"]);
            str.replace(str.find("{list[i].main.temp}"), 19, to_string((int)round(j2["list"][k]["main"]["temp"].get<double>())));
            str.replace(str.find("{list[i].main.temp}"), 19, to_string((int)round(j2["list"][k]["main"]["temp"].get<double>())));
            cash_file.close();
            return str;
        }
        else {
            Client cli("http://api.openweathermap.org");
            auto get_weather = cli.Get("/data/2.5/forecast?q=Simferopol&units=metric&lang=ru&appid=616d89ba57860508805151a5b38d0c08");
            if (get_weather && get_weather->status == 200) {
                    Client cli("http://api.openweathermap.org");
                    auto get_weather = cli.Get("/data/2.5/forecast?q=Simferopol&units=metric&lang=ru&appid=616d89ba57860508805151a5b38d0c08");
                    ofstream fout; cout << "Api request was sended to weather server \n";
                    fout.open("cash.txt", std::ofstream::out | std::ofstream::trunc);
                    fout << get_weather->body;
                    fout.close();
                    goto start;
            }
        }
    }
}

int main() {
    Server svr; // Создаём сервер
    svr.Get("/", [](const httplib::Request& req, httplib::Response& get_weather) {get_weather.set_content(cash_check(), "text/html");}); std::cout << "Start server... OK\n";
    svr.listen("192.168.31.46", 25567);
}
