#pragma once

#include "index/scalar_storage.h"
#include "index/index_factory.h"

#include <string>
#include <vector>

#include <rapidjson/document.h>

class VectorDatabase
{
    public:
        VectorDatabase(const std::string& db_path);

        // 写入或更新数据
        void upsert(uint64_t id, const rapidjson::Document& data, IndexFactory::IndexType index_type);

        // 查询数据
        rapidjson::Document query(uint64_t id);

    private:
        ScalarStorage scalar_storage_;
};
