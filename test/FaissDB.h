#ifndef FACERECOGNITION_CPP_FAISSDB_H
#define FACERECOGNITION_CPP_FAISSDB_H
//faiss
#include <faiss/IndexHNSW.h>
#include <faiss/IndexIDMap.h>
#include <faiss/index_io.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <filesystem>

class FaissDB {
public:
	FaissDB(int dim, const std::string &db_name);
	~FaissDB();
	int insert(int n, const std::vector<float> &vec, const std::vector<faiss::idx_t> &ids);
	int query(int n, const std::vector<float> &vec, int topk, std::vector<float> &distances, std::vector<faiss::idx_t> &indices);
	int remove(const std::vector<faiss::idx_t> &ids);
private:
	std::unique_ptr<faiss::IndexHNSWFlat> _indexHNSW_ptr;
	std::unique_ptr<faiss::Index> _indexIDMap_ptr;
	std::mutex _rw_lock;
	std::string _db_name;
};

#endif //FAISSDB_H
