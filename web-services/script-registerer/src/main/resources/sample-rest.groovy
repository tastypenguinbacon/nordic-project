import groovy.json.*

log.info("Received {}", message)
id = message.get("id")
if (id == null)
    jsonOut = db.getMessage("")
else {
    String text = id.asText()
    if (text.endsWith("-info") && message.get("code") != null)
        jsonOut = db.postMessage(text, '{"code":' + message.get("code").asText() + "}")
    else
        jsonOut = db.getMessage(id.asText())
}
return jsonOut