#pragma once

#include <string>
#include <rocksdb/db.h>
#include <rapidjson/document.h>

// 标量存储引擎，基于标量标签查询向量数据
class ScalarStorage {
    public:
        // db_path: 数据库文件路径
        ScalarStorage(const std::string& db_path);

        ~ScalarStorage();

        // id: 向量id  data: 向量数据
        void insert_scalar(uint64_t id, const rapidjson::Document& data);

        rapidjson::Document get_scalar(uint64_t id);

    private:
        rocksdb::DB* db_;
};