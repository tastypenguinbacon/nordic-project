package io.github.tastypenguinbacon.nordic.db.service;

import com.mongodb.MongoClient;

/**
 * Created by pingwin on 08.06.17.
 */
public abstract class MongoCommunication {
    protected MongoClient obtainMongoClient() {
        String location = System.getProperty("mongo-db-location");
        String[] hostAndPort = location.split(":");
        return new MongoClient(hostAndPort[0], Integer.parseInt(hostAndPort[1]));
    }
}
