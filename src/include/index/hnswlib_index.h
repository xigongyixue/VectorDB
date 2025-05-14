#pragma once

#include "index/index_factory.h"
#include "hnswlib/hnswlib.h"
#include <vector>

class HNSWLibIndex {
    public:
        // dim：向量维度 num_data：索引容纳向量个数 metric：距离度量类型 M: 索引节点最大近邻数  ef_construction：构建最大近邻时最大候选邻居数（优先队列大小）
        HNSWLibIndex(int dim, int num_data, IndexFactory::MetricType metric, int M = 16, int ef_construction = 200);

        void insert_vectors(const std::vector<float>& data, uint64_t label);

        // 单个向量查询
        std::pair<std::vector<long>, std::vector<float> > search_vectors(const std::vector<float>& query, int k, int ef_search = 50);

    private:
        int dim;
        hnswlib::SpaceInterface<float>* space; // 计算向量之间相似度
        hnswlib::HierarchicalNSW<float>* index; // 索引本身，负责存储数据和执行查询
};