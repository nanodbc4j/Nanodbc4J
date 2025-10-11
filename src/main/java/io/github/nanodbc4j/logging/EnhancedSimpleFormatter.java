package io.github.nanodbc4j.logging;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.util.Objects;
import java.util.logging.Formatter;
import java.util.logging.Level;
import java.util.logging.LogRecord;

/**
 * Enhanced version of {@code SimpleFormatter} that optionally appends
 * source file name and line number for fine-grained log levels (FINE/FINER/FINEST).
 * <p>
 * Supports custom format via system property:
 * {@code -Djava.util.logging.LineNumberFormatter.format=...}
 *
 * Default format includes a placeholder for source location: [%7$s]
 */
@SuppressWarnings("unused")
public class EnhancedSimpleFormatter extends Formatter {

    private final String format;

    public EnhancedSimpleFormatter() {
        // Try to load custom format from system property
        String customFormat = System.getProperty("java.util.logging.EnhancedSimpleFormatter.format");
        // Default format: similar to SimpleFormatter but with [%7$s] for source location
        this.format = Objects.requireNonNullElse(customFormat, "%1$tY-%1$tm-%1$td %1$tH:%1$tM:%1$tS %4$s %2$s [%7$s]%n%5$s%6$s%n");
    }

    @Override
    public String format(LogRecord record) {
        ZonedDateTime zdt = ZonedDateTime.ofInstant(
                record.getInstant(), ZoneId.systemDefault());

        String source;
        if (record.getSourceClassName() != null) {
            source = record.getSourceClassName();
            if (record.getSourceMethodName() != null) {
                source += " " + record.getSourceMethodName();
            }
        } else {
            source = record.getLoggerName();
        }

        String message = formatMessage(record);

        String throwable = "";
        if (record.getThrown() != null) {
            StringWriter sw = new StringWriter();
            PrintWriter pw = new PrintWriter(sw);
            pw.println();
            record.getThrown().printStackTrace(pw);
            pw.close();
            throwable = sw.toString();
        }

        String sourceLocation = "";
        // Show file:line only for FINE and lower (i.e., more detailed than INFO)
        if (record.getLevel().intValue() <= Level.FINE.intValue()) {
            sourceLocation = getCallerLocation();
        }

        return String.format(format,
                zdt,
                source,
                record.getLoggerName(),
                record.getLevel().getName(),
                message,
                throwable,
                sourceLocation);
    }

    private String getCallerLocation() {
        StackTraceElement[] stack = new Throwable().getStackTrace();
        for (StackTraceElement frame : stack) {
            String className = frame.getClassName();
            // Skip JUL internal classes and this formatter
            if (className.startsWith("java.util.logging.")
                    || className.equals(this.getClass().getName())
                    || className.startsWith("java.lang.Thread")) {
                continue;
            }
            // First non-JUL frame is the caller
            return frame.getFileName() + ":" + frame.getLineNumber();
        }
        return "";
    }
}