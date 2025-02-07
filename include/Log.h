#ifndef LOG_H
#define LOG_H

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "Exceptions.h"

using namespace std;
namespace fs = std::filesystem;

/// @brief класс для создания логов
class Log {
private:
  string filename;

public:
  /// @brief инициализация объекта класса и создание имя файла лога
  /// @param name имя лога
  Log(const string &name) {
    // создаем директорию для логов если нет
    if (!fs::exists("./logs")) {
      fs::create_directory("./logs");
    }

    // создаем "уникальное" имя для лога
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    ostringstream oss;
    oss << put_time(localtime(&now), "%Y-%m-%d_%H-%M-%S");
    filename = "./logs/" + oss.str() + "_" + name + ".log";
  }

  /// @brief запись строки в лог
  /// @param line строка для записи
  void write(const string &line) {
    try {
      // в начале для записи использовался пользовательский класс SystemManager
      ofstream log_file(filename, ios::app);
      if (!log_file.is_open()) {
        throw runtime_error(FILE_OPEN_EXCEPTION +
                            "Failed to open log file: " + filename);
      }
      log_file << line << endl;
    } catch (const exception &e) {
      throw runtime_error(FILE_WRITE_EXCEPTION +
                          "Error writing to log: " + e.what());
    }
  }

  /// @brief получить расположения файла лога
  /// @return расположение лога
  string get_filename() { return filename; }
};

#endif