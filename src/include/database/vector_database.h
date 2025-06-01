#pragma once

#include "index/scalar_storage.h"
#include "index/index_factory.h"
#include "persistence/persistence.h"

#include <string>
#include <vector>

#include <rapidjson/document.h>

class VectorDatabase
{
    public:
        VectorDatabase(const std::string& db_path, const std::string& wal_path);

        // 写入或更新数据
        void upsert(uint64_t id, const rapidjson::Document& data, IndexFactory::IndexType index_type);

        // 根据id查询数据
        rapidjson::Document query(uint64_t id);

        // 使用向量索引做近似查找，结合过滤条件
        std::pair<std::vector<long>, std::vector<float>> search(const rapidjson::Document& json_request);

        void reloadDatabase();

        void writeWALLog(const std::string& operation_type, const rapidjson::Document& json_data);

        IndexFactory::IndexType getIndexTypeFromRequest(const rapidjson::Document& json_request);

    private:
        ScalarStorage scalar_storage_;
        Persistence persistence_;
};
