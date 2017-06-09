import groovy.json.*
log.info("Received {}", message)

id = message.get("stationId").asText()
jsonOut = db.postMessage(id, message.get("body").toString())

return jsonOut