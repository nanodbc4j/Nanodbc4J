package io.github.michael1297.jdbc.metadata;

public record OdbcDatasource(String name, String driver) {
    @Override
    public String toString() {
        return String.format("ODBC Data Source: %s (Driver: %s)", name, driver);
    }
}
