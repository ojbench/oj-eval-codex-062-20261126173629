#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>
#include <string>

template<class T, int info_len = 2>
class MemoryRiver {
private:
    std::string file_name;

public:
    MemoryRiver() = default;
    MemoryRiver(const std::string &name) : file_name(name) {}

    void initialise(std::string name = "") {
        if (!name.empty()) file_name = name;
        std::ofstream file(file_name, std::ios::binary | std::ios::trunc);
        const int zero = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<const char *>(&zero), sizeof(zero));
    }

    void get_info(int &value, int n) {
        if (n < 1 || n > info_len) return;
        std::ifstream file(file_name, std::ios::binary);
        file.seekg(static_cast<std::streamoff>(n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&value), sizeof(value));
    }

    void write_info(int value, int n) {
        if (n < 1 || n > info_len) return;
        std::fstream file(file_name, std::ios::binary | std::ios::in | std::ios::out);
        file.seekp(static_cast<std::streamoff>(n - 1) * sizeof(int));
        file.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }

    int write(T &value) {
        std::ofstream file(file_name, std::ios::binary | std::ios::app);
        file.seekp(0, std::ios::end);
        const int index = static_cast<int>(file.tellp());
        file.write(reinterpret_cast<const char *>(&value), sizeof(T));
        return index;
    }

    void update(T &value, const int index) {
        std::fstream file(file_name, std::ios::binary | std::ios::in | std::ios::out);
        file.seekp(index);
        file.write(reinterpret_cast<const char *>(&value), sizeof(T));
    }

    void read(T &value, const int index) {
        std::ifstream file(file_name, std::ios::binary);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&value), sizeof(T));
    }

    // This version deliberately does not reclaim deleted records.
    void Delete(int) {}
};

#endif
