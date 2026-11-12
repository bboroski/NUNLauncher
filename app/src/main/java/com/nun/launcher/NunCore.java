package com.nun.launcher;

/** JNI boundary to the native NUN runtime compiled from core/NUN. */
public final class NunCore {
    private static final boolean AVAILABLE;
    static {
        boolean loaded;
        try {
            System.loadLibrary("nun_android_bridge");
            loaded = true;
        } catch (UnsatisfiedLinkError error) {
            loaded = false;
        }
        AVAILABLE = loaded;
    }
    private NunCore() {}
    public static boolean isAvailable() { return AVAILABLE; }
    public static int start() { return AVAILABLE ? nativeStart() : -1000; }
    public static int poll() { return AVAILABLE ? nativePoll() : -1000; }
    public static int stop() { return AVAILABLE ? nativeStop() : -1000; }
    public static int state() { return AVAILABLE ? nativeState() : -1; }
    private static native int nativeStart();
    private static native int nativePoll();
    private static native int nativeStop();
    private static native int nativeState();
}
