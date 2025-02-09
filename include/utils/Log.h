#ifndef LOG_H
#define LOG_H

#include <string>

/// @brief класс для создания логов
class Log {
private:
  std::string filename;

public:
  /// @brief инициализация объекта класса и создание имя файла лога
  /// @param name имя лога
  Log(const std::string &name);

  /// @brief запись строки в лог
  /// @param line строка для записи
  void write(const std::string &line);

  /// @brief получить расположения файла лога
  /// @return расположение лога
  std::string get_filename();
};

#endif