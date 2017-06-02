package io.github.tastypenguinbacon.peripherial.message;

/**
 * Created by pingwin on 28.05.17.
 */
@ValidMessage
public class Message {
    public enum MessageType {
        ERROR, STATUS, REQUEST
    }

    private MessageType type;

    private String message;

    public Message() {
    }

    public Message(String type, String message) {
        setMessage(message);
        setType(type);
    }

    public MessageType getType() {
        return type;
    }

    public void setType(String type) {
        for (MessageType messageType : MessageType.values()) {
            if (messageType.toString().equalsIgnoreCase(type))
                this.type = messageType;
        }
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
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
