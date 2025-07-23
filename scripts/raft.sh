# 集群数据管理----------------------------------------------------------------------------------------------------

# 使用配置文件启动 /home/cc/vectorDB/build/src/vdbserver
./vdb_server ../../../conf1.ini

# 提升为主节点
curl -X POST -H "Content-Type: application/json" -d '{}' http://localhost:8080/admin/setLeader

# 查看节点信息
curl -X GET http://localhost:8080/admin/listNode

# 写数据
curl -X POST -H "Content-Type: application/json" -d '{"id": 6, "vectors": [0.9], "int_field": 49, "indexType": "FLAT"}' http://localhost:8080/upsert

# 读数据
curl -X POST -H "Content-Type: application/json" -d '{"vectors": [0.9], "k": 5, "indexType": "FLAT", "filter":{"fieldName":"int_field","value":43, "op":"!="}}' http://localhost:8080/search

# 加从节点
curl -X POST -H "Content-Type: application/json" -d '{"nodeId": 2, "endpoint": "127.0.0.1:8082"}' http://localhost:8080/admin/addFollower
curl -X GET http://localhost:8080/admin/listNode
# ---------------------------------------------------------------------------------------------------------------


# 集群流量管理----------------------------------------------------------------------------------------------------

# 查看node信息
curl "http://localhost:6060/getNodeInfo?instanceId=instance1&nodeId=node123"

# 增加node1信息
curl -X POST "http://localhost:6060/addNode" -H "Content-Type: application/json" -d '{"instanceId": "instance1", "nodeId": "node123", "url": "http://127.0.0.1:8080", "role": 1, "status": 0}'

# 删除node信息
curl -X DELETE "http://localhost:6060/removeNode?instanceId=instance1&nodeId=node124"

# 查看instance下的所有node信息
curl "http://localhost:6060/getInstance?instanceId=instance1"

# 增加node2信息
curl -X POST "http://localhost:6060/addNode" -H "Content-Type: application/json" -d '{"instanceId": "instance1", "nodeId": "node124", "url": "http://127.0.0.1:9090", "role": 1, "status": 0}'
# ---------------------------------------------------------------------------------------------------------------