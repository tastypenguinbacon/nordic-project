package io.github.tastypenguinbacon.peripherial.message;

import javax.validation.Constraint;
import javax.validation.Payload;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Created by pingwin on 28.05.17.
 */
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Constraint(validatedBy = MessageValidator.class)
public @interface ValidMessage {
    String message() default "Invalid message";
    Class<?>[] groups() default {};
    Class<? extends Payload>[] payload() default {};
}
