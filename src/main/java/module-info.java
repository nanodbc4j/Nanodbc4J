module io.github.nanodbc4j {
    requires com.sun.jna;
    requires java.logging;
    requires java.sql;
    requires static lombok;

    exports io.github.nanodbc4j.jdbc;
    exports io.github.nanodbc4j.dto;
    exports io.github.nanodbc4j.exceptions;

    provides java.sql.Driver with io.github.nanodbc4j.jdbc.NanodbcDriver;
}