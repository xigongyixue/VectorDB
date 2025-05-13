#include "FaissDB.h"
#include <iostream>
using namespace std;

int main(){
	const int d = 128;
    const int n = 5;
    std::vector<float> data(d * n);

    for (int i = 0; i < d * n; ++i) {
        data[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    std::vector<faiss::idx_t> ids = {101, 102, 103, 104, 105};

    int M = 16;
    FaissDB faiss_db(d, "faiss.index");
	faiss_db.insert(n, data, ids);

    const int k = 3;
    std::vector<float> query(d);

    for (int i = 0; i < d; ++i) {
        query[i] = static_cast<float>(rand()) / RAND_MAX;
    }

    std::vector<faiss::idx_t> result_ids(k);
    std::vector<float> distances(k);

    faiss_db.query(1, query, k, distances, result_ids);

    std::cout << "search result:" << std::endl;
    for (int i = 0; i < k; ++i) {
        std::cout << "vector ID: " << result_ids[i] << ", dis: " << distances[i] << std::endl;
    }
    
	return 0;
}
