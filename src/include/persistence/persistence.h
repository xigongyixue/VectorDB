#pragma once

#include <string>
#include <fstream>
#include <cstdint>
#include <rapidjson/document.h>

#include "index/scalar_storage.h"

class Persistence
{
    public:
        Persistence(): increaseID_(1), lastSnapshotID_(0) { };
        ~Persistence();

        // 初始化日志存储路径
        void init(const std::string& local_path);

        uint64_t increaseID();

        uint64_t getID() const;

        // 将一条操作日志写入预写日志文件，记录操作类型、JSON和版本信息
        void writeWALLog(const std::string& operation_type, const rapidjson::Document& json_data, const std::string& version);
        
        // 读取下一条预写日志
        void readNextWALLog(std::string* operation_type, rapidjson::Document* json_data);

        void takeSnapshot(ScalarStorage& scalar_storage);
        void loadSnapshot(ScalarStorage& scalar_storage);
        void saveLastSnapshotID();
        void loadLastSnapshotID();

    private:
        uint64_t increaseID_; // 唯一自增ID
        uint64_t lastSnapshotID_; // 快照存储的最后一个预写日志id
        std::fstream wal_log_file_; // 记录操作日志

        const std::string snapshot_folder_path = "snapshots_"; // 快照所在的文件夹
        const std::string snapshots_MaxLogID_path = "snapshots_MaxLogID"; // 快照id存储文件
};