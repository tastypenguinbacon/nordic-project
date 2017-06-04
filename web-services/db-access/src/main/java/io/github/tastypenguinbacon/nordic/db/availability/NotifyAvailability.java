package io.github.tastypenguinbacon.nordic.db.availability;

import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.common.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.nordic.common.rest.TargetService;
import javaslang.collection.HashMap;
import org.slf4j.Logger;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import javax.ejb.*;
import javax.inject.Inject;
import javax.ws.rs.client.Entity;
import java.util.Map;

/**
 * Created by pingwin on 02.06.17.
 */
@Singleton
@Startup
public class NotifyAvailability {
    private TimerService timerService;

    @Inject
    @SLF4JLogger
    private Logger logger;

    @Inject
    @TargetService(name = "resource-locator")
    private CommunicatorProvider resourceLocator;

    @Resource
    public void setTimerService(TimerService timerService) {
        this.timerService = timerService;
    }

    @PostConstruct
    public void init() {
        TimerConfig timerConfig = new TimerConfig();
        timerConfig.setPersistent(false);
        int timeout = Integer.parseInt(System.getProperty("resource-locator-timeout"));
        timerService.createIntervalTimer(0, timeout / 2, timerConfig);
    }

    @Timeout
    public void timeout() {
        logger.info("Message Processors - notified availability.");
        String location = System.getProperty("db-access");
        Map<String, String> locations = HashMap
                .of("scripts", location + "/groovy")
                .put("database", location).toJavaMap();
        resourceLocator.sendMessage(rl -> rl
                .request()
                .post(Entity.json(locations)));
    }
}
