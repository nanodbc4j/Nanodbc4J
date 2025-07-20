# Nanodbc4J

**Experimental Java binding for [nanodbc](https://github.com/nanodbc/nanodbc) using JNA**

This project aims to provide a lightweight Java interface to the [nanodbc](https://github.com/nanodbc/nanodbc) C++ library via **Java Native Access (JNA)**. It allows Java applications to use ODBC directly through the modern C++ nanodbc library, without relying on JDBC drivers.

> ⚠️ **Warning**: This project is currently **experimental** and under active development. APIs may change frequently and it is not yet suitable for production use.

## Goals

- Provide a simple and efficient way to access ODBC from Java.
- Wrap nanodbc features such as:
    - Connection handling
    - Queries and prepared statements
    - Transactions
    - Attribute settings (connection & statement)
- Use JNA to avoid JNI boilerplate and simplify integration.

## Features (planned)

- [ ] Connection management via ODBC
- [ ] Support for setting connection and statement attributes
- [ ] Query execution and result set handling
- [ ] Prepared statements
- [ ] Transaction support
- [ ] Unicode support
- [ ] Error handling and diagnostics

## Requirements

- Java 17+
- JNA (via Maven or Gradle)
- C++ compiler for building native nanodbc wrapper
- ODBC driver installed and configured

## Usage (Example)

```java
// Coming soon!
```