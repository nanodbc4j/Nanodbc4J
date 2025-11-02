package io.github.nanodbc4j.internal.binding;

import com.sun.jna.Function;
import com.sun.jna.Library;
import com.sun.jna.Platform;
import lombok.experimental.UtilityClass;
import lombok.extern.java.Log;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteOrder;
import java.nio.file.FileVisitOption;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.EnumSet;
import java.util.HashMap;
import java.util.Map;
import java.util.UUID;

@Log
@UtilityClass
class LibraryLoader {
    private static final String LIBRARY_NAME = "nanodbc4j";
    private static final String TEMP_LIB_PREFIX = "nanodbc4j_";
    private static final String LOCK_EXT = ".lck";
    private static volatile boolean isLoaded = false;

    /**
     * Loads native library from JAR resources.
     * Performs cleanup, extracts library to temp directory and loads it.
     * Thread-safe and idempotent.
     *
     * @throws IOException if library extraction fails or library not found in JAR
     */
    static synchronized void load() throws IOException {
        if (isLoaded) {
            return;
        }

        // Only on first run - clean up old files
        cleanup();

        // Convert library name to system-specific (libodbc.so, odbc.dll, libodbc.dylib)
        String libName = System.mapLibraryName(LIBRARY_NAME);
        // Define path inside JAR: e.g., /natives/odbc.dll
        String resourcePath = "natives" + '/' + libName;

        loadLibraryFromJar(resourcePath);
        isLoaded = true;
    }

    /**
     * Extracts and loads native library from JAR resource.
     * Creates temp files with UUID and lock file for cleanup tracking.
     *
     * @param resourcePath path to native library in JAR
     * @throws IOException if file operations fail or library not found in JAR
     */
    private static void loadLibraryFromJar(String resourcePath) throws IOException {
        String libFileName = new File(resourcePath).getName();
        String uuid = UUID.randomUUID().toString();
        String extractedLibName = TEMP_LIB_PREFIX + uuid + '_' + libFileName;
        String extractedLckName = extractedLibName + LOCK_EXT;

        Path tempDir = Paths.get(System.getProperty("java.io.tmpdir"));
        Path libFile = tempDir.resolve(extractedLibName);
        Path lckFile = tempDir.resolve(extractedLckName);

        try (InputStream in = LibraryLoader.class.getClassLoader().getResourceAsStream(resourcePath)) {
            if (in == null) {
                throw new IOException("Cannot find native library in JAR: " + resourcePath);
            }

            Files.copy(in, libFile, StandardCopyOption.REPLACE_EXISTING);

            // Create .lck file - marker "library in use"
            Files.createFile(lckFile);

            // Guarantee deletion on exit (may work on Linux/macOS)
            libFile.toFile().deleteOnExit();
            lckFile.toFile().deleteOnExit();

            // Set execute permissions (not needed on Windows)
            if (!Platform.isWindows()) {
                boolean isSetExecutable = libFile.toFile().setExecutable(true);
                if (!isSetExecutable) {
                    log.warning("Could not set executable permission: " + libFile);
                    // Not fatal, but warn
                }
            }

            System.load(libFile.toString());
            log.info("Successfully loaded native library from JAR: " + libFile);
        }
    }

    /**
     * Cleans up stale temporary native libraries.
     * Deletes library files that don't have corresponding lock files.
     * Suppresses all exceptions during cleanup.
     */
    private static void cleanup() {
        try {
            Path tempDir = Paths.get(System.getProperty("java.io.tmpdir"));
            Files.walkFileTree(tempDir, EnumSet.of(FileVisitOption.FOLLOW_LINKS), 1, new SimpleFileVisitor<>() {
                @Override
                public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) {
                    if (file != null && !Files.isDirectory(file)) {
                        String name = file.getFileName().toString();
                        if (name.startsWith(TEMP_LIB_PREFIX) && !name.endsWith(LOCK_EXT)) {
                            Path lckFile = file.resolveSibling(name + LOCK_EXT);
                            if (!Files.exists(lckFile)) {
                                try {
                                    Files.delete(file);
                                    log.fine("Deleted stale temp lib: " + file);
                                } catch (Exception ignore) {
                                }
                            }
                        }
                    }
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch (Exception e) {
            log.warning("Failed to cleanup old native libs: " + e.getMessage());
        }
    }

    /**
     * Returns native library configuration options.
     * Configures string encoding based on platform and calling convention.
     *
     * @return map with library options (string encoding and calling convention)
     */
    static Map<String, Object> getNativeLibraryOptions() {
        Map<String, Object> options = new HashMap<>();
        if (Platform.isWindows()) {
            options.put(Library.OPTION_STRING_ENCODING, "UTF-16LE");
        } else {
            if (ByteOrder.nativeOrder() == ByteOrder.LITTLE_ENDIAN) {
                options.put(Library.OPTION_STRING_ENCODING, "UTF-16LE");
            } else {
                options.put(Library.OPTION_STRING_ENCODING, "UTF-16BE");
            }
        }
        options.put(Library.OPTION_STRING_ENCODING, "UTF-16LE");
        options.put(Library.OPTION_CALLING_CONVENTION, Function.C_CONVENTION);
        return options;
    }
}
