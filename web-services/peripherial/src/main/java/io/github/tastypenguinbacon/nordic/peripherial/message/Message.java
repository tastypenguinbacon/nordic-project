package io.github.tastypenguinbacon.nordic.peripherial.message;

import com.fasterxml.jackson.databind.JsonNode;

/**
 * Created by pingwin on 28.05.17.
 */
@ValidMessage
public class Message {

    private String type;

    private JsonNode message;

    public Message() {
    }

    public Message(String type, JsonNode message) {
        setMessage(message);
        setType(type);
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type.toLowerCase();
    }

    public JsonNode getMessage() {
        return message;
    }

    public void setMessage(JsonNode message) {
        this.message = message;
    }

    @Override
    public String toString() {
        return "Message{" +
                "type=" + type +
                ", message='" + message + '\'' +
                '}';
    }
}
