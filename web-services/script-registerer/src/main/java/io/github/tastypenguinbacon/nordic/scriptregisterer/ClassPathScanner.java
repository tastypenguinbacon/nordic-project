package io.github.tastypenguinbacon.nordic.scriptregisterer;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Created by pingwin on 08.06.17.
 */
public class ClassPathScanner {
    private final Logger logger = LoggerFactory.getLogger(ClassPathScanner.class);

    public List<String> getResourceFiles(String path) throws IOException {
        List<String> filenames = new ArrayList<>();
        try (InputStream in = getResourceAsStream(path);
             BufferedReader br = new BufferedReader(new InputStreamReader(in))) {
            String resource;
            while ((resource = br.readLine()) != null) {
                filenames.add(resource);
            }
        }
        return filenames;
    }

    private InputStream getResourceAsStream(String resource) {
        InputStream in = getContextClassLoader().getResourceAsStream(resource);
        return in == null ? getClass().getResourceAsStream(resource) : in;
    }

    private ClassLoader getContextClassLoader() {
        return Thread.currentThread().getContextClassLoader();
    }

    public String getResourceAsString(String resource) {
        logger.info("Getting resource for {}", resource);
        StringBuilder script = new StringBuilder();
        try (InputStream in = getClass().getResourceAsStream("/" + resource)) {
            Scanner scanner = new Scanner(in);
            String line;
            while (scanner.hasNext()) {
                line = scanner.nextLine();
                script.append(line).append('\n');
            }
            return script.toString();
        } catch (IOException e) {
            throw new UncheckedIOException(e);
        }
    }
}
