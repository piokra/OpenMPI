#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    std::string cmd;
    if(argc<3) return -1;
    cmd = "mpirun -np ";
    cmd+=argv[2];
    cmd+=" ";
    cmd+=argv[1];
    cmd+=" > out.log";

    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    system(cmd.c_str());
    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
}
