#pragma once

#include "in_memory_state_mgr.h"
#include "log_state_machine.h"
#include "raft_logger_wrapper.h"
#include "logger/logger.h"

#include <libnuraft/asio_service.hxx>

#include "database/vector_database.h"


// raft信息统一管理
class RaftStuff {
    public:
        RaftStuff(int node_id, const std::string& endpoint, int port, VectorDatabase* vector_database);

        void init();

        // 向集群添加新节点
        ptr< cmd_result< ptr<buffer> > > addSrv(int srv_id, const std::string& srv_endpoint);

        void enableElectionTimeout(int lower_bound, int upper_bound);

        // 判断是否为主节点
        bool isLeader() const;

        std::vector<std::tuple<int, std::string, std::string, nuraft::ulong, nuraft::ulong>> getAllNodesInfo() const;

        std::tuple<int, std::string, std::string, nuraft::ulong, nuraft::ulong> getCurrentNodesInfo() const;

        std::string getNodeStatus(int node_id) const;

        ptr< cmd_result< ptr<buffer> > > appendEntries(const std::string& entry);

    private:
        int node_id;
        std::string endpoint;
        int port_;
        ptr<state_mgr> smgr_; // 转换管理器
        ptr<state_machine> sm_; // 状态机
        raft_launcher launcher_; // raft启动器
        ptr<raft_server> raft_instance_; // raft服务器
        VectorDatabase* vector_database_;
};