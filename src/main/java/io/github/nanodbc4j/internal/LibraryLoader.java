package io.github.nanodbc4j.internal;

import com.sun.jna.Platform;
import lombok.experimental.UtilityClass;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.FileVisitOption;
import java.nio.file.FileVisitResult;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.SimpleFileVisitor;
import java.nio.file.StandardCopyOption;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.EnumSet;
import java.util.UUID;
import java.util.logging.Logger;

@UtilityClass
public class LibraryLoader {
    private static final Logger logger = Logger.getLogger(LibraryLoader.class.getName());
    private static final String LIBRARY_NAME = "nanodbc4j";
    private static final String TEMP_LIB_PREFIX = "nanodbc4j_";
    private static final String LOCK_EXT = ".lck";
    private static volatile boolean isLoaded = false;

    public static synchronized void load() throws IOException {
        if (isLoaded) {
            return;
        }

        // Только при первом запуске — чистим старые файлы
        cleanup();

        // Преобразуем имя библиотеки в системное (libodbc.so, odbc.dll, libodbc.dylib)
        String libName = System.mapLibraryName(LIBRARY_NAME);
        // Определяем путь внутри JAR: например, /natives/odbc.dll
        String resourcePath = "natives" + '/' + libName;

        loadLibraryFromJar(resourcePath);
        isLoaded = true;
    }

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
                throw new RuntimeException("Cannot find native library in JAR: " + resourcePath);
            }

            Files.copy(in, libFile, StandardCopyOption.REPLACE_EXISTING);

            // Создаём .lck файл — маркер "библиотека используется"
            Files.createFile(lckFile);

            // Гарантируем удаление при выходе (на Linux/macOS может сработать)
            libFile.toFile().deleteOnExit();
            lckFile.toFile().deleteOnExit();

            // Устанавливаем права на выполнение (не нужно на Windows)
            if (!Platform.isWindows()) {
                boolean isSetExecutable = libFile.toFile().setExecutable(true);
                if (!isSetExecutable) {
                    logger.warning("Could not set executable permission: " + libFile);
                    // Не фатально, но предупреждаем
                }
            }

            System.load(libFile.toString());
            logger.info("Successfully loaded native library from JAR: " + libFile);
        }
    }

    // === Аналог cleanup() из sqlite-jdbc ===
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
                                    logger.fine("Deleted stale temp lib: " + file);
                                } catch (Exception e) {
                                    // ignore
                                }
                            }
                        }
                    }
                    return FileVisitResult.CONTINUE;
                }
            });
        } catch (Exception e) {
            logger.warning("Failed to cleanup old native libs: " + e.getMessage());
        }
    }
}
