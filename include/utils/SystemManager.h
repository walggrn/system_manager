#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

enum TypeItem {
  File,
  Directory,
  Symlink,
  All,
};

/// @brief класс для работы с файлами и директориями
class SystemManager {
private:
  std::string get_type_string(const TypeItem type);

  /// @brief собираем массив строк в единую
  /// @param lines массив строк
  /// @param detail_output добавить номер строки и ее длину(по умолчанию нет)
  /// @return единая строка
  std::string concat_lines(const std::vector<std::string> &lines,
                           bool detail_output = false);

  /// @brief проверка существования объекта и соответствия типа
  /// @param path расположение объекта
  /// @param type тип(по умолчанию любой)
  void validate_path(const std::string &path, TypeItem type = All);

  /// @brief проверка объекта на соответстивие ожидаемого типа
  /// @param path расположение объекта
  /// @param type тип
  /// @return объект соответствует(true) - не соответствует(false)
  bool validate_type_object(const std::string &path, TypeItem type);

public:
  /// @brief вывод содержимого директории
  /// @param path расположение директории
  /// @param recursive выводить содержимое вложенных директорий
  /// @param depth максимальная глубина
  /// @param current_depth текущая глубина
  void list(const std::string &path, bool recursive = false, int depth = -1,
            int current_depth = 0);

  /// @brief копирование директории
  /// @param old_path расположение директории
  /// @param new_path расположение скопированной директории
  /// @param recreate пересоздать, если существует
  /// @param recursive копировать объекты из вложенных папок
  void copy_directory(const std::string &old_path, const std::string &new_path,
                      bool recreate = false, bool recursive = false);

  /// @brief копирование файла
  /// @param old_path расположение файла
  /// @param new_path расположение копированного файла
  /// @param recreate пересоздать, если существует
  void copy_file(const std::string &old_path, const std::string &new_path,
                 bool recreate = false);

  /// @brief создание директории
  /// @param path путь, где создать
  /// @param recreate пересоздать, если существует
  void create_directory(const std::string &path, bool recreate = false);

  /// @brief создание файла
  /// @param path путь, где создать
  /// @param recreate пересоздать, если существует
  void create_file(const std::string &path, bool recreate = false);

  /// @brief перемещение или переименование объекта
  /// @param old_path текущее расположение объекта
  /// @param new_path новое расположение объекта
  /// @param recreate пересоздать, если объект существует
  void modify(const std::string &old_path, const std::string &new_path,
              bool recreate = false);

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
  void find(std::vector<std::string> &matches,
            std::vector<std::string> &visited, const std::string &name,
            const std::string &path, bool contains = false, TypeItem type = All,
            int limit = -1, bool recursive = false, int depth = -1,
            bool log_visited = false, int current_depth = 0);

  /// @brief чтение данных файла
  /// @param path расположение файла
  /// @param detail_output детальный вывод
  /// @return данные файла
  std::string read(const std::string &path, bool detail_output = false);

  /// @brief вставка строки в файл)
  /// @param path расположение файла
  /// @param data данные для вставки
  /// @param number_line номер линии для вставки(в конец файла если не передан)
  /// @param skip пропуск символов перед вставкой(в конец строки если не
  /// передан)
  void insert(const std::string &path, const std::string &data,
              int number_line = -1, int skip = -1);

  /// @brief удаление директории
  /// @param path расположение директории
  void delete_directory(const std::string &path);

  /// @brief удаление файла
  /// @param path расположение файла
  void delete_file(const std::string &path);

  /// @brief удаление части данных из файла
  /// @param path расположение файла
  /// @param number_line номер линии для удаления
  /// @param skip пропуск символов до удаления
  /// @param length кол-во символов для удаления
  void delete_data_from_file(const std::string &path, int number_line = -1,
                             int skip = 0, int length = -1);
};

#endif