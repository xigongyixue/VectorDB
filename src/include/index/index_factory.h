#pragma once

#include "index/faiss_index.h"
#include <map>

// 管理不同的向量索引类型
class IndexFactory {
    public:

        enum class IndexType {
            FLAT,
            UNKNOWN = -1 
        };

        enum class MetricType {
            L2,
            IP
        };
    
        void init(IndexType type, int dim, MetricType  metric = MetricType::L2);
        void* getIndex(IndexType type) const;

    private:

        // 存储已初始化向量索引
        std::map<IndexType, void*> index_map;
};

IndexFactory* getGlobalIndexFactory();