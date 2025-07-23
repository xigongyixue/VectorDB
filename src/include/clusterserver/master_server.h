#pragma once

#include "common/httplib.h"

#include <etcd/Client.hpp>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

enum class ServerRole { Master, Backup };

struct ServerInfo {
    std::string url;
    ServerRole role;
    rapidjson::Document toJson() const;
    static ServerInfo fromJson(const rapidjson::Document& value);
};

class MasterServer {
    public:
        explicit MasterServer(const std::string& etcdEndpoints, int httpPort);
        void run();

    private:
        etcd::Client etcdClient_; // 与etcd集群交互
        httplib::Server httpServer_;
        int httpPort_;

        void setResponse(httplib::Response& res, int retCode, const std::string& msg, const rapidjson::Document* data = nullptr);

        // etcd元数据
        // key: <instance ID,node ID>
        // value: <url,role,status>
        void getNodeInfo(const httplib::Request& req, httplib::Response& res);
        void addNode(const httplib::Request& req, httplib::Response& res);
        void removeNode(const httplib::Request& req, httplib::Response& res);

        // 获得该实例下所有节点（前缀匹配）
        void getInstance(const httplib::Request& req, httplib::Response& res);
};
