package org.nun.launcher;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.provider.Settings;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

/**
 * Minimal, ordinary Android Home application.
 *
 * This Activity does not implement a firmware loader, bootloader interface,
 * root workflow, partition writer or privileged NUN system service.
 */
public final class MainActivity extends Activity {
    private static final int COLOR_CANVAS = Color.rgb(13, 13, 13);
    private static final int COLOR_SURFACE = Color.rgb(26, 26, 26);
    private static final int COLOR_YELLOW = Color.rgb(255, 214, 0);
    private static final int COLOR_TEXT = Color.rgb(230, 230, 230);
    private static final int COLOR_MUTED = Color.rgb(153, 153, 153);
    private static final int COLOR_BORDER = Color.rgb(38, 38, 38);

    private final List<ResolveInfo> visibleApps = new ArrayList<>();

    private GridLayout appGrid;
    private EditText searchField;
    private TextView statusText;
    private TextView emptyText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setStatusBarColor(COLOR_CANVAS);
        getWindow().setNavigationBarColor(COLOR_CANVAS);
        getWindow().getDecorView().setSystemUiVisibility(0);

        setContentView(buildHomeSurface());
    }

    @Override
    protected void onResume() {
        super.onResume();
        refreshApplications();
    }

    private View buildHomeSurface() {
        LinearLayout root = new LinearLayout(this);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setBackgroundColor(COLOR_CANVAS);
        root.setPadding(dp(18), dp(14), dp(18), dp(12));
        root.setFitsSystemWindows(true);

        LinearLayout header = new LinearLayout(this);
        header.setOrientation(LinearLayout.HORIZONTAL);
        header.setGravity(Gravity.CENTER_VERTICAL);

        ImageView mark = new ImageView(this);
        mark.setImageResource(R.drawable.duck_mark);
        mark.setContentDescription("NUN duck mark");
        LinearLayout.LayoutParams markParams =
                new LinearLayout.LayoutParams(dp(48), dp(48));
        markParams.rightMargin = dp(12);
        header.addView(mark, markParams);

        LinearLayout identity = new LinearLayout(this);
        identity.setOrientation(LinearLayout.VERTICAL);

        TextView brand = new TextView(this);
        brand.setText("NUN");
        brand.setTextColor(COLOR_YELLOW);
        brand.setTextSize(27);
        brand.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        brand.setIncludeFontPadding(false);
        identity.addView(brand);

        TextView subtitle = new TextView(this);
        subtitle.setText(R.string.home_subtitle);
        subtitle.setTextColor(COLOR_MUTED);
        subtitle.setTextSize(10);
        subtitle.setLetterSpacing(0.08f);
        identity.addView(subtitle);

        header.addView(identity, new LinearLayout.LayoutParams(
                0, ViewGroup.LayoutParams.WRAP_CONTENT, 1f));

        TextView hostTag = new TextView(this);
        hostTag.setText("HOST");
        hostTag.setTextColor(COLOR_YELLOW);
        hostTag.setTextSize(10);
        hostTag.setTypeface(Typeface.MONOSPACE, Typeface.BOLD);
        hostTag.setPadding(dp(9), dp(6), dp(9), dp(6));
        hostTag.setBackground(rounded(COLOR_SURFACE, COLOR_YELLOW, dp(6)));
        header.addView(hostTag);

        root.addView(header, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        TextView sectionTitle = new TextView(this);
        sectionTitle.setText("APPLICATIONS");
        sectionTitle.setTextColor(COLOR_TEXT);
        sectionTitle.setTextSize(13);
        sectionTitle.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        sectionTitle.setLetterSpacing(0.08f);
        LinearLayout.LayoutParams titleParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        titleParams.topMargin = dp(28);
        titleParams.bottomMargin = dp(10);
        root.addView(sectionTitle, titleParams);

        searchField = new EditText(this);
        searchField.setSingleLine(true);
        searchField.setTextColor(COLOR_TEXT);
        searchField.setHintTextColor(COLOR_MUTED);
        searchField.setTextSize(15);
        searchField.setHint(R.string.search_hint);
        searchField.setPadding(dp(14), dp(11), dp(14), dp(11));
        searchField.setBackground(rounded(COLOR_SURFACE, COLOR_BORDER, dp(8)));
        searchField.setCompoundDrawablePadding(dp(8));
        root.addView(searchField, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, dp(50)));

        statusText = new TextView(this);
        statusText.setTextColor(COLOR_MUTED);
        statusText.setTextSize(11);
        statusText.setTypeface(Typeface.MONOSPACE, Typeface.NORMAL);
        LinearLayout.LayoutParams statusParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        statusParams.topMargin = dp(12);
        statusParams.bottomMargin = dp(8);
        root.addView(statusText, statusParams);

        ScrollView scroll = new ScrollView(this);
        scroll.setFillViewport(true);
        scroll.setClipToPadding(false);

        LinearLayout gridContainer = new LinearLayout(this);
        gridContainer.setOrientation(LinearLayout.VERTICAL);

        appGrid = new GridLayout(this);
        appGrid.setAlignmentMode(GridLayout.ALIGN_BOUNDS);
        appGrid.setUseDefaultMargins(false);
        gridContainer.addView(appGrid, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        emptyText = new TextView(this);
        emptyText.setText(R.string.empty_apps);
        emptyText.setTextColor(COLOR_MUTED);
        emptyText.setTextSize(14);
        emptyText.setGravity(Gravity.CENTER);
        emptyText.setPadding(dp(16), dp(36), dp(16), dp(36));
        emptyText.setVisibility(View.GONE);
        gridContainer.addView(emptyText, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        scroll.addView(gridContainer);
        root.addView(scroll, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, 0, 1f));

        Button settingsButton = new Button(this);
        settingsButton.setText(R.string.settings_button);
        settingsButton.setTextColor(COLOR_YELLOW);
        settingsButton.setTextSize(12);
        settingsButton.setAllCaps(false);
        settingsButton.setBackground(rounded(COLOR_SURFACE, COLOR_YELLOW, dp(8)));
        settingsButton.setOnClickListener(view -> openAndroidSettings());
        LinearLayout.LayoutParams settingsParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, dp(48));
        settingsParams.topMargin = dp(10);
        root.addView(settingsButton, settingsParams);

        searchField.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence text, int start, int count, int after) {
                // No work is required before a local filter update.
            }

            @Override
            public void onTextChanged(CharSequence text, int start, int before, int count) {
                renderApplications();
            }

            @Override
            public void afterTextChanged(Editable editable) {
                // The visible catalog is already filtered by onTextChanged.
            }
        });

        return root;
    }

    private void refreshApplications() {
        Intent launcherQuery = new Intent(Intent.ACTION_MAIN);
        launcherQuery.addCategory(Intent.CATEGORY_LAUNCHER);

        PackageManager packageManager = getPackageManager();
        List<ResolveInfo> discovered =
                packageManager.queryIntentActivities(launcherQuery, 0);

        visibleApps.clear();
        for (ResolveInfo item : discovered) {
            if (item == null || item.activityInfo == null) {
                continue;
            }
            if (getPackageName().equals(item.activityInfo.packageName)) {
                continue;
            }
            visibleApps.add(item);
        }

        Collections.sort(visibleApps, new Comparator<ResolveInfo>() {
            @Override
            public int compare(ResolveInfo left, ResolveInfo right) {
                String leftLabel = left.loadLabel(packageManager).toString();
                String rightLabel = right.loadLabel(packageManager).toString();
                return leftLabel.compareToIgnoreCase(rightLabel);
            }
        });

        renderApplications();
    }

    private void renderApplications() {
        if (appGrid == null || searchField == null) {
            return;
        }

        appGrid.removeAllViews();
        int widthDp = getResources().getConfiguration().screenWidthDp;
        int columns = Math.max(3, Math.min(6, widthDp / 88));
        appGrid.setColumnCount(columns);

        String query = searchField.getText() == null
                ? ""
                : searchField.getText().toString();
        PackageManager packageManager = getPackageManager();
        int shown = 0;

        for (ResolveInfo item : visibleApps) {
            if (item.activityInfo == null) {
                continue;
            }
            String label = item.loadLabel(packageManager).toString();
            String packageName = item.activityInfo.packageName;
            if (!LauncherFilter.matches(label, packageName, query)) {
                continue;
            }

            appGrid.addView(createApplicationTile(item, label),
                    createTileLayoutParams());
            shown++;
        }

        statusText.setText(shown + " / " + visibleApps.size() + " AVAILABLE");
        emptyText.setVisibility(shown == 0 ? View.VISIBLE : View.GONE);
    }

    private View createApplicationTile(ResolveInfo item, String label) {
        LinearLayout tile = new LinearLayout(this);
        tile.setOrientation(LinearLayout.VERTICAL);
        tile.setGravity(Gravity.CENTER);
        tile.setPadding(dp(5), dp(10), dp(5), dp(8));
        tile.setBackground(rounded(COLOR_SURFACE, COLOR_BORDER, dp(8)));
        tile.setClickable(true);
        tile.setFocusable(true);
        tile.setContentDescription(getString(R.string.app_tile_description, label));

        ImageView icon = new ImageView(this);
        icon.setScaleType(ImageView.ScaleType.FIT_CENTER);
        Drawable appIcon;
        try {
            appIcon = item.loadIcon(getPackageManager());
        } catch (RuntimeException exception) {
            appIcon = getPackageManager().getDefaultActivityIcon();
        }
        icon.setImageDrawable(appIcon);
        tile.addView(icon, new LinearLayout.LayoutParams(dp(46), dp(46)));

        TextView name = new TextView(this);
        name.setText(label);
        name.setTextColor(COLOR_TEXT);
        name.setTextSize(11);
        name.setGravity(Gravity.CENTER);
        name.setMaxLines(2);
        name.setEllipsize(android.text.TextUtils.TruncateAt.END);
        name.setPadding(0, dp(7), 0, 0);
        tile.addView(name, new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        tile.setOnClickListener(view -> launchApplication(item));
        return tile;
    }

    private GridLayout.LayoutParams createTileLayoutParams() {
        GridLayout.LayoutParams params = new GridLayout.LayoutParams();
        params.width = 0;
        params.height = dp(104);
        params.columnSpec = GridLayout.spec(GridLayout.UNDEFINED, 1f);
        params.setMargins(dp(4), dp(4), dp(4), dp(4));
        return params;
    }

    private void launchApplication(ResolveInfo item) {
        if (item.activityInfo == null) {
            Toast.makeText(this, "Application activity is unavailable",
                    Toast.LENGTH_SHORT).show();
            return;
        }

        ComponentName component = new ComponentName(
                item.activityInfo.packageName,
                item.activityInfo.name);
        Intent launch = new Intent(Intent.ACTION_MAIN);
        launch.addCategory(Intent.CATEGORY_LAUNCHER);
        launch.setComponent(component);
        launch.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);

        try {
            startActivity(launch);
        } catch (ActivityNotFoundException | SecurityException exception) {
            Toast.makeText(this, "Application could not be opened",
                    Toast.LENGTH_SHORT).show();
            refreshApplications();
        }
    }

    private void openAndroidSettings() {
        try {
            startActivity(new Intent(Settings.ACTION_SETTINGS));
        } catch (ActivityNotFoundException exception) {
            Toast.makeText(this, "Android Settings is unavailable",
                    Toast.LENGTH_SHORT).show();
        }
    }

    private GradientDrawable rounded(int fillColor, int strokeColor, int radiusPx) {
        GradientDrawable drawable = new GradientDrawable();
        drawable.setShape(GradientDrawable.RECTANGLE);
        drawable.setColor(fillColor);
        drawable.setCornerRadius(radiusPx);
        drawable.setStroke(dp(1), strokeColor);
        return drawable;
    }

    private int dp(float value) {
        return Math.round(value * getResources().getDisplayMetrics().density);
    }
}
