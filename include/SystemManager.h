#ifndef FILE_H
#define FILE_H

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "Exceptions.h"

using namespace std;
namespace fs = filesystem;

enum TypeItem {
  File,
  Directory,
  Symlink,
  All,
};

/// @brief класс для работы с файлами и директориями
class SystemManager {
private:
  string get_type_string(const TypeItem type) {
    switch (type) {
    case File:
      return "File";
    case Directory:
      return "Directory";
    case Symlink:
      return "Symlink";
    case All:
      return "Any";
    default:
      return "";
    }
  }

  /// @brief собираем массив строк в единую
  /// @param lines массив строк
  /// @param detail_output добавить номер строки и ее длину(по умолчанию нет)
  /// @return единая строка
  string concat_lines(const vector<string> &lines, bool detail_output = false) {
    stringstream concated_data;
    for (size_t i = 0; i < lines.size(); i++) {
      if (detail_output) {
        concated_data << i + 1 << ": " << lines[i] << " :" << lines[i].size()
                      << ((i == lines.size() - 1) ? "" : "\n");
      } else {
        concated_data << lines[i] << ((i == lines.size() - 1) ? "" : "\n");
      }
    }
    return concated_data.str();
  }

  /// @brief проверка существования объекта и соответствия типа
  /// @param path расположение объекта
  /// @param type тип(по умолчанию любой)
  void validate_path(const string &path, TypeItem type = All) {
    if (!fs::exists(path)) {
      throw_error(
          OBJECT_NOT_EXIST_EXCEPTION,
          format("Object not found at the specified path: \"{}\".", path));
    }
    if (!validate_type_object(path, type)) {
      throw_error(INVALID_TYPE_OBJECT_EXCEPTION,
                  format("Object at the specified path: \"{}\" type does not "
                         "match the expected type. Expected type: {}.",
                         path, get_type_string(type)));
    }
  }

