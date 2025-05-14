#pragma once

#include <faiss/Index.h>
#include <vector>

class FaissIndex {
    public:
        FaissIndex(faiss::Index* index);

        // 将单个向量数据和标签写入索引中
        void insert_vectors(const std::vector<float>& data, uint64_t label);

        // 查询与待查询向量最近邻的K个向量
        // 返回找到的向量标签和相应的距离
        std::pair<std::vector<long> , std::vector<float> > search_vectors(const std::vector<float>& query, int k);

    private:
        faiss::Index* index;
};