# Hướng Dẫn Cài Đặt Qt Trên Windows + VSCode

Qt là một framework mạnh mẽ để xây dựng giao diện đồ họa (GUI) và ứng dụng đa nền tảng. Hướng dẫn này giúp bạn cài đặt Qt và cấu hình môi trường để lập trình Qt bằng Visual Studio Code (VSCode) trên Windows.

---

## 🔧 Bước 1: Cài Đặt Qt (bằng Qt Online Installer)

1. Truy cập trang chính thức: [https://www.qt.io/download-qt-installer](https://www.qt.io/download-qt-installer)
2. Tải trình cài đặt: **Qt Online Installer for Windows**
3. Chạy file `.exe` và đăng nhập hoặc tạo tài khoản Qt miễn phí.
4. Khi được yêu cầu chọn phiên bản:
   - Chọn **Qt 6.x**
   - Chọn **MinGW (x64)** nếu bạn chưa có compiler.
   - Tích chọn **Qt Creator** nếu muốn IDE mặc định (không bắt buộc với VSCode).

> ⚠️ Ghi chú: Sau khi cài, Qt thường nằm tại `C:\Qt\`.

### ⚙️ Chi Tiết Cài Đặt

- Chọn **Next**.

![Ảnh hướng dẫn cài Qt](../UI/image/1.png)

- Nhập email của bạn. Nhấn **Submit**.

![Ảnh hướng dẫn cài Qt](../UI/image/2.png)

- Chọn **Disable seending usage statics**. Nhấn **Next**.

![Ảnh hướng dẫn cài Qt](../UI/image/3.png)

- Chọn **I have read and agree to the terms and conditions of using Open source qt** và **I'm an individual and do not use Qt for any company** . Nhấn **Next**.

![Ảnh hướng dẫn cài Qt](../UI/image/4.png)

- Chọn **Qt 6.9 for desktop developments** . Nhấn **Next**.

![Ảnh hướng dẫn cài Qt](../UI/image/5.png)

---

## ⚙️ Bước 2: Cài MinGW Compiler (nếu chưa có)

Qt với MinGW hoạt động tốt. Nếu chưa cài, bạn có thể:

- Cài kèm trong Qt Installer (tùy chọn "MinGW" trong bước chọn component)
- Hoặc tải riêng tại: [https://sourceforge.net/projects/mingw-w64/](https://sourceforge.net/projects/mingw-w64/)

> ✅ Kiểm tra: Sau khi cài, thêm đường dẫn `bin` vào biến môi trường PATH. Ví dụ:
>
> ```
> C:\Qt\Tools\mingw1120_64\bin
> ```

---

## 💻 Bước 3: Cài Đặt Visual Studio Code

1. Tải và cài: [https://code.visualstudio.com](https://code.visualstudio.com)
2. Cài các extension cần thiết:
   - **C/C++** (Microsoft)
   - **CMake Tools**
   - **CodeLLDB** (hoặc **Windows Debugger** nếu bạn muốn debug)
   - **Qt VS Tools** (tuỳ chọn)

---

## 🛠️ Bước 4: Cấu Hình Dự Án Qt Trong VSCode

1. **Tạo thư mục dự án**, ví dụ `MyQtApp`
2. Thêm các file sau:

### `main.cpp`

```cpp
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QLabel label("Hello, Qt from VSCode!");
    label.show();
    return app.exec();
}
```
