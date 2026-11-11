package com.nun.launcher;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.Locale;

public final class MainActivity extends Activity {
    private static final int BG = Color.rgb(9, 11, 13);
    private static final int PANEL = Color.rgb(20, 23, 26);
    private static final int PANEL_ALT = Color.rgb(27, 31, 35);
    private static final int YELLOW = Color.rgb(244, 211, 63);
    private static final int WHITE = Color.rgb(245, 246, 241);
    private static final int MUTED = Color.rgb(153, 160, 164);

    private final Handler handler = new Handler(Looper.getMainLooper());
    private final List<AppEntry> apps = new ArrayList<>();
    private final List<ModuleEntry> modules = new ArrayList<>();
    private LinearLayout root;
    private LinearLayout body;
    private LinearLayout appGrid;
    private TextView coreStatus;
    private String coreStatusText = "STARTING NUN CORE…";
    private String selectedTab = "HOME";
    private String searchQuery = "";
    private boolean coreStarted;

    private final Runnable coreTicker = new Runnable() {
        @Override public void run() {
            if (!coreStarted) return;
            int result = NunCore.poll();
            if (result != 0) {
                coreStarted = false;
                updateCoreStatus("CORE DEGRADED · " + result);
                return;
            }
            handler.postDelayed(this, 5000L);
        }
    };

