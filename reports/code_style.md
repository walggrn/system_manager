# Отчет "Стили кодирования"

## Выбранный стиль кодирования
В качестве стиля кодирования был выбран **LLVM**. Для форматирования использовался **clang-format**.

## Использование
В настройках проекта были добавлены следующие строчки:
```
"editor.formatOnSave": true,
"C_Cpp.clang_format_fallbackStyle": "LLVM",
"C_Cpp.clang_format_style": "file",
```
После сохранения файлов проекта они автоматически были преобразованы в стиль **LLVM**.

## Пример
Был написан следующий класс:
```
#include <iostream>
#include <string>
#include <vector>

#include "commands/ICommand.h"
#include "utils/Log.h"
#include "utils/SystemManager.h"
#include "utils/Parser.h"

using namespace std;

class Example{
    private:
        string String;
        vector<int> VectorInt;
        int Int;

    public:
        Example() : String("string"), VectorInt({1, 2, 3, 4}), Int(1) {};

        string get_string() {
            return String;
        }

        vector<int> get_vector_int() {
            return VectorInt;
        }

        int get_vector_int_value(int& index){
            return VectorInt[index];
        }

        int get_int() {
            return Int;
        }
};
```
После сохранения код был преобразован для соответствия стилю кодирования **LLVM**.
```
#include <iostream>
#include <string>
#include <vector>

#include "commands/ICommand.h"
#include "utils/Log.h"
#include "utils/Parser.h"
#include "utils/SystemManager.h"

using namespace std;

class Example {
private:
  string String;
  vector<int> VectorInt;
  int Int;

public:
  Example() : String("string"), VectorInt({1, 2, 3, 4}), Int(1) {};

  string get_string() { return String; }

  vector<int> get_vector_int() { return VectorInt; }

  int get_vector_int_value(int &index) { return VectorInt[index]; }

  int get_int() { return Int; }
};
```