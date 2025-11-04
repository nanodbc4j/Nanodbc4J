import java.util.*

plugins {
    // Java plugin for building jars
    java
    // Publishing to Maven repositories
    id("com.vanniktech.maven.publish") version "0.34.0"
}

group = "io.github.nanodbc4j"
version = "1.0-beta-1"
description = "JDBC Type 1 driver implementation using nanodbc C++ library for ODBC connectivity. Open-source replacement for the removed JDBC-ODBC Bridge."

java {
    // Use Java toolchain to target Java 17 (source/target)
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

repositories {
    mavenCentral()
}

val props = Properties().apply {
    val propertiesFile = File(File("$projectDir"), "gradle.properties")
    propertiesFile.inputStream().use { load(it) }
}

val jnaVersion: String = props.getProperty("jnaVersion")
val lombokVersion: String= props.getProperty("lombokVersion")
val assertjVersion: String = props.getProperty("assertjVersion")
val junitVersion: String = props.getProperty("junitVersion")

dependencies {
    implementation("net.java.dev.jna:jna:$jnaVersion")

    compileOnly("org.projectlombok:lombok:$lombokVersion")
    annotationProcessor("org.projectlombok:lombok:$lombokVersion")

    testImplementation("org.assertj:assertj-core:${assertjVersion}")
    testImplementation(platform("org.junit:junit-bom:${junitVersion}"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    testRuntimeOnly("org.junit.platform:junit-platform-launcher")
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
        if (nativeProfile != "debug" && !file(dir).exists()) {
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

// JUnit tests
tasks.test {
    useJUnitPlatform()
    testLogging {
        events("passed", "skipped", "failed")
    }
}

// Publishing configuration: produces POM metadata
mavenPublishing {
    coordinates(group.toString(), name.toString(), version.toString())

    pom {
        name.set("Nanodbc4J")
        description.set(project.description)
        inceptionYear.set("2025")
        url.set("https://github.com/nanodbc4j/Nanodbc4J")
        licenses {
            license {
                name.set("The Apache License, Version 2.0")
                url.set("https://www.apache.org/licenses/LICENSE-2.0.txt")
                distribution.set("https://www.apache.org/licenses/LICENSE-2.0.txt")
            }
        }
        developers {
            developer {
                id.set("Michael1297")
                name.set("Michael1297")
                url.set("https://github.com/Michael1297")
            }
        }
        scm {
            url.set("https://github.com/nanodbc4j/Nanodbc4J")
            connection.set("scm:git:git://github.com/nanodbc4j/Nanodbc4J.git")
            developerConnection.set("scm:git:ssh://github.com/nanodbc4j/Nanodbc4J.git")
        }
    }
}