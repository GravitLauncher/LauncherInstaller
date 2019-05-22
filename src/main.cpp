#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
bool is_systemd = false;
bool is_debian = false;
bool is_arch = false;
bool is_centos = false;
bool is_root = false;
std::string download_url = "http://mirror.gravitlauncher.ml/build/";
bool is_file_exist(const char* file)
{
    struct stat sts;
    return !((stat (file, &sts)) == -1);
}
bool read_yes()
{
    std::string response;
    std::cin >> response;
    return (response == "yes" || response == "YES" || response == "Yes");
}
int root_action()
{
    std::cerr << "[WARN] Installing on behalf of the superuser is prohibited" << std::endl;
        is_root = true;
        std::cout << "[ROOT] Check package installation" << std::endl;
        if(!is_file_exist("/bin/unzip") && (is_debian || is_arch || is_centos))
        {
            std::cout << "[ROOT] Unzip not found. Install [YES/NO]? ";
            if(read_yes())
            {
                if(is_debian)
                {
                    system("apt-get install unzip");
                }
                else if(is_arch)
                {
                    system("pacman -S unzip");
                }
                else if(is_centos)
                {
                    system("yum install unzip");
                }
            }
        }
        if(!is_file_exist("/bin/curl") && (is_debian || is_arch || is_centos))
        {
            std::cout << "[ROOT] Curl not found. Install [YES/NO]? ";
            if(read_yes())
            {
                if(is_debian)
                {
                    system("apt-get install curl");
                }
                else if(is_arch)
                {
                    system("pacman -S curl");
                }
                else if(is_centos)
                {
                    system("yum install curl");
                }
            }
        }
        std::string response;
        std::cout << "Create new user for LaunchServer [YES/NO]? ";
        std::cin >> response;
        if(response == "yes" || response == "YES" || response == "Yes")
        {
            system("useradd -u 2001 -m launchserver");
            std::cout << "[ROOT] Create user launchserver" << std::endl;
            chdir("/home/launchserver");
            std::cout << "[ROOT] Change dir to /home/launchserver" << std::endl;
            setuid(2001);
        }
        else
        {
            return -1;
        }
        return 0;
}
int install_action()
{
    std::cout << "UID: " << getuid() << "" << std::endl;
    std::cout << "Download LaunchServer" << std::endl;
    std::string download_launchserver = "curl -o LaunchServer.jar "+download_url+"/LaunchServer.jar";
    system(download_launchserver.c_str());
    if(!is_file_exist("LaunchServer.jar"))
    {
        std::cerr << "[ERROR] LaunchServer.jar not found. May be curl not installed?";
        return -1;
    }
    std::cout << "Download libraries" << std::endl;
    std::string download_lib = "curl -o libraries.zip "+download_url+"/libraries.zip";
    system(download_lib.c_str());
    if(!is_file_exist("libraries.zip"))
    {
        std::cerr << "[ERROR] libraries.zip not found. May be curl not installed?";
        return -1;
    }
    std::cout << "Unpack libraries" << std::endl;
    system("unzip libraries.zip");
    if(!is_file_exist("libraries"))
    {
        std::cerr << "[ERROR] libraries not found. May be unzip not installed?";
        return -1;
    }
    system("rm libraries.zip");
    std::cout << "Installation test" << std::endl;
    system("java -javaagent:LaunchServer.jar -jar LaunchServer.jar multi build checkInstall stop");
    std::cout << "Chmod private.key to 700" << std::endl;
    chmod("private.key", S_IRUSR | S_IWUSR | S_IXUSR); // 700
    std::ofstream file;  
    file.open ("start.sh", std::ios::out | std::ios::app);
    file << "#!/bin/sh" << std::endl;
    std::cout << "Create start script. Using screen [YES/NO]? ";
    bool is_screen = false;
    if(read_yes())
    {
        is_screen = true;
        file << "screen -S launchserver java -javaagent:LaunchServer.jar -jar LaunchServer.jar";
    }
    else file << "java -javaagent:LaunchServer.jar -jar LaunchServer.jar";
    file << std::endl;
    file.close();
    chmod("start.sh", S_IRUSR | S_IWUSR | S_IXUSR); // 700 Что бы не запускали от рута
    std::cout << "Start script created!" << std::endl;
    if(is_root)
    {
        std::cout << "Switch to the user \"launchserver\"(su - launchserver) before using start.sh" << std::endl;
    }
    if(is_screen)
    {
        std::cout << "Screen mini help:" << std::endl;
        std::cout << "\"screen -x\" - switch to the last screen" << std::endl;
        std::cout << "\"Ctrl + A + D\" - exit screen" << std::endl;
    }
    return 0;
}
const char* avalible_options = "bm";
struct installerOptions
{
    bool isBuild = false;
    bool isNoInstall = false;
};
installerOptions opts;
int main(int argc, char **argv)
{
    int opt; // каждая следующая опция попадает сюда
    while((opt = getopt(argc, argv, avalible_options)) != -1) { // вызываем getopt пока она не вернет -1
        switch(opt) {
            case 'b': // Собирать из исходников
                opts.isBuild = true;
                break;
            case 'm': // Пропустить этап установки(например для установки модулей в уже установленный LaunchServer)
                opts.isNoInstall = true;
                break;
        }
    }
    std::cout << "GravitLauncher Linux installer" << std::endl;
    if(is_file_exist("/bin/apt-get"))
    {
        is_debian = true;
    }
    if(is_file_exist("/bin/pacman"))
    {
        is_arch = true;
    }
    if(is_file_exist("/bin/yum"))
    {
        is_centos = true;
    }
    if(getuid() == 0)
    {
        if(root_action() != 0) return -1;
    }
    std::cout << "Avalible versions: [dev, 5.0.0b6, 5.0.0b7]" << std::endl;
    std::cout << "Print version:";
    std::string version;
    std::cin >> version;
    if(version == "dev" || version == "5.0.0b6" || version == "5.0.0b7")
    {
        download_url = download_url + version;
    }
    else
    {
        std::cout << "Version " << version << " not found. Download dev[YES/NO]?";
        if(read_yes())
        {
            download_url = download_url + "dev";
        }
        else download_url = download_url + version;
    }
    if(!opts.isNoInstall && install_action() != 0) return -1;
    std::cout << "Download modules[YES/NO]?";
    if(read_yes())
    {
        if(is_file_exist("modules"))
            mkdir("modules", S_IRUSR | S_IWUSR | S_IXUSR);
        chdir("modules");
        std::string module;
        std::cout << "Print module name:";
        std::cin >> module;
        while(module != "" && module != "stop" && module != "exit")
        {
            std::string download_module = "curl -o \""+module+".jar\" \""+download_url+"/modules/"+module+"_module.jar\"";
            std::cout << "Download " << module << ".jar" << std::endl;
            int ret = system(download_module.c_str());
            if(ret != 0)
            {
                std::cerr << "[ERROR] Error download module " << module << std::endl;
                if(is_file_exist((module+".jar").c_str()))
                {
                    remove((module+".jar").c_str());
                }
            }
            std::cout << "Print module name:";
            std::cin >> module;
        }
        chdir("..");
    }
    if(is_root)
    {
        if(is_file_exist("/bin/bash"))
        {
            execl("/bin/bash", NULL);
        }
    }
}
