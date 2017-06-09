log.info("Received {}", message)

body = message.get("body")
device = message.get("stationId").asText()
timestamp = body.get("timestamp").asLong()

result = db.getMessage(device + "-info", timestamp)

log.info("Result {}", result)

log.info(result.toString())


return result.toString()
