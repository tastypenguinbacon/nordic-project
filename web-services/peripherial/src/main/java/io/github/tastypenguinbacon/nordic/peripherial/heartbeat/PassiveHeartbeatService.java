package io.github.tastypenguinbacon.nordic.peripherial.heartbeat;

import io.github.tastypenguinbacon.nordic.common.logger.SLF4JLogger;
import io.github.tastypenguinbacon.nordic.common.rest.CommunicatorProvider;
import io.github.tastypenguinbacon.nordic.common.rest.TargetService;
import io.github.tastypenguinbacon.nordic.peripherial.cache.Cache;
import io.github.tastypenguinbacon.nordic.peripherial.cache.CacheTimeout;
import javaslang.collection.HashSet;
import javaslang.collection.Set;
import org.slf4j.Logger;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import javax.ejb.*;
import javax.inject.Inject;
import javax.ws.rs.client.Entity;
import java.util.List;

/**
 * Created by pingwin on 26.05.17.
 */
@Singleton
@Startup
@ConcurrencyManagement(ConcurrencyManagementType.CONTAINER)
public class PassiveHeartbeatService implements AvailabilityService {

    private static final int HEARTBEAT_TIMEOUT = 120000;

    private final Cache cache;
    private final Logger logger;

    private final CommunicatorProvider heartbeatLost;

    private TimerService timerService;
    private Set<String> melded = HashSet.empty();

    @Inject
    public PassiveHeartbeatService(@CacheTimeout(HEARTBEAT_TIMEOUT) Cache cache,
                                   @TargetService(name = "heartbeat-loss") CommunicatorProvider heartbeatLost,
                                   @SLF4JLogger Logger logger) {
        this.cache = cache;
        this.heartbeatLost = heartbeatLost;
        this.logger = logger;
    }

    @Resource
    public void setTimerService(TimerService timerService) {
        logger.info("Obtained TimerService for PassiveHeartbeatService");
        this.timerService = timerService;
    }

    @PostConstruct
    public void init() {
        TimerConfig timerConfig = new TimerConfig();
        timerConfig.setPersistent(false);
        timerService.createIntervalTimer(0, HEARTBEAT_TIMEOUT, timerConfig);
    }

    @Timeout
    public void heartbeatTimeout() {
        logger.info("HeartbeatResource timeout occurred");
        List<String> connectionLost = melded.filter(element -> !cache.contains(element))
                .peek(element -> logger.warn("Lost connectivity to {}", element))
                .toJavaList();
        notifyHeartBeatLost(connectionLost);
        melded = melded.filter(cache::contains)
                .peek(element -> logger.info("{} is available", element));
    }

    private void notifyHeartBeatLost(List<String> connectionLost) {
       if (!connectionLost.isEmpty())
           heartbeatLost.sendMessage(wt -> wt.request().post(Entity.json(connectionLost)));
    }

    @Override
    public void updateAvailability(String identifier) {
        cache.update(identifier);
        melded = melded.add(identifier);
    }
}
