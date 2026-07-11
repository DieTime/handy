#ifndef HANDY_TESTS_LOG_TEMP_FILE_HPP
#define HANDY_TESTS_LOG_TEMP_FILE_HPP

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>

class TempFile {
public:
    TempFile() {
        char tmpl[] = "/tmp/handy_log_test_XXXXXX";
        int fd = mkstemp(tmpl);
        close(fd);
        path_ = tmpl;
    }

    ~TempFile() { remove(path_.c_str()); }

    TempFile(const TempFile &) = delete;
    TempFile &operator=(const TempFile &) = delete;

    const std::string &path() const { return path_; }

    std::string contents() const {
        std::ifstream f(path_, std::ios::binary);
        std::ostringstream ss;
        ss << f.rdbuf();
        return ss.str();
    }

private:
    std::string path_;
};

#endif /* HANDY_TESTS_LOG_TEMP_FILE_HPP */
