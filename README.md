# ProxySwitch

## Project Purpose
The purpose of this project is to address the issue where certain Windows applications only recognize the HTTP proxy from environment variables instead of the proxy settings in Windows Internet settings. This tool allows you to set or clear proxy settings both in environment variables and Windows Internet settings.

## 项目目的
该项目的目的是解决某些Windows应用程序只识别环境变量中的HTTP代理，而不读取Windows Internet设置中的代理配置。此工具允许您在环境变量和Windows Internet设置中设置或清除代理设置。

## Functionality
- **Set Proxy Settings**: Sets the HTTP and HTTPS proxy settings in both environment variables and Windows Internet settings.
- **设置代理设置**：在环境变量和Windows Internet设置中设置HTTP和HTTPS代理设置。
- **Clear Proxy Settings**: Clears the HTTP and HTTPS proxy settings from both environment variables and Windows Internet settings.
- **清除代理设置**：从环境变量和Windows Internet设置中清除HTTP和HTTPS代理设置。

## Usage
### Setting Proxy
By default, the proxy settings are set to `127.0.0.1:7897` if no command is provided and proxy is not already set.

### 设置代理
默认情况下，如果没有提供命令且代理尚未设置，则代理设置为`127.0.0.1:7897`。

### Clearing Proxy
To clear the proxy settings, run the application with the `clear` command:

### 清除代理
要清除代理设置，请使用`clear`命令运行应用程序：

### 双击切换
双击应用程序图标即可实现代理设置的切换。如果代理已设置，则清除代理；如果代理未设置，则设置默认代理。

## Dependencies
- **Windows**: This application uses Windows-specific APIs and registry settings.
- **C++20**: This project is written in C++20.
- **CMake**: This project uses CMake and Ninja as the build system.

## 依赖项
- **Windows**：该应用程序使用特定于Windows的API和注册表设置。
- **C++20**：该项目使用C++20编写。
- **CMake**：该项目使用CMake和Ninja作为生成器。

## Building the Project
1. Ensure CMake and Ninja are installed.
2. Clone the repository.
3. Create a build directory and navigate to it.
4. Run CMake to configure the project:

## 构建项目
1. 确保已安装CMake和Ninja。
2. 克隆仓库。
3. 创建一个构建目录并导航到该目录。
4. 运行CMake配置项目：

5. Build the project:

5. 构建项目：

## Additional Information
- This project includes functionality for setting and getting environment variables, setting and clearing Windows Internet proxy settings, and checking if the proxy is set.
- The application also includes functionality to detect if it was launched by double-clicking (from Explorer).

## 其他信息
- 该项目包括设置和获取环境变量、设置和清除Windows Internet代理设置以及检查代理是否已设置的功能。
- 该应用程序还包括检测是否通过双击（从资源管理器）启动的功能。