  /// @brief проверка объекта на соответстивие ожидаемого типа
  /// @param path расположение объекта
  /// @param type тип
  /// @return объект соответствует(true) - не соответствует(false)
  bool validate_type_object(const string &path, TypeItem type) {
    switch (type) {
    case File:
      return fs::is_regular_file(path);
    case Directory:
      return fs::is_directory(path);
    case Symlink:
      return fs::is_symlink(path);
    case All:
      return true;
    default:
      return false;
    }
  }

public:
  /// @brief вывод содержимого директории
  /// @param path расположение директории
  /// @param recursive выводить содержимое вложенных директорий
  /// @param depth максимальная глубина
  /// @param current_depth текущая глубина
  void list(const string &path, bool recursive = false, int depth = -1,
            int current_depth = 0) {
    if (depth != -1 && current_depth > depth)
      return;
    vector<string> directories = {};
    fs::directory_iterator it;
    try {
      it = fs::directory_iterator(path);
    } catch (const fs::filesystem_error &e) {
      write_error(DIRECTORY_OPEN_EXCEPTION, e.what());
      return;
    }
    cout << "Current directory: " << path << "\n" << endl;
    cout << format("{:<20} {:<30} {:<20} {:<}", "Type", "Last Write Time",
                   "Size(kb)", "Name")
         << endl;
    cout << string(100, '-') << endl;
    cout << endl;
    for (const auto &entry : it) {
      string path = entry.path().string();
      string time = "-";
      string type = "-";
      string size = "-";
      if (validate_type_object(path, Directory)) {
        type = "Directory";
        directories.push_back(path);
      } else if (validate_type_object(path, File)) {
        type = "File";
        auto ftime = fs::last_write_time(path);
        auto sctp =
            chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now() +
                std::chrono::system_clock::now());
        time_t cftime = std::chrono::system_clock::to_time_t(sctp);
        time = ctime(&cftime);
        time.pop_back();
        size = to_string(fs::file_size(path) / 1024);
      } else if (validate_type_object(path, Symlink)) {
        type = "Symlink";
      } else {
        type = "Unknown";
      }
      cout << format("{:<20} {:<30} {:<20} {:<}", type, time, size, path)
           << endl;
    }
    cout << "\n";
    if (recursive)
      for (const auto &dir : directories) {
        list(dir, recursive, depth, current_depth + 1);
      }
  }

  /// @brief копирование директории
  /// @param old_path расположение директории
  /// @param new_path расположение скопированной директории
  /// @param recreate пересоздать, если существует
  /// @param recursive копировать объекты из вложенных папок
  void copy_directory(const string &old_path, const string &new_path,
                      bool recreate = false, bool recursive = false) {
    try {
      validate_path(old_path, Directory);
      if (fs::exists(new_path)) {
        if (recreate)
          fs::remove_all(new_path);
        else
          throw_error(
              DIRECTORY_ALREADY_EXISTS_EXCEPTION,
              format("Directory at the specified path: \"{}\" already exists.",
                     new_path));
      }
      if (recursive)
        fs::copy(old_path, new_path, fs::copy_options::recursive);
      else
        fs::copy(old_path, new_path);
    } catch (const fs::filesystem_error &e) {
      throw_error(FILESYSTEM_EXCEPTION, e.what());
    }
  }

  /// @brief копирование файла
  /// @param old_path расположение файла
  /// @param new_path расположение копированного файла
  /// @param recreate пересоздать, если существует
  void copy_file(const string &old_path, const string &new_path,
                 bool recreate = false) {
    try {
      validate_path(old_path, File);
      if (fs::exists(new_path)) {
        if (recreate)
          fs::remove(new_path);
        else
          throw_error(
              FILE_ALREADY_EXISTS_EXCEPTION,
              format("File at the specified path: \"{}\" already exists.",
                     new_path));
      }
      fs::copy_file(old_path, new_path);
    } catch (const fs::filesystem_error &e) {
      throw_error(FILESYSTEM_EXCEPTION, e.what());
    }
  }

  /// @brief создание директории
  /// @param path путь, где создать
  /// @param recreate пересоздать, если существует
  void create_directory(const string &path, bool recreate = false) {
    try {
      if (fs::exists(path)) {
        if (recreate)
          fs::remove_all(path);
        else
          throw_error(
              DIRECTORY_ALREADY_EXISTS_EXCEPTION,
              format("Directory at the specified path: \"{}\" already exists.",
                     path));
      }
      fs::create_directories(path);
    } catch (const fs::filesystem_error &e) {
      throw_error(FILESYSTEM_EXCEPTION, e.what());
    }
  }

  /// @brief создание файла
  /// @param path путь, где создать
  /// @param recreate пересоздать, если существует
  void create_file(const string &path, bool recreate = false) {
    if (fs::exists(path)) {
      if (recreate)
        fs::remove(path);
      else
        throw_error(
            FILE_ALREADY_EXISTS_EXCEPTION,
            format("File at the specified path: \"{}\" already exists.", path));
    }
    ofstream new_file(path, ios::out);
    if (!new_file.is_open()) {
      throw_error(
          FILE_OPEN_EXCEPTION,
          format("File at the specified path: \"{}\" cannot be opened.", path));
    }
    new_file.close();
  }

  /// @brief перемещение или переименование объекта
  /// @param old_path текущее расположение объекта
  /// @param new_path новое расположение объекта
  /// @param recreate пересоздать, если объект существует
  void modify(const string &old_path, const string &new_path,
              bool recreate = false) {
    try {
      validate_path(old_path);
      if (fs::exists(new_path)) {
        if (recreate) {
          fs::remove_all(new_path);
        } else {
          throw_error(
              OBJECT_ALREADY_EXISTS_EXCEPTION,
              format("Object at the specified path: \"{}\" already exists.",
                     new_path));
        }
      }
      fs::rename(old_path, new_path);
    } catch (const fs::filesystem_error &e) {
      throw_error(FILESYSTEM_EXCEPTION, e.what());
    }
  }

  /// @brief метод для поиска объектов по имени
  /// @param matches массив для совпадений
  /// @param visited массив посещенных директорий
  /// @param what имя, по которому поиск
  /// @param where расположение директории, в которой ищем
  /// @param type тип объекта, который ищем
  /// @param recursive_traversal рекурсивный поиск
  /// @param limit ограничение количества совпадений
  /// @param max_depth максимальная глубина поиска в вложенных директориях
  /// @param log_visited сохранять посещенные директории
  /// @param current_depth текущий уровень глубины
  void find(vector<string> &matches, vector<string> &visited,
            const string &name, const string &path, bool contains = false,
            const TypeItem type = All, int limit = -1, bool recursive = false,
            int depth = -1, bool log_visited = false, int current_depth = 0) {
    try {
      if (depth != -1 && depth < current_depth)
        return;
      fs::directory_iterator current_dir;
      try {
        current_dir = fs::directory_iterator(path);
      } catch (const fs::filesystem_error &e) {
        write_error(FILESYSTEM_EXCEPTION, e.what());
      }
      for (const auto &entry : current_dir) {
        string path = entry.path().string();
        if (entry.path().filename().compare(name) == 0 ||
            (contains &&
             entry.path().filename().string().find(name) != string::npos)) {
          if (limit != -1 && limit <= matches.size()) {
            return;
          }
          if (validate_type_object(path, type)) {
            matches.push_back(path);
          }
        }
        if (recursive && validate_type_object(path, Directory)) {
          if (log_visited) {
            visited.push_back(path);
          }
          find(matches, visited, name, path, contains, type, limit, recursive,
               depth, log_visited, current_depth + 1);
        }
      }
    } catch (const fs::filesystem_error &e) {
      throw_error(FILESYSTEM_EXCEPTION, e.what());
    }
  }

  /// @brief чтение данных файла
  /// @param path расположение файла
  /// @param detail_output детальный вывод
  /// @return данные файла
  string read(const string &path, bool detail_output = false) {
    validate_path(path, File);
    ifstream file(path);
    vector<string> lines;
    if (!file.is_open()) {
      throw_error(
          FILE_OPEN_EXCEPTION,
          format("File at the specified path: \"{}\" cannot be opened.", path));
    }
    string line;
    while (getline(file, line))
      lines.push_back(line);
    file.close();
    string data = concat_lines(lines, detail_output);
    return data;
  }

  /// @brief вставка строки в файл)
  /// @param path расположение файла
  /// @param data данные для вставки
  /// @param number_line номер линии для вставки(в конец файла если не передан)
  /// @param skip пропуск символов перед вставкой(в конец строки если не
  /// передан)
  void insert(const string &path, const string &data, int number_line = -1,
              int skip = -1) {
    validate_path(path, File);
    fstream file(path, ios::in);
    vector<string> lines;
    if (!file.is_open()) {
      throw_error(FILE_OPEN_EXCEPTION,
                  format("File at the specified path: \"{}\" cannot be opened "
                         "for reading.",
                         path));
    }
    string line;
    while (getline(file, line))
      lines.push_back(line);
    file.close();
    if (number_line == -1 || number_line > lines.size()) {
      lines.push_back("");
      number_line = lines.size();
    }
    if (skip == -1 || skip > lines[number_line - 1].size()) {
      skip = lines[number_line - 1].size();
    }
    lines[number_line - 1].insert(skip, data);
    file.open(path, ios::out | ios::trunc);
    if (!file.is_open()) {
      throw_error(FILE_OPEN_EXCEPTION,
                  format("File at the specified path: \"{}\" cannot be opened "
                         "for insertion.",
                         path));
    }
    string concated_data = concat_lines(lines);
    file << concated_data;
    file.close();
  }

  /// @brief удаление директории
  /// @param path расположение директории
  void delete_directory(const string &path) {
    validate_path(path, Directory);
    for (const auto &entry : fs::directory_iterator(path)) {
      if (fs::is_directory(entry)) {
        delete_directory(entry.path().string());
      } else {
        fs::remove(entry);
      }
    }
    fs::remove(path);
  }

  /// @brief удаление файла
  /// @param path расположение файла
  void delete_file(const string &path) {
    validate_path(path, File);
    fs::remove_all(path);
  }

  /// @brief удаление части данных из файла
  /// @param path расположение файла
  /// @param number_line номер линии для удаления
  /// @param skip пропуск символов до удаления
  /// @param length кол-во символов для удаления
  void delete_data_from_file(const string &path, int number_line = -1,
                             int skip = 0, int length = -1) {
    validate_path(path, File);
    fstream file(path, ios::in | ios::out);
    vector<string> lines;
    if (!file.is_open()) {
      throw_error(FILE_OPEN_EXCEPTION,
                  format("File at the specified path: \"{}\" cannot be opened "
                         "for reading.",
                         path));
    }
    string line;
    while (getline(file, line))
      lines.push_back(line);
    file.close();
    if (number_line < 0 || number_line > lines.size()) {
      throw_error(INVALID_KEY_VALUE_EXCEPTION,
                  "Number line cannot be less than 0 or more than the number "
                  "of lines in the file.");
    }
    if (skip < 0 || skip >= lines[number_line].size()) {
      throw_error(INVALID_KEY_VALUE_EXCEPTION,
                  "The number of symbols to skip cannot be less than 0 or more "
                  "than the length of the line symbols.");
    }
    if (length == -1 || (skip + length) > lines[number_line - 1].size()) {
      length = lines[number_line - 1].size() - skip;
    }
    lines[number_line - 1].erase(skip, length);
    if (lines[number_line - 1].size() == 0) {
      lines.erase(lines.begin() + number_line - 1);
    }
    string data = concat_lines(lines);
    file.open(path, ios::out);
    file << data;
    file.close();
  }
};

#endif