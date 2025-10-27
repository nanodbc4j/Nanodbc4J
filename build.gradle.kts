import java.util.*

plugins {
    // Java plugin for building jars
    java
    // Publishing to Maven repositories
    `maven-publish`
    // Signing artifacts (GPG)
    signing
}

group = "io.github.nanodbc4j"
version = "1.0-SNAPSHOT"
description = "JDBC Type 1 driver implementation using nanodbc C++ library for ODBC connectivity. Open-source replacement for the removed JDBC-ODBC Bridge."

val jnaVersion = "5.17.0"
val lombokVersion = "1.18.38"

java {
    // Use Java toolchain to target Java 17 (source/target)
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
    // Attach sources and javadoc jars (required for Maven Central)
    withSourcesJar()
    withJavadocJar()
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("net.java.dev.jna:jna:$jnaVersion")

    // Lombok in Maven was 'provided' -> use compileOnly + annotationProcessor
    compileOnly("org.projectlombok:lombok:$lombokVersion")
    annotationProcessor("org.projectlombok:lombok:$lombokVersion")
}

val nativeProfile = providers.gradleProperty("nativeProfile").getOrElse("release").lowercase(Locale.getDefault())

val nativeWindowsDir = when (nativeProfile) {
    "debug" -> "out/build/x64-Debug"
    "release" -> "out/build/x64-Release"
    else -> error("Unsupported nativeProfile: $nativeProfile. Use 'debug' or 'release'.")
}

val nativeLinuxDir = when (nativeProfile) {
    "debug" -> "out/build/WSL-GCC-Debug"
    "release" -> "out/build/WSL-GCC-Release"
    else -> error("Unsupported nativeProfile: $nativeProfile. Use 'debug' or 'release'.")
}

// Copy native libraries into resources/natives similarly to your pom's resources configuration.
// This places *.dll and *.so under resources/natives in the produced JAR/resources.
tasks.processResources {
    duplicatesStrategy = DuplicatesStrategy.INCLUDE

    listOf(nativeWindowsDir, nativeLinuxDir).forEach { dir ->
        if (!file(dir).exists()) {
            throw GradleException("Native directory does not exist: $dir. Build native libraries first.")
        }
    }

    // Copy Windows native DLLs
    from(nativeWindowsDir) {
        include("*.dll")
        into("natives")
    }
    // Copy Linux (WSL) shared objects
    from(nativeLinuxDir) {
        include("*.so")
        into("natives")
    }
}

// Add Automatic-Module-Name to the JAR manifest for JPMS compatibility
tasks.jar {
    manifest {
        attributes["Automatic-Module-Name"] = "io.github.nanodbc4j"
    }
}

// Configure Javadoc options: use Java 17 source and disable strict doclint (doclint=none)
tasks.withType<Javadoc> {
    (options as StandardJavadocDocletOptions).addStringOption("Xdoclint:none", "-quiet")
    options.encoding = "UTF-8"
}

// Publishing configuration: produces POM metadata
publishing {
    publications {
        create<MavenPublication>("mavenJava") {
            from(components["java"])

            pom {
                name.set("Nanodbc4J")
                description.set(project.description)
                url.set("https://github.com/nanodbc4j/Nanodbc4J")

                licenses {
                    license {
                        name.set("Apache License 2.0")
                        url.set("https://www.apache.org/licenses/LICENSE-2.0.txt")
                        distribution.set("repo")
                    }
                }

                scm {
                    connection.set("scm:git:git://github.com/nanodbc4j/Nanodbc4J.git")
                    developerConnection.set("scm:git:ssh://github.com/nanodbc4j/Nanodbc4J.git")
                    url.set("https://github.com/nanodbc4j/Nanodbc4J")
                }

                developers {
                    developer {
                        id.set("nanodbc4j")
                        name.set("nanodbc4j")
                    }
                }
            }
        }
    }
}

// Signing configuration: use in-memory PGP key if provided via properties or environment variables.
signing {
    val signingKey: String? = findProperty("signing.key") as String? ?: System.getenv("SIGNING_KEY")
    val signingPassword: String? = findProperty("signing.password") as String? ?: System.getenv("SIGNING_PASSWORD")

    if (!signingKey.isNullOrBlank()) {
        useInMemoryPgpKeys(signingKey, signingPassword)
        sign(publishing.publications["mavenJava"])
    } else {
        // No key provided: no-op. CI should provide keys for signing during release.
    }
}