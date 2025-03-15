# ProxySwitch

## Project Purpose
The purpose of this project is to address the issue where certain Windows applications only recognize the HTTP proxy from environment variables instead of the proxy settings in Windows Internet settings. This tool allows you to set or clear proxy settings both in environment variables and Windows Internet settings.

## ��ĿĿ��
����Ŀ��Ŀ���ǽ��ĳЩWindowsӦ�ó���ֻʶ�𻷾������е�HTTP����������ȡWindows Internet�����еĴ������á��˹����������ڻ���������Windows Internet���������û�����������á�

## Functionality
- **Set Proxy Settings**: Sets the HTTP and HTTPS proxy settings in both environment variables and Windows Internet settings.
- **���ô�������**���ڻ���������Windows Internet����������HTTP��HTTPS�������á�
- **Clear Proxy Settings**: Clears the HTTP and HTTPS proxy settings from both environment variables and Windows Internet settings.
- **�����������**���ӻ���������Windows Internet���������HTTP��HTTPS�������á�

## Usage
### Setting Proxy
By default, the proxy settings are set to `127.0.0.1:7897` if no command is provided and proxy is not already set.

### ���ô���
Ĭ������£����û���ṩ�����Ҵ�����δ���ã����������Ϊ`127.0.0.1:7897`��

### Clearing Proxy
To clear the proxy settings, run the application with the `clear` command:

### �������
Ҫ����������ã���ʹ��`clear`��������Ӧ�ó���

### ˫���л�
˫��Ӧ�ó���ͼ�꼴��ʵ�ִ������õ��л���������������ã�����������������δ���ã�������Ĭ�ϴ���

## Dependencies
- **Windows**: This application uses Windows-specific APIs and registry settings.
- **C++20**: This project is written in C++20.
- **CMake**: This project uses CMake and Ninja as the build system.

## ������
- **Windows**����Ӧ�ó���ʹ���ض���Windows��API��ע������á�
- **C++20**������Ŀʹ��C++20��д��
- **CMake**������Ŀʹ��CMake��Ninja��Ϊ��������

## Building the Project
1. Ensure CMake and Ninja are installed.
2. Clone the repository.
3. Create a build directory and navigate to it.
4. Run CMake to configure the project:

## ������Ŀ
1. ȷ���Ѱ�װCMake��Ninja��
2. ��¡�ֿ⡣
3. ����һ������Ŀ¼����������Ŀ¼��
4. ����CMake������Ŀ��

5. Build the project:

5. ������Ŀ��

## Additional Information
- This project includes functionality for setting and getting environment variables, setting and clearing Windows Internet proxy settings, and checking if the proxy is set.
- The application also includes functionality to detect if it was launched by double-clicking (from Explorer).

## ������Ϣ
- ����Ŀ�������úͻ�ȡ�������������ú����Windows Internet���������Լ��������Ƿ������õĹ��ܡ�
- ��Ӧ�ó��򻹰�������Ƿ�ͨ��˫��������Դ�������������Ĺ��ܡ�
