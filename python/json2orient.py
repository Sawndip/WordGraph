import pyorient
import json


f = open("../graph.json")
graph = json.loads(f.read())

db = pyorient.OrientDB("localhost", 2424)
session_id = db.connect("root", "0F0739BB9B03876FD86D33F55AE9AA0BE4991E1255F6244A9CF6F17CB95B0E9A")

db.db_open("WordGraph", "admin", "admin")

nodes = graph["nodes"]

wordIds = {}

typeMapping = {}
typeMapping["prev"] = "previous"
typeMapping["next"] = "next"

for node in nodes:
    word = node["word"]
    rec = db.command("create vertex Node set name='{0}', count=0".format(word))
    # store id of current word
    rid = rec[0].rid
    wordIds[word] = rid


for node in nodes:
    fromId = wordIds[node["word"]]
    for rel in node["outgoing"]:
        to = rel["word"]
        count = rel["count"]
        relType = typeMapping[rel["type"]]
        toId = wordIds[to]
        db.command("create edge {0} from {1} to {2} set count={3}".format(relType, fromId, toId, count))

# set up script to update counts of each node
"ALTER class previous set "