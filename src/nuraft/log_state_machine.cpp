#include "nuraft/log_state_machine.h"
#include "logger/logger.h"
#include "common/constants.h"

#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

using namespace nuraft;

void log_state_machine::setVectorDatabase(VectorDatabase* vector_database) {
    vector_database_ = vector_database;
    last_committed_idx_ = vector_database->getStartIndexID();
}

ptr<buffer> log_state_machine::commit(const ulong log_idx, buffer& data) {
    std::string content(reinterpret_cast<const char*>(data.data() + data.pos() + sizeof(int)), data.size() - sizeof(int));
    GlobalLogger->debug("Commit log_idx: {}, content: {}",log_idx, content);

    rapidjson::Document json_request;
    json_request.Parse(content.c_str());
    uint64_t label = json_request[REQUEST_ID].GetUint64();

    last_committed_idx_ = log_idx;

    IndexFactory::IndexType indexType = vector_database_->getIndexTypeFromRequest(json_request);

    vector_database_->upsert(label, json_request, indexType);

    // 返回raft日志编号
    ptr<buffer> ret = buffer::alloc(sizeof(log_idx));
    buffer_serializer bs(ret);
    bs.put_u64(log_idx);
    return ret;
}

ptr<buffer> log_state_machine::pre_commit(const ulong log_idx, buffer& data) {
    std::string content(reinterpret_cast<const char*>(data.data() + data.pos() + sizeof(int)), data.size() - sizeof(int));
    GlobalLogger->debug("Pre Commit log_idx: {}, content: {}",log_idx, content);
    return nullptr;
}