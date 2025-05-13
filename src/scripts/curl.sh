curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.8], "id":2, "indexType": "FLAT"}' http://localhost:8000/insert
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.5], "k":2, "indexType": "FLAT"}' http://localhost:8000/search
curl -X POST -H "Content-Type: application/json" -d '{"vectors":[0.5], "k":2, "indexType": "FLAT1"}' http://localhost:8000/search