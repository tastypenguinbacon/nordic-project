package io.github.tastypenguinbacon.peripherial.heartbeat;

import io.github.tastypenguinbacon.peripherial.cache.Cache;
import io.github.tastypenguinbacon.peripherial.cache.CacheTimeout;
import io.github.tastypenguinbacon.peripherial.logger.SLF4JLogger;
import javaslang.collection.HashSet;
import javaslang.collection.Set;
import org.slf4j.Logger;

import javax.annotation.PostConstruct;
import javax.annotation.Resource;
import javax.ejb.*;
import javax.inject.Inject;

/**
 * Created by pingwin on 26.05.17.
 */
@Singleton
@Startup
@ConcurrencyManagement(ConcurrencyManagementType.CONTAINER)
public class PassiveHeartbeatService implements AvailabilityService {

    public static final int HEARTBEAT_TIMEOUT = 6000;

    private final Cache cache;
    private final Logger logger;
    private TimerService timerService;
    private Set<String> melded = HashSet.empty();

    @Inject
    public PassiveHeartbeatService(@CacheTimeout(HEARTBEAT_TIMEOUT) Cache cache, @SLF4JLogger Logger logger) {
        this.cache = cache;
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
        logger.info("Heartbeat timeout occurred");
        melded.filter(element -> !cache.contains(element))
                .forEach(element -> logger.warn("Lost connectivity to {}", element));
        melded = melded.filter(cache::contains)
                .peek(element -> logger.info("{} is available", element));
    }

    @Override
    public void updateAvailability(String identifier) {
        cache.update(identifier);
        melded = melded.add(identifier);
    }
}
