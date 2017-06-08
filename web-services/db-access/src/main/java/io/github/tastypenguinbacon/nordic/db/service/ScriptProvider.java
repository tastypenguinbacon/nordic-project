package io.github.tastypenguinbacon.nordic.db.service;

import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;
import javaslang.control.Option;
import org.bson.Document;

/**
 * Created by pingwin on 04.06.17.
 */
public class ScriptProvider extends MongoCommunication {
    public Option<String> query(String scriptName) {
        try (MongoClient mongo = obtainMongoClient()) {
            MongoDatabase scripts = mongo.getDatabase("scripts");

            MongoCollection<Document> groovyScripts = scripts.getCollection("groovy");
            Document query = new Document();
            query.put("name", scriptName);

            for (Document script : groovyScripts.find(query)) {
                if (script.containsKey("script")) {
                    return Option.of(script.get("script").toString());
                }
            }
        }
        return Option.none();
    }

    public void addScript(String scriptName, String script) {
        try (MongoClient mongo = obtainMongoClient()) {
            MongoDatabase scripts = mongo.getDatabase("scripts");
            MongoCollection<Document> groovyScripts = scripts.getCollection("groovy");

            Document toRegister = new Document();
            toRegister.put("name", scriptName);
            toRegister.put("script", script);

            Document query = new Document();
            query.put("name", scriptName);

            groovyScripts.deleteMany(query);
            groovyScripts.insertOne(toRegister);
        }
    }
}
