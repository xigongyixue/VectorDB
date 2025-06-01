#pragma once

#include <string>
#include <fstream>
#include <cstdint>
#include <rapidjson/document.h>

class Persistence
{
    public:
        Persistence(): increaseID_(1) { };
        ~Persistence();

        // 初始化日志存储路径
        void init(const std::string& local_path);

        uint64_t increaseID();

        uint64_t getID() const;

        // 将一条操作日志写入预写日志文件，记录操作类型、JSON和版本信息
        void writeWALLog(const std::string& operation_type, const rapidjson::Document& json_data, const std::string& version);
        
        // 读取下一条预写日志
        void readNextWALLog(std::string* operation_type, rapidjson::Document* json_data);

    private:
        uint64_t increaseID_; // 唯一自增ID
        std::fstream wal_log_file_; // 记录操作日志
};