#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <syslog.h>
#include <exception>
#include <sys/stat.h>
#include <fstream>
#include "DaemonManager.h"



int main(int argc, char* argv[]) {
    try {
        openlog("Kirpichenko_daemon", LOG_PID, LOG_DAEMON);
        pid_t chProc1 = fork();
        if (!chProc1) {
            setsid();
            pid_t chProc2 = fork(); // this proc will have init process as a parent when proc1 will be terminated
            if (!chProc2) {
                umask(0);
                DaemonManager::createNewSession();
            }
            closelog();
            return 0;
        }
    }
    catch (std::exception& e) {
        syslog(LOG_ERR, "An exception occured: %s", e.what());
        waitpid(0, nullptr, 0);
        closelog();
        return -1;
    }
    closelog();
    return 0;
}