    @Override protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setStatusBarColor(BG);
        getWindow().setNavigationBarColor(BG);
        loadModules();
        refreshApps();
        startCore();
        buildShell();
        handler.postDelayed(coreTicker, 5000L);
    }

    @Override protected void onDestroy() {
        handler.removeCallbacks(coreTicker);
        if (coreStarted) {
            NunCore.stop();
            coreStarted = false;
        }
        super.onDestroy();
    }

    private void startCore() {
        if (!NunCore.isAvailable()) {
            updateCoreStatus("NUN SOURCE LINKED · NATIVE BRIDGE UNAVAILABLE");
            return;
        }
        int result = NunCore.start();
        coreStarted = result == 0;
        updateCoreStatus(coreStarted
                ? "NUN CORE ACTIVE · ANDROID HOST"
                : "NUN CORE START FAILED · " + result);
    }

    private void updateCoreStatus(String value) {
        coreStatusText = value;
        if (coreStatus != null) coreStatus.setText(value);
    }

    private void buildShell() {
        root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setBackgroundColor(BG);
        root.setPadding(dp(18), dp(16), dp(18), dp(10));

        LinearLayout brandRow = new LinearLayout(this);
        brandRow.setGravity(Gravity.CENTER_VERTICAL);
        brandRow.setOrientation(LinearLayout.HORIZONTAL);
        TextView mark = text("N", 26, BG, true);
        mark.setGravity(Gravity.CENTER);
        mark.setBackground(round(YELLOW, 16));
        brandRow.addView(mark, new LinearLayout.LayoutParams(dp(52), dp(52)));

        LinearLayout brandText = new LinearLayout(this);
        brandText.setOrientation(LinearLayout.VERTICAL);
        brandText.setPadding(dp(12), 0, 0, 0);
        brandText.addView(text("NUN", 27, WHITE, true));
        TextView subtitle = text("ANDROID SYSTEM SURFACE", 10, MUTED, true);
        subtitle.setLetterSpacing(0.12f);
        brandText.addView(subtitle);
        brandRow.addView(brandText, new LinearLayout.LayoutParams(
                0, LinearLayout.LayoutParams.WRAP_CONTENT, 1f));

        TextView settings = pill("SETTINGS", false);
        settings.setOnClickListener(v -> openSettings());
        brandRow.addView(settings);
        root.addView(brandRow);

        coreStatus = text(coreStatusText, 10, YELLOW, true);
        coreStatus.setPadding(0, dp(12), 0, dp(14));
        root.addView(coreStatus);

        LinearLayout tabs = new LinearLayout(this);
        tabs.setOrientation(LinearLayout.HORIZONTAL);
        tabs.setPadding(0, 0, 0, dp(12));
        tabs.addView(tab("HOME"), weighted());
        tabs.addView(tab("APPS"), weighted());
        tabs.addView(tab("SYSTEM"), weighted());
        root.addView(tabs);

        ScrollView scroll = new ScrollView(this);
        scroll.setFillViewport(true);
        body = new LinearLayout(this);
        body.setOrientation(LinearLayout.VERTICAL);
        body.setClipToPadding(false);
        scroll.addView(body);
        root.addView(scroll, new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT, 0, 1f));

        TextView footer = text("BLACK / GOLD INTERFACE  ·  NUN ECOSYSTEM", 9, MUTED, true);
        footer.setGravity(Gravity.CENTER);
        footer.setPadding(0, dp(12), 0, dp(4));
        root.addView(footer);
        setContentView(root);
        renderBody();
    }

    private LinearLayout.LayoutParams weighted() {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(0, dp(42), 1f);
        params.setMargins(dp(3), 0, dp(3), 0);
        return params;
    }

    private TextView tab(String label) {
        TextView view = pill(label, selectedTab.equals(label));
        view.setOnClickListener(v -> animateTap(v, () -> {
            selectedTab = label;
            refreshTabs();
            renderBody();
        }));
        return view;
    }

    private void refreshTabs() {
        if (root == null || root.getChildCount() < 3) return;
        LinearLayout tabRow = (LinearLayout) root.getChildAt(2);
        for (int i = 0; i < tabRow.getChildCount(); i++) {
            TextView tab = (TextView) tabRow.getChildAt(i);
            boolean active = tab.getText().toString().equals(selectedTab);
            tab.setTextColor(active ? BG : WHITE);
            tab.setBackground(round(active ? YELLOW : PANEL_ALT, 13));
        }
    }

    private void renderBody() {
        body.removeAllViews();
        appGrid = null;
        if ("APPS".equals(selectedTab)) renderApps();
        else if ("SYSTEM".equals(selectedTab)) renderSystem();
        else renderHome();
    }

    private void renderHome() {
        LinearLayout hero = card();
        hero.addView(text("SYSTEM OVERVIEW / 01", 10, YELLOW, true));
        hero.addView(text("One system.\nSeven source modules.", 25, WHITE, true));
        TextView detail = text(
                "NUN connects the native runtime, language tooling, platform services and ecosystem control plane.",
                13, MUTED, false);
        detail.setPadding(0, dp(8), 0, 0);
        hero.addView(detail);
        body.addView(hero, bottomMargin(dp(12)));

        LinearLayout metrics = new LinearLayout(this);
        metrics.setOrientation(LinearLayout.HORIZONTAL);
        metrics.addView(metric("07", "SOURCE MODULES"), metricParams());
        metrics.addView(metric("01", "NATIVE CORE"), metricParams());
        metrics.addView(metric("36", "ANDROID API"), metricParams());
        body.addView(metrics, bottomMargin(dp(12)));

        TextView section = text("QUICK ACCESS", 11, YELLOW, true);
        section.setPadding(0, dp(6), 0, dp(8));
        body.addView(section);

        LinearLayout quick = card();
        LinearLayout appsAction = actionRow("APP LIBRARY", "Browse installed applications", "↗");
        appsAction.setOnClickListener(v -> {
            selectedTab = "APPS";
            refreshTabs();
            renderBody();
        });
        quick.addView(appsAction);
        quick.addView(separator());
        LinearLayout systemAction = actionRow("SYSTEM MODULES", "Inspect the NUN source map", "↗");
        systemAction.setOnClickListener(v -> {
            selectedTab = "SYSTEM";
            refreshTabs();
            renderBody();
        });
        quick.addView(systemAction);
        body.addView(quick, bottomMargin(dp(12)));

        TextView note = text(
                "NUN Launcher is an Android HOME app. The native runtime runs inside this app process; it is not a bare-metal bootloader.",
                11, MUTED, false);
        note.setPadding(dp(3), dp(5), dp(3), dp(18));
        body.addView(note);
    }

    private void renderApps() {
        TextView heading = text("YOUR APPLICATIONS", 11, YELLOW, true);
        heading.setPadding(0, dp(4), 0, dp(10));
        body.addView(heading);

        EditText search = new EditText(this);
        search.setSingleLine(true);
        search.setTextColor(WHITE);
        search.setHintTextColor(MUTED);
        search.setHint("Search apps");
        search.setTextSize(14);
        search.setPadding(dp(14), 0, dp(14), 0);
        search.setBackground(round(PANEL_ALT, 14));
        search.setText(searchQuery);
        body.addView(search, bottomMargin(dp(12)));

        appGrid = new LinearLayout(this);
        appGrid.setOrientation(LinearLayout.VERTICAL);
        body.addView(appGrid);
        search.addTextChangedListener(new TextWatcher() {
            @Override public void beforeTextChanged(CharSequence s, int start, int count, int after) {}
            @Override public void onTextChanged(CharSequence s, int start, int before, int count) {
                searchQuery = s.toString();
                renderAppGrid();
            }
            @Override public void afterTextChanged(Editable s) {}
        });
        renderAppGrid();

        TextView refresh = pill("REFRESH APPLICATIONS", false);
        refresh.setOnClickListener(v -> animateTap(v, () -> {
            refreshApps();
            renderAppGrid();
        }));
        body.addView(refresh, bottomMargin(dp(20)));
    }

    private void renderAppGrid() {
        if (appGrid == null) return;
        appGrid.removeAllViews();
        PackageManager pm = getPackageManager();
        List<AppEntry> filtered = new ArrayList<>();
        for (AppEntry app : apps) {
            if (app.label.toLowerCase(Locale.ROOT)
                    .contains(searchQuery.toLowerCase(Locale.ROOT))) filtered.add(app);
        }
        if (filtered.isEmpty()) {
            TextView empty = text("No matching applications.", 13, MUTED, false);
            empty.setPadding(dp(4), dp(18), dp(4), dp(18));
            appGrid.addView(empty);
            return;
        }
        final int columns = 3;
        for (int index = 0; index < filtered.size(); index += columns) {
            LinearLayout row = new LinearLayout(this);
            row.setOrientation(LinearLayout.HORIZONTAL);
            for (int column = 0; column < columns; column++) {
                int current = index + column;
                if (current < filtered.size()) {
                    AppEntry entry = filtered.get(current);
                    row.addView(appTile(entry, pm), appTileParams());
                } else {
                    row.addView(new View(this), appTileParams());
                }
            }
            appGrid.addView(row, bottomMargin(dp(8)));
        }
    }

    private View appTile(AppEntry entry, PackageManager pm) {
        LinearLayout tile = new LinearLayout(this);
        tile.setOrientation(LinearLayout.VERTICAL);
        tile.setGravity(Gravity.CENTER);
        tile.setPadding(dp(6), dp(12), dp(6), dp(10));
        tile.setBackground(round(PANEL, 16));

        ImageView icon = new ImageView(this);
        icon.setImageDrawable(entry.info.loadIcon(pm));
        tile.addView(icon, new LinearLayout.LayoutParams(dp(42), dp(42)));

        TextView label = text(entry.label, 11, WHITE, true);
        label.setGravity(Gravity.CENTER);
        label.setMaxLines(2);
        label.setPadding(0, dp(8), 0, 0);
        tile.addView(label);
        tile.setOnClickListener(v -> animateTap(v, () -> launchApp(entry.packageName)));
        return tile;
    }

    private LinearLayout.LayoutParams appTileParams() {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(0, dp(112), 1f);
        params.setMargins(dp(3), 0, dp(3), 0);
        return params;
    }

    private void renderSystem() {
        LinearLayout overview = card();
        overview.addView(text("NUN CORE / STATUS", 10, YELLOW, true));
        overview.addView(text(coreStarted ? "RUNTIME ACTIVE" :
                (NunCore.isAvailable() ? "RUNTIME STOPPED" : "NATIVE BRIDGE NOT LOADED"),
                21, WHITE, true));
        overview.addView(text(coreStarted
                ? "The native C runtime is polling through the Android monotonic clock adapter."
                : "The complete NUN source tree is linked. Runtime activation depends on a successful Android NDK build.",
                12, MUTED, false));
        body.addView(overview, bottomMargin(dp(12)));

        TextView heading = text("SOURCE MODULES", 11, YELLOW, true);
        heading.setPadding(0, dp(4), 0, dp(8));
        body.addView(heading);

        for (ModuleEntry module : modules) {
            LinearLayout item = card();
            LinearLayout top = new LinearLayout(this);
            top.setGravity(Gravity.CENTER_VERTICAL);
            TextView name = text(module.id, 15, WHITE, true);
            top.addView(name, new LinearLayout.LayoutParams(0,
                    LinearLayout.LayoutParams.WRAP_CONTENT, 1f));
            top.addView(pill("LINKED SOURCE", true));
            item.addView(top);
            TextView role = text(module.role, 12, MUTED, false);
            role.setPadding(0, dp(7), 0, dp(4));
            item.addView(role);
            item.addView(text(module.boundary, 10, YELLOW, false));
            body.addView(item, bottomMargin(dp(8)));
        }

        TextView settings = pill("OPEN ANDROID SETTINGS", false);
        settings.setOnClickListener(v -> openSettings());
        body.addView(settings, bottomMargin(dp(16)));
    }

    private void loadModules() {
        modules.clear();
        try (InputStream input = getAssets().open("nun-modules.json");
             ByteArrayOutputStream output = new ByteArrayOutputStream()) {
            byte[] buffer = new byte[1024];
            int read;
            while ((read = input.read(buffer)) != -1) output.write(buffer, 0, read);
            JSONObject data = new JSONObject(new String(output.toByteArray(), StandardCharsets.UTF_8));
            JSONArray array = data.getJSONArray("modules");
            for (int i = 0; i < array.length(); i++) {
                JSONObject item = array.getJSONObject(i);
                modules.add(new ModuleEntry(item.optString("id", "UNKNOWN"),
                        item.optString("role", ""), item.optString("boundary", "")));
            }
        } catch (Exception ignored) {
            modules.clear();
        }
    }

    private void refreshApps() {
        apps.clear();
        Intent query = new Intent(Intent.ACTION_MAIN);
        query.addCategory(Intent.CATEGORY_LAUNCHER);
        List<ResolveInfo> resolved = getPackageManager().queryIntentActivities(query, 0);
        for (ResolveInfo info : resolved) {
            String packageName = info.activityInfo == null ? "" : info.activityInfo.packageName;
            if (packageName.isEmpty() || packageName.equals(getPackageName())) continue;
            CharSequence label = info.loadLabel(getPackageManager());
            apps.add(new AppEntry(packageName,
                    label == null ? packageName : label.toString(), info));
        }
        Collections.sort(apps, Comparator.comparing(
                entry -> entry.label.toLowerCase(Locale.ROOT)));
    }

    private void launchApp(String packageName) {
        Intent launch = getPackageManager().getLaunchIntentForPackage(packageName);
        if (launch == null) {
            Toast.makeText(this, "Application cannot be opened", Toast.LENGTH_SHORT).show();
            return;
        }
        launch.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        try {
            startActivity(launch);
        } catch (Exception error) {
            Toast.makeText(this, "Launch failed", Toast.LENGTH_SHORT).show();
        }
    }

    private void openSettings() {
        try {
            startActivity(new Intent(Settings.ACTION_SETTINGS));
        } catch (Exception ignored) {
            Toast.makeText(this, "Settings unavailable", Toast.LENGTH_SHORT).show();
        }
    }

    private LinearLayout metric(String value, String label) {
        LinearLayout panel = new LinearLayout(this);
        panel.setOrientation(LinearLayout.VERTICAL);
        panel.setPadding(dp(12), dp(12), dp(8), dp(12));
        panel.setBackground(round(PANEL, 14));
        panel.addView(text(value, 23, YELLOW, true));
        TextView caption = text(label, 8, MUTED, true);
        caption.setPadding(0, dp(3), 0, 0);
        panel.addView(caption);
        return panel;
    }

    private LinearLayout.LayoutParams metricParams() {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(0,
                LinearLayout.LayoutParams.WRAP_CONTENT, 1f);
        params.setMargins(dp(3), 0, dp(3), 0);
        return params;
    }

    private LinearLayout actionRow(String title, String detail, String glyph) {
        LinearLayout row = new LinearLayout(this);
        row.setOrientation(LinearLayout.HORIZONTAL);
        row.setGravity(Gravity.CENTER_VERTICAL);
        row.setPadding(dp(8), dp(10), dp(8), dp(10));
        row.setBackground(round(PANEL_ALT, 12));
        TextView label = text(title + "\n" + detail, 12, WHITE, true);
        row.addView(label, new LinearLayout.LayoutParams(0,
                LinearLayout.LayoutParams.WRAP_CONTENT, 1f));
        row.addView(text(glyph, 18, YELLOW, true));
        row.setClickable(true);
        row.setFocusable(true);
        return row;
    }

    private LinearLayout card() {
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setPadding(dp(16), dp(16), dp(16), dp(16));
        layout.setBackground(round(PANEL, 18));
        return layout;
    }

    private TextView text(String value, int size, int color, boolean bold) {
        TextView view = new TextView(this);
        view.setText(value);
        view.setTextSize(size);
        view.setTextColor(color);
        if (bold) view.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        return view;
    }

    private TextView pill(String value, boolean active) {
        TextView view = text(value, 10, active ? BG : WHITE, true);
        view.setGravity(Gravity.CENTER);
        view.setPadding(dp(13), 0, dp(13), 0);
        view.setBackground(round(active ? YELLOW : PANEL_ALT, 13));
        view.setClickable(true);
        view.setFocusable(true);
        return view;
    }

    private GradientDrawable round(int color, int radius) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setColor(color);
        drawable.setCornerRadius(dp(radius));
        return drawable;
    }

    private View separator() {
        View view = new View(this);
        view.setBackgroundColor(Color.rgb(48, 53, 57));
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT, dp(1));
        params.setMargins(0, dp(6), 0, dp(6));
        view.setLayoutParams(params);
        return view;
    }

    private LinearLayout.LayoutParams bottomMargin(int margin) {
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT);
        params.bottomMargin = margin;
        return params;
    }

    private void animateTap(View view, Runnable action) {
        view.animate().scaleX(0.975f).scaleY(0.975f)
                .setDuration(80L)
                .setInterpolator(new AccelerateDecelerateInterpolator())
                .withEndAction(() -> view.animate().scaleX(1f).scaleY(1f)
                        .setDuration(90L).withEndAction(action).start())
                .start();
    }

    private int dp(int value) {
        return Math.round(value * getResources().getDisplayMetrics().density);
    }

    private static final class AppEntry {
        final String packageName;
        final String label;
        final ResolveInfo info;
        AppEntry(String packageName, String label, ResolveInfo info) {
            this.packageName = packageName;
            this.label = label;
            this.info = info;
        }
    }

    private static final class ModuleEntry {
        final String id;
        final String role;
        final String boundary;
        ModuleEntry(String id, String role, String boundary) {
            this.id = id;
            this.role = role;
            this.boundary = boundary;
        }
    }
}
