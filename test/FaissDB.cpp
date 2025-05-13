#include "FaissDB.h"
#include <iostream>

using namespace std;

FaissDB::FaissDB(int dim, const std::string &db_name)
{
	this->_db_name = db_name;
	if (std::filesystem::exists(db_name))
	{
		_indexIDMap_ptr = unique_ptr<faiss::Index>(faiss::read_index(db_name.c_str()));
		std::cout<<"load faiss data success from file"<<std::endl;
	}
	else
	{
		_indexHNSW_ptr = unique_ptr<faiss::IndexHNSWFlat>(new faiss::IndexHNSWFlat(dim, 200));
		_indexIDMap_ptr= unique_ptr<faiss::IndexIDMap>(new faiss::IndexIDMap(_indexHNSW_ptr.get()));
	}
}

FaissDB::~FaissDB()
{
}

int FaissDB::insert(int n, const std::vector<float>& vec, const std::vector<faiss::idx_t>& ids)
{
	std::lock_guard<std::mutex> lk(_rw_lock);
	_indexIDMap_ptr->add_with_ids(n, vec.data(), ids.data());
	faiss::write_index(_indexIDMap_ptr.get(),  _db_name.c_str());
	return 0;
}

int FaissDB::query(int n, const std::vector<float>& vec, int topk, std::vector<float>& distances,
	std::vector<faiss::idx_t>& indices)
{
	std::lock_guard<std::mutex> lk(_rw_lock);
	if (_indexIDMap_ptr->ntotal == 0)
	{
		std::cerr << "Index is empty. Please add data before searching." << std::endl;
		return -1;
	}
	_indexIDMap_ptr->search(n, vec.data(), topk, distances.data(), indices.data());

	return 0;
}

int FaissDB::remove(const std::vector<faiss::idx_t>& ids)
{
	faiss::IDSelectorBatch selector(ids.size(), ids.data());
	return _indexIDMap_ptr->remove_ids(selector);
}

