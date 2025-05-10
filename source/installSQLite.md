# Các bước cài SQLite

## DOWNLOAD 
- Vào Link: https://www.sqlite.org/download.html dowload:
    + Source Code : sqlite-amalgamation-3490100.zip (Lấy thư viện sqlite3.h)
    + Precompiled Binaries for Windows : sqlite-tools-win-x64-3490100.zip (Để tạo file .db riêng từ file script .sql)
- Copy 2 thư mục sqlite3.c và sqlite3.h vào lib
- Copy tất cả thư mục trong sqlite-tools-win-x64-3490100 cùng vị trí với file main.cpp


## NOTE
- Muốn include file sqlite3.h thì cần phải include như sau
```c++ 
extern "C" {
    #include "sqlite3.h"
}
//để intellisense nhận biết đó là file header của .c
```

- Sau đó vào file main.cpp, mở terminal lên compile file sqlite3.c trước:
```bash
    gcc -c ./lib/sqlite3.c -o sqlite3.o
```

- Rồi mới compile file main.cpp

```bash
    g++ -I"./lib" -I"./database" main.cpp database/DatabaseConnection.cpp sqlite3.o -o main.exe
```

- Sau đó chạy file main.exe như bth