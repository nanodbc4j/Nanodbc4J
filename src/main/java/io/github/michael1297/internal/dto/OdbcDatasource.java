package io.github.michael1297.internal.dto;

public record OdbcDatasource(String name, String driver) {
    @Override
    public String toString() {
        return String.format("ODBC Data Source: %s (Driver: %s)", name, driver);
    }
}
