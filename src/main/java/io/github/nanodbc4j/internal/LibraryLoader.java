package io.github.nanodbc4j.internal;

import com.sun.jna.Platform;
import lombok.experimental.UtilityClass;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.StandardCopyOption;
import java.util.logging.Logger;

@UtilityClass
public class LibraryLoader {
    private static final Logger logger = Logger.getLogger(LibraryLoader.class.getName());
    private static final String LIBRARY_NAME = "odbc";
    private static final String TEMP_LIB_PREFIX = "nanodbc4j_";
    private static volatile boolean isLoaded = false;

    public static synchronized void load() throws IOException {
        if (isLoaded) {
            return;
        }

        // Преобразуем имя библиотеки в системное (libodbc.so, odbc.dll, libodbc.dylib)
        String libName = System.mapLibraryName(LibraryLoader.LIBRARY_NAME);

        // Определяем путь внутри JAR: например, /natives/odbc.dll
        String resourcePath = getNativeResourcePath(libName);

        loadLibraryFromJar(resourcePath);
        isLoaded = true;
    }

    private static String getNativeResourcePath(String libName) {
        return "natives/" + libName;
    }

    private static void loadLibraryFromJar(String resourcePath) throws IOException {
        try (InputStream in = LibraryLoader.class.getClassLoader().getResourceAsStream(resourcePath)) {
            if (in == null) {
                throw new RuntimeException("Cannot find native library in JAR: " + resourcePath);
            }

            // Создаём временный файл с правильным расширением
            File temp = File.createTempFile(TEMP_LIB_PREFIX, "." + getExtension(resourcePath));
            temp.deleteOnExit();

            Files.copy(in, temp.toPath(), StandardCopyOption.REPLACE_EXISTING);

            // Устанавливаем права на выполнение (не нужно на Windows)
            if (!Platform.isWindows()) {
                boolean isSetExecutable = temp.setExecutable(true);
                if (!isSetExecutable) {
                    logger.warning("Could not set executable permission: " + temp.getAbsolutePath());
                    // Не фатально, но предупреждаем
                }
            }

            System.load(temp.getAbsolutePath());
            logger.info("Successfully loaded native library from JAR: " + temp.getAbsolutePath());
        }
    }

    private static String getExtension(String path) {
        int lastDot = path.lastIndexOf('.');
        return lastDot != -1 ? path.substring(lastDot + 1) : "";
    }
}
