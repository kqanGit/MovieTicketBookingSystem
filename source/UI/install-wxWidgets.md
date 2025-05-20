# Hướng dẫn cài đặt wxWidgets trên Windows (MSYS2 UCRT64)

## 🧰 Yêu cầu trước khi bắt đầu

- Windows 10 hoặc 11
- [MSYS2](https://www.msys2.org/) đã cài đặt
- Đảm bảo bạn đang sử dụng **MSYS2 UCRT64** shell
- Visual Studio Code (nếu dùng để lập trình GUI)

---

## 🚀 Mở đúng terminal: MSYS2 UCRT64

Khi bạn mở đúng shell `MSYS2 UCRT64`, dòng nhắc lệnh sẽ có dạng:

```bash
Admin@TênMáy UCRT64 ~
```

---

## 📦 Bước 1: Cập nhật MSYS2 và cài g++/make

### 1.1 Cập nhật hệ thống:

```bash
pacman -Syu
# Nếu được yêu cầu, hãy đóng terminal và mở lại
pacman -Su
```

### 1.2 Kiểm tra `g++` và `make`:

```bash
g++ --version
make --version
```

Nếu thấy lỗi `command not found`, cài đặt như sau:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-make
```

Ngoài ra, nên cài thêm `git` nếu chưa có:

```bash
pacman -S git
```

---

## 📁 Bước 2: Tải source code wxWidgets

### Cách 1: Dùng Git (ưu tiên)

```bash
cd /c/Library
git clone https://github.com/wxWidgets/wxWidgets.git wxWidgets-3.2.8
cd wxWidgets-3.2.8
git checkout v3.2.8
```

> 📝 Bạn có thể thay `/c/Library` bằng bất kỳ thư mục nào khác nếu muốn. Đây chỉ là gợi ý.

### Cách 2: Tải từ trang chính thức (nếu không dùng Git)

- Vào: [https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.8](https://github.com/wxWidgets/wxWidgets/releases/tag/v3.2.8)
- Tải file `.zip`, giải nén vào thư mục như: `C:/Library/wxWidgets-3.2.8`

---

## 🔨 Bước 3: Build wxWidgets

```bash
cd wxWidgets-3.2.8/build/msw

# Build bản Release:
mingw32-make -f makefile.gcc BUILD=release UNICODE=1 SHARED=1

# Build bản Debug:
mingw32-make -f makefile.gcc BUILD=debug UNICODE=1 SHARED=1
```

> ⚠️ Quá trình này có thể mất từ 5–20 phút tùy cấu hình máy.

---

## ⚙️ Bước 4: Thêm `wx-config` vào PATH

```bash
export PATH=/c/Library/wxWidgets-3.2.8/build/msw:$PATH
```

> ✅ Để lệnh này có hiệu lực mỗi lần mở terminal, thêm vào file `~/.bash_profile` hoặc `~/.bashrc`:
>
> ```bash
> echo 'export PATH=/c/Library/wxWidgets-3.2.8/build/msw:$PATH' >> ~/.bashrc
> ```

---

## ✅ Bước 5: Kiểm tra

```bash
wx-config --version
```

Kết quả đúng là:

```
3.2.8
```

---

## 🧪 Bước 6: Viết chương trình wxWidgets đầu tiên

### Tạo file `testGUI.cpp`:

```cpp
#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        wxFrame* frame = new wxFrame(nullptr, wxID_ANY, "Hello wxWidgets!");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
```

---

## 🔧 Bước 7: Compile chương trình

```bash
cd /c/Users/Admin/Documents/code
g++ testGUI.cpp -o testGUI.exe `wx-config --cxxflags --libs`
```

---

## ▶️ Bước 8: Chạy chương trình

```bash
./testGUI.exe
```

Nếu thấy lỗi `Permission denied`, hãy thêm quyền thực thi:

```bash
chmod +x testGUI.exe
./testGUI.exe
```

---

## 💡 Gợi ý mở rộng: cấu hình VSCode

Tạo hoặc chỉnh file `.vscode/c_cpp_properties.json`:

```json
{
  "configurations": [
    {
      "name": "windows-gcc-x64",
      "includePath": [
        "${workspaceFolder}/**",
        "C:/Library/wxWidgets-3.2.8/include",
        "C:/Library/wxWidgets-3.2.8/lib/gcc_dll/mswud"
      ],
      "defines": ["_DEBUG", "UNICODE"],
      "compilerPath": "D:/MinGW/ucrt64/bin/g++.exe",
      "intelliSenseMode": "windows-gcc-x64",
      "cppStandard": "c++17"
    }
  ],
  "version": 4
}
```

---
