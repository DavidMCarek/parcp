#include <iostream>
#include <unistd.h>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {

    char *toDir = NULL;
    char *fromDir = NULL;

    int c;

    while ((c = getopt (argc, argv, "f:t:")) != -1) {
        switch (c) {
            case 't':
                toDir = optarg;
                break;
            case 'f':
                fromDir = optarg;
                break;

            default:
                abort();
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

    int waitCount = 0;

    while ((subDir = readdir(dir)) != NULL) {

        if (strcmp(".", subDir->d_name) == 0 || strcmp("..", subDir->d_name) == 0)
            continue;

        string from(fromDir);
        string to(toDir);

        from += "/";
        from += subDir->d_name;

        to += "/";
        to += subDir->d_name;

        if(subDir->d_type == DT_DIR) {

            waitCount++;

            if (fork() == 0) {

                char *args[256] = {
                        argv[0],
                        const_cast<char *>("-f"),
                        const_cast<char *>(from.c_str()),
                        const_cast<char *>("-t"),
                        const_cast<char *>(to.c_str()),
                        NULL
                };


                execv(argv[0], args);
            }
        }
        else {
            cout << "Copying: " << from << endl;

            std::ifstream src(from, std::ios::binary);
            std::ofstream dst(to, std::ios::binary);

            dst << src.rdbuf();

            src.close();
            dst.close();
        }
    }

    closedir(dir);

    for (int i = waitCount; i > 0; i--)
        wait(nullptr);

    return 0;
}
