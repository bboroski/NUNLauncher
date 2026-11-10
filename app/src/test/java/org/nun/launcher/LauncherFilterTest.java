package org.nun.launcher;

import org.junit.Test;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public final class LauncherFilterTest {
    @Test
    public void emptyQueryMatchesEveryVisibleApp() {
        assertTrue(LauncherFilter.matches("Calculator", "com.example.calc", ""));
        assertTrue(LauncherFilter.matches("Calculator", "com.example.calc", "   "));
    }

    @Test
    public void matchingIsCaseInsensitive() {
        assertTrue(LauncherFilter.matches("Settings", "android.settings", "SETT"));
        assertTrue(LauncherFilter.matches("NUN Tools", "org.nun.tools", "nun"));
    }

    @Test
    public void packageNameCanBeSearched() {
        assertTrue(LauncherFilter.matches("Photos", "com.vendor.gallery", "vendor"));
    }

    @Test
    public void nonMatchingQueryIsRejected() {
        assertFalse(LauncherFilter.matches("Calculator", "com.example.calc", "weather"));
    }

    @Test
    public void nullInputsAreHandled() {
        assertTrue(LauncherFilter.matches(null, null, null));
        assertFalse(LauncherFilter.matches(null, null, "missing"));
    }
}
