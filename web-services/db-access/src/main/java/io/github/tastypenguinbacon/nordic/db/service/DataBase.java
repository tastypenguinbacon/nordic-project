package io.github.tastypenguinbacon.nordic.db.service;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.mongodb.MongoClient;
import com.mongodb.client.MongoDatabase;
import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import javaslang.collection.List;
import javaslang.control.Option;
import javaslang.control.Try;
import org.bson.Document;
import org.slf4j.Logger;

import javax.inject.Inject;

/**
 * Created by pingwin on 08.06.17.
 */
public class DataBase extends MongoCommunication {
    @Inject
    @SLF4JLogger
    private Logger logger;

    public void put(String station, JsonNode payload) {
        logger.info("Database received from {} the following {}", station, payload);
        try (MongoClient mongo = obtainMongoClient()) {
            MongoDatabase db = mongo.getDatabase("data");

            long timestamp = Option.of(payload.get("timestamp"))
                    .map(JsonNode::asLong)
                    .getOrElse(System.currentTimeMillis() / 1000);

            Document toRegister = new Document();
            toRegister.put("timestamp", timestamp);
            toRegister.put("body", payload.toString());
            db.getCollection(station).insertOne(toRegister);
        }
    }

    public List<String> availableIdentifiers() {
        try (MongoClient mongo = obtainMongoClient()) {
            MongoDatabase db = mongo.getDatabase("data");
            return List.ofAll(db.listCollectionNames());
        }
    }

    public List<JsonNode> getAfter(String station, long dataAfter) {
        try (MongoClient mongo = obtainMongoClient()) {
            MongoDatabase db = mongo.getDatabase("data");
            ObjectMapper mapper = new ObjectMapper();
            Document query = new Document();
            logger.info("Querying for {} after {}", station, dataAfter);
            query.put("timestamp", new Document("$gt", dataAfter));
            List<Document> documents = List.ofAll(db.getCollection(station).find(query));
            logger.info("Documents queried: {}", documents);
            return documents
                    .filter(document -> document.getLong("timestamp") > dataAfter)
                    .map(document -> document.get("body"))
                    .map(body -> Try.of(() -> (String) body))
                    .map(b -> b.flatMap(body -> Try.of(
                            () -> mapper.readValue(body, JsonNode.class))))
                    .filter(Try::isSuccess)
                    .map(Try::get);
        }
    }
}
