#include "persistence/persistence.h"
#include "logger/logger.h" 

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h> 

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Persistence::~Persistence() {
    if (wal_log_file_.is_open()) {
        wal_log_file_.close();
    }
}

void Persistence::init(const std::string& local_path) {
    wal_log_file_.open(local_path, std::ios::in | std::ios::out | std::ios::app); // 以 std::ios::in | std::ios::out | std::ios::app 模式打开文件
    if (!wal_log_file_.is_open()) {
        GlobalLogger->error("An error occurred while writing the WAL log entry. Reason: {}", std::strerror(errno)); // 使用日志打印错误消息和原因
        throw std::runtime_error("Failed to open WAL log file at path: " + local_path);
    }
}


uint64_t Persistence::increaseID() {
    increaseID_++;
    return increaseID_;
}

uint64_t Persistence::getID() const {
    return increaseID_;
}

void Persistence::writeWALLog(const std::string& operation_type, const rapidjson::Document& json_data, const std::string& version) {
    uint64_t log_id = increaseID();

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    json_data.Accept(writer);

    // 写入日志
    wal_log_file_ << log_id << "|" << version << "|" << operation_type << "|" << buffer.GetString() << std::endl;

    if (wal_log_file_.fail()) { // 检查是否发生错误
        GlobalLogger->error("An error occurred while writing the WAL log entry. Reason: {}", std::strerror(errno)); // 使用日志打印错误消息和原因
    } else {
       GlobalLogger->debug("Wrote WAL log entry: log_id={}, version={}, operation_type={}, json_data_str={}", log_id, version, operation_type, buffer.GetString()); // 打印日志
       wal_log_file_.flush(); // 强制持久化
    }
}

void Persistence::readNextWALLog(std::string* operation_type, rapidjson::Document* json_data) {
    GlobalLogger->debug("Reading next WAL log entry");
    std::string line;
    if(std::getline(wal_log_file_, line)) {
        std::istringstream iss(line);
        std::string log_id_str, version, json_data_str;
        std::getline(iss, log_id_str, '|');
        std::getline(iss, version, '|');
        std::getline(iss, *operation_type, '|');
        std::getline(iss, json_data_str, '|');
        uint64_t log_id = std::stoull(log_id_str);

        // 如果日志id大于当前id，则更新
        if(log_id > increaseID_) {
            increaseID_ = log_id;
        }

        // 返回json数据
        json_data->Parse(json_data_str.c_str());

        GlobalLogger->debug("Read WAL log entry: log_id={}, operation_type={}, json_data_str={}", log_id_str, *operation_type, json_data_str);
    } else {
        // 到达文件末尾
        wal_log_file_.clear();
        GlobalLogger->debug("No more WAL log entries to read");
    }
}