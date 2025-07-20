# Nanodbc4J

**Experimental JDBC-ODBC Bridge using nanodbc and JNA**

> **Note**: This project is **experimental** and under active development. It is not suitable for production use.

Nanodbc4J is a **JDBC driver** that enables Java applications to access ODBC data sources directly, using the modern [nanodbc](https://github.com/nanodbc/nanodbc ) C++ library via **Java Native Access (JNA)**.

It is designed as a replacement for the legacy Oracle JDBC-ODBC Bridge, which was removed in Java 8. Nanodbc4J aims to provide a **lightweight, cross-platform** alternative that supports modern ODBC features and integrates with standard JDBC-based applications.

##  Why this Project?

Since Oracle removed the built-in **JDBC-ODBC Bridge** in Java 8, Java developers have been left without a standard way to connect to ODBC data sources.

Nanodbc4J fills this gap by:

- Using modern C++ (via `nanodbc`) for safe and efficient ODBC access.
- Providing a **JDBC-compliant interface** for use in Java applications.
- Supporting cross-platform builds for **Windows (ODBC)** and **Linux (unixODBC)**.

## Features (planned)

- [ ] Connection management via ODBC
- [ ] Support for setting connection and statement attributes
- [ ] Query execution and result set handling
- [ ] Prepared statements
- [ ] Transaction support
- [ ] Unicode support
- [ ] Error handling and diagnostics

## ⚠️ Limitations

- **Experimental**: APIs may change frequently.
- **Performance**: JNA is slower than JNI or native code.
- **Platform dependent**: Requires ODBC driver installed and configured.
- **Incomplete JDBC coverage**: Not all JDBC features are implemented yet.

---

## Requirements

- Java 17+
- JNA (via Maven or Gradle)
- C++ compiler for building native nanodbc wrapper
- ODBC driver installed and configured

## Usage (Example)

```java
// Coming soon!
```