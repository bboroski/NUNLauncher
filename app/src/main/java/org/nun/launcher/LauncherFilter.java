package org.nun.launcher;

import java.util.Locale;

/** Pure, local-only matching for the visible launcher activity catalog. */
public final class LauncherFilter {
    private LauncherFilter() {
        throw new AssertionError("No instances");
    }

    public static boolean matches(String label, String packageName, String query) {
        if (query == null || query.trim().isEmpty()) {
            return true;
        }

        final String needle = query.trim().toLowerCase(Locale.ROOT);
        final String safeLabel = label == null ? "" : label.toLowerCase(Locale.ROOT);
        final String safePackage = packageName == null
                ? ""
                : packageName.toLowerCase(Locale.ROOT);

        return safeLabel.contains(needle) || safePackage.contains(needle);
    }
}
