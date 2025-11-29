# Nanodbc4J
![build status](https://github.com/nanodbc4j/Nanodbc4J/actions/workflows/build.yml/badge.svg)
![lines count](https://img.shields.io/endpoint?url=https%3A%2F%2Fghloc.vercel.app%2Fapi%2Fnanodbc4j%2FNanodbc4J%2Fbadge%3Ffilter%3D%255Ecpp%252F%252C%255Esrc%252F)

**A modern JDBC-ODBC bridge using nanodbc and JNA**

>  Still experimental — use with caution in non-production environments.

Nanodbc4J is a **JDBC driver** that enables Java applications to access ODBC data sources directly, using the modern [nanodbc](https://github.com/nanodbc/nanodbc ) C++ library via **Java Native Access (JNA)**.

It is designed as a replacement for the legacy Oracle JDBC-ODBC Bridge, which was removed in Java 8. Nanodbc4J aims to provide a **lightweight, cross-platform** alternative that supports modern ODBC features and integrates with standard JDBC-based applications.

##  Why this Project?

Since Oracle removed the built-in **JDBC-ODBC Bridge** in Java 8, Java developers have been left without a standard way to connect to ODBC data sources.

Nanodbc4J fills this gap by:

- Using modern C++ (via `nanodbc`) for safe and efficient ODBC access.
- Providing a **JDBC-compliant interface** for use in Java applications.
- Supporting cross-platform builds for **Windows (ODBC)** and **Linux (unixODBC)**.

##  Implemented Features

- [x] JDBC `Driver` registration and URL parsing (`jdbc:nanodbc4j:...`)
- [x] Connection via **DSN** or full **ODBC connection string**
- [x] Basic `Statement` and `PreparedStatement` support
- [x] `ResultSet` navigation and data retrieval (strings, integers, decimals, dates, booleans)
- [x] Full **metadata support** (`DatabaseMetaData`, `ResultSetMetaData`) — enables integration with DBeaver
- [x] Automatic loading of native libraries via **JNA** (no manual setup required)
- [x] Cross-platform native builds (Windows, Linux)
- [x] Basic ODBC error diagnostics and mapping to `SQLException`


##  Limitations & Notes

- **Not 100% JDBC-compliant** — some methods throw `SQLFeatureNotSupportedException`.
- **Requires JNA** — users must include `jna.jar`.
- **ODBC driver must be installed and configured** on the system.
- **Performance**: JNA adds overhead vs. pure JNI — acceptable for most integration scenarios.
- **Unicode support**: basic; full UTF-16/UTF-8 handling depends on underlying ODBC driver.

---

## Requirements

- **Java 17+**
- **ODBC driver** installed (e.g., PostgreSQL ODBC, SQL Server ODBC, etc.)
- **(Optional)** For development: C++ compiler to rebuild native layer

##  Usage Example (Java)

```java
import java.sql.*;

public class Nanodbc4JExample {
    public static void main(String[] args) throws SQLException {
        // Option 1: Connect via DSN
        String url = "jdbc:nanodbc4j:DSN=MyDatabase;UID=user;PWD=secret";

        // Option 2: Connect via full ODBC connection string
        // String url = "jdbc:nanodbc4j:DRIVER={PostgreSQL ODBC Driver};SERVER=localhost;PORT=5432;DATABASE=test;UID=postgres;PWD=pass";

        try (Connection conn = DriverManager.getConnection(url);
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT id, name FROM users")) {

            while (rs.next()) {
                System.out.println(rs.getInt("id") + ": " + rs.getString("name"));
            }
        }
    }
}
