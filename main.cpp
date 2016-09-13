#include <iostream>
#include <unistd.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {

    char *toDir;
    char *fromDir;

    int c;

    while ((c = getopt (argc, argv, "f:t:")) != -1) {
        switch (c) {
            case 't':
                toDir = optarg;
                break;
            case 'f':
                fromDir = optarg;
                break;
        }
    }

    if (!toDir || !fromDir) {
        cout << "Invalid option(s). Use -f <from_directory> -t <to_directory>" << endl;
        return 0;
    }

    DIR *dir = opendir(fromDir);
    mkdir(toDir, 0700);
    cout << "Created directory " << toDir << endl;

    dirent *subDir;
    subDir = readdir(dir);

    while (subDir) {

        string from(fromDir);
        string to(toDir);

        from += subDir->d_name;
        to += subDir->d_name;

        if(subDir->d_type == DT_DIR) {
            if (fork() == 0) {

                char *args[256] = {
                        argv[0],
                        static_cast<char *>("-f"),
                        static_cast<char *>(from.c_str()),
                        static_cast<char *>("-t"),
                        static_cast<char *>(to.c_str())
                };

                execv(argv[0], args);
            }
        }
        else {
            std::ifstream src(from, std::ios::binary);
            std::ofstream dst(to, std::ios::binary);

            dst << src.rdbuf();

            src.close();
            dst.close();
        }

        subDir = readdir(dir);
    }

    return 0;
}