#pragma once

#include "scalar_storage.h"

#include <map>

// 管理不同的向量索引类型
class IndexFactory {
    public:

        enum class IndexType {
            FLAT,
            HNSW,
            FILTER,
            UNKNOWN = -1 
        };

        enum class MetricType {
            L2,
            IP
        };
    
        void init(IndexType type, int dim = 1, int num_data = 0, MetricType metric = MetricType::L2);
        void* getIndex(IndexType type) const;
        void saveIndex(const std::string& folder_path, ScalarStorage& scalar_storage);
        void loadIndex(const std::string& folder_path, ScalarStorage& scalar_storage);

    private:

        // 存储已初始化向量索引
        std::map<IndexType, void*> index_map;
};

IndexFactory* getGlobalIndexFactory